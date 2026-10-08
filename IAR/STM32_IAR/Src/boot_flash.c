#include "stm32f4xx.h"
#include <stdint.h>

/*
 * Все функции этого файла помещаются
 * в секцию BOOT_CODE.
 */
#pragma default_function_attributes = @ "BOOT_CODE"

#define FLASH_KEY1       0x45670123U
#define FLASH_KEY2       0xCDEF89ABU

#define FLASH_SR_EOP     (1U << 0)
#define FLASH_SR_OPERR   (1U << 1)
#define FLASH_SR_WRPERR  (1U << 4)
#define FLASH_SR_PGAERR  (1U << 5)
#define FLASH_SR_PGPERR  (1U << 6)
#define FLASH_SR_PGSERR  (1U << 7)
#define FLASH_SR_BSY     (1U << 16)

#define FLASH_SR_ERRORS  (FLASH_SR_OPERR  | \
                          FLASH_SR_WRPERR | \
                          FLASH_SR_PGAERR | \
                          FLASH_SR_PGPERR | \
                          FLASH_SR_PGSERR)

#define FLASH_CR_PG      (1U << 0)
#define FLASH_CR_SER     (1U << 1)
#define FLASH_CR_SNB_Msk (0xFU << 3)
#define FLASH_CR_PSIZE_Msk (3U << 8)
#define FLASH_CR_STRT    (1U << 16)
#define FLASH_CR_LOCK    (1U << 31)

#define BOOTFLASH_OK       0
#define BOOTFLASH_ERROR   -1
#define BOOTFLASH_TIMEOUT -2
#define BOOTFLASH_PARAM   -3

/*
 * Ограничение ожидания, чтобы не зависнуть навечно.
 * Значение нужно проверить на реальной частоте CPU.
 */
#define FLASH_WAIT_LIMIT  10000000U

static int BootFlash_WaitReady(void)
{
    uint32_t timeout = FLASH_WAIT_LIMIT;

    while ((FLASH->SR & FLASH_SR_BSY) != 0U)
    {
        if (--timeout == 0U)
            return BOOTFLASH_TIMEOUT;
    }

    if ((FLASH->SR & FLASH_SR_ERRORS) != 0U)
    {
        FLASH->SR = FLASH_SR_ERRORS;
        return BOOTFLASH_ERROR;
    }

    return BOOTFLASH_OK;
}

/* Разблокировать Flash */
int BootFlash_Unlock(void)
{
    if ((FLASH->CR & FLASH_CR_LOCK) != 0U)
    {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }

    if ((FLASH->CR & FLASH_CR_LOCK) != 0U)
        return BOOTFLASH_ERROR;

    return BOOTFLASH_OK;
}

/* Заблокировать Flash */
void BootFlash_Lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

/*
 * Стереть один сектор:
 * sector = 0..11 для STM32F405.
 */
int BootFlash_EraseSector(uint32_t sector)
{
    int result;

    if (sector > 11U)
        return BOOTFLASH_PARAM;

    result = BootFlash_WaitReady();
    if (result != BOOTFLASH_OK)
        return result;

    FLASH->SR = FLASH_SR_EOP | FLASH_SR_ERRORS;

    FLASH->CR &= ~(FLASH_CR_PG |
                   FLASH_CR_SER |
                   FLASH_CR_SNB_Msk |
                   FLASH_CR_PSIZE_Msk);

    /* PSIZE = 10: программирование 32-битным словом */
    FLASH->CR |= (2U << 8);

    FLASH->CR |= FLASH_CR_SER | (sector << 3);
    FLASH->CR |= FLASH_CR_STRT;

    result = BootFlash_WaitReady();

    FLASH->CR &= ~(FLASH_CR_SER | FLASH_CR_SNB_Msk);

    if (result != BOOTFLASH_OK)
        return result;

    if ((FLASH->SR & FLASH_SR_EOP) != 0U)
        FLASH->SR = FLASH_SR_EOP;

    return BOOTFLASH_OK;
}

/*
 * Записать одно 32-битное слово.
 * address должен быть выровнен по 4 байтам.
 */
int BootFlash_ProgramWord(uint32_t address, uint32_t data)
{
    int result;

    if ((address & 3U) != 0U)
        return BOOTFLASH_PARAM;

    /* Адрес должен находиться в пользовательской Flash */
    if ((address < 0x08000000U) ||
        (address >= 0x08100000U))
        return BOOTFLASH_PARAM;

    result = BootFlash_WaitReady();
    if (result != BOOTFLASH_OK)
        return result;

    FLASH->SR = FLASH_SR_EOP | FLASH_SR_ERRORS;

    FLASH->CR &= ~(FLASH_CR_PG |
                   FLASH_CR_SER |
                   FLASH_CR_SNB_Msk |
                   FLASH_CR_PSIZE_Msk);

    FLASH->CR |= FLASH_CR_PG | (2U << 8);

    *(volatile uint32_t *)address = data;

    result = BootFlash_WaitReady();

    FLASH->CR &= ~FLASH_CR_PG;

    if (result != BOOTFLASH_OK)
        return result;

    if ((FLASH->SR & FLASH_SR_EOP) != 0U)
        FLASH->SR = FLASH_SR_EOP;

    if (*(volatile uint32_t *)address != data)
        return BOOTFLASH_ERROR;

    return BOOTFLASH_OK;
}

/* Прочитать одно 32-битное слово */
uint32_t BootFlash_ReadWord(uint32_t address)
{
    return *(volatile const uint32_t *)address;
}

#pragma default_function_attributes =




Размещаем секцию по фиксированному адресу
В IAR .icf:

place at address mem:0x08020000
{
    readonly section BOOT_CODE
};

Пример использования

if (BootFlash_Unlock() == BOOTFLASH_OK)
{
    if (BootFlash_EraseSector( sector ) == BOOTFLASH_OK)
    {
        BootFlash_ProgramWord(address, data);
    }

    BootFlash_Lock();
}

Здесь sector, address и data — параметры твоего алгоритма обновления.
