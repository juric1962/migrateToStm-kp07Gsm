#include "firmware.h"
#include "stm32f4xx.h"
#include "dfpin.h"
#include <stdint.h>

/*
 * Все функции этого файла помещаются
 * в секцию BOOT_CODE.
 */
//#pragma default_function_attributes = @ "BOOT_CODE"

#define FLASH_KEY1       0x45670123U
#define FLASH_KEY2       0xCDEF89ABU

//#define FLASH_SR_EOP     (1U << 0)
#define FLASH_SR_OPERR   (1U << 1)
//#define FLASH_SR_WRPERR  (1U << 4)
//#define FLASH_SR_PGAERR  (1U << 5)
//#define FLASH_SR_PGPERR  (1U << 6)
//#define FLASH_SR_PGSERR  (1U << 7)
//#define FLASH_SR_BSY     (1U << 16)

#define FLASH_SR_ERRORS  (FLASH_SR_OPERR  | \
                          FLASH_SR_WRPERR | \
                          FLASH_SR_PGAERR | \
                          FLASH_SR_PGPERR | \
                          FLASH_SR_PGSERR)

//#define FLASH_CR_PG      (1U << 0)
//#define FLASH_CR_SER     (1U << 1)
//#define FLASH_CR_SNB_Msk (0xFU << 3)
//#define FLASH_CR_PSIZE_Msk (3U << 8)
//#define FLASH_CR_STRT    (1U << 16)
//#define FLASH_CR_LOCK    (1U << 31)

#define BOOTFLASH_OK       0
#define BOOTFLASH_ERROR   -1
#define BOOTFLASH_TIMEOUT -2
#define BOOTFLASH_PARAM   -3

/*
 * Ограничение ожидания, чтобы не зависнуть навечно.
 * Значение нужно проверить на реальной частоте CPU.
 */
#define FLASH_WAIT_LIMIT  10000000U
// static - область видимости только в пределах этого файла


/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
static void My_NVIC_SystemReset(void)  @ "BOOT_CODE"
{
  __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
  SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
                           (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                            SCB_AIRCR_SYSRESETREQ_Msk    );         /* Keep priority group unchanged */
  __DSB();                                                          /* Ensure completion of memory access */

  for(;;)                                                           /* wait until reset */
  {
    __NOP();
  }
}



static void SV2_RD(void) @ "BOOT_CODE"
{
    S2_R_PORT->BSRR = (uint32_t)S2_R_PIN << 16;
    S2_G_PORT->BSRR = S2_G_PIN;
}

static void SV2_GR(void) @ "BOOT_CODE"
{
    S2_R_PORT->BSRR = S2_R_PIN;
    S2_G_PORT->BSRR = (uint32_t)S2_G_PIN << 16;
}

 static int BootFlash_WaitReady(void) @ "BOOT_CODE"
{
    uint32_t timeout = FLASH_WAIT_LIMIT;

    while ((FLASH->SR & FLASH_SR_BSY) != 0U)
    {
        IWDG->KR = 0xAAAAU; // сброс watchdog я его запретить не могу 
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
static int BootFlash_Unlock(void) @ "BOOT_CODE"
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
static void BootFlash_Lock(void) @ "BOOT_CODE"
{
    FLASH->CR |= FLASH_CR_LOCK;
}

/*
 * Стереть один сектор:
 * sector = 0..11 для STM32F405.
 */
static int BootFlash_EraseSector(uint32_t sector) @ "BOOT_CODE"
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
static int BootFlash_ProgramWord(uint32_t address, uint32_t data) @ "BOOT_CODE"
{
    int result;
    SV2_GR();
    if ((address & 3U) != 0U)
        return BOOTFLASH_PARAM;

    /* Адрес должен находиться в пользовательской Flash */
    if ((address < 0x08000000U) ||
        (address >= 0x08060000U))
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
static uint32_t BootFlash_ReadWord(uint32_t address) @ "BOOT_CODE"
{
    return *(volatile const uint32_t *)address;
}





/*
Размещаем секцию по фиксированному адресу
В IAR .icf:

place at address mem:0x080E0000
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
*/

/*

это программа загрузчик Сергея рабочая

*/


void SelfProgramm(void) @ "BOOT_CODE"
{
  
  
__IO uint32_t dst = 0x08000000;
__IO uint32_t src = 0x08060000;
__IO int i,err;
  __disable_irq();
if (BootFlash_Unlock() != BOOTFLASH_OK) goto exitProgram;



/*

new program present ? in source adress
*/
if (*(uint32_t*)src == 0xffffffff) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_6 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_5 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_4 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_3 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_2 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_1 ) != BOOTFLASH_OK) goto exitProgram;
if (BootFlash_EraseSector( ADDR_FLASH_SECTOR_0 ) != BOOTFLASH_OK) goto exitProgram;




for (i = 0; i < (0x60000/4); ++i)
{
  for ( err = 0; err < 4; err++)               // 4 attempt to write 
  {
  IWDG->KR = 0xAAAAU; // сброс watchdog я его запретить не могу  
if( BootFlash_ProgramWord( dst, *(uint32_t*)src ) != BOOTFLASH_OK) { continue; }  // no ready
if ((*(__IO uint32_t *)dst) != (*(__IO uint32_t*)src)) { continue; }               // bad write
else { goto next_word;}
 }
if( err >= 3 ) { SV2_RD(); goto exitProgram; }  
next_word: 
  src += 4;
  dst += 4;
}

exitProgram:
     BootFlash_Lock();
     My_NVIC_SystemReset();  // recomendation gpt

}
//#pragma default_function_attributes =

