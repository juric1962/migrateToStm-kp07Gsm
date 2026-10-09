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


/*

это программа загрузчик Сергея рабочая

*/


int main(void)
{
  /* USER CODE BEGIN 1 */
  
  
  
  __set_PRIMASK(1); //????????? ??????????

SCB->VTOR = 0x080E0000;//????????? ?????? ??????? ?????????? ?? ?????????? ??????

__set_PRIMASK(0);//????????? ??????????


  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
HAL_IWDG_Start(&hiwdg); 
  /* USER CODE END 2 */
FLASH_Unlock ();  




/*    
while(1)
{
Delay_us(500000);
HAL_IWDG_Refresh(&hiwdg);
LED1_RED(1);
LED2_RED(1);
LED3_RED(1);
Delay_us(500000);
HAL_IWDG_Refresh(&hiwdg);
LED1_RED(0);
LED2_RED(0);
LED3_RED(0);
}
*/

// test 
/*
if (FLASH_If_Erase10() )
{
LED1_RED(1);
LED2_RED(1);
LED3_RED(1);
while(1);
}
*/



__IO uint32_t dst = 0x08000000;
__IO uint32_t src = 0x08060000;
__IO int i,err;
/*

new program present ? in source adress
*/
if (*(uint32_t*)src == 0xffff) goto exitProgram;


if (FLASH_If_Erase1(0x08000000) )
{
   FLASH_Lock();
   
   LED1_RED(1);
   LED2_RED(1);
   LED3_RED(1);
   
   while (1)
  {
   //HAL_IWDG_Refresh(&hiwdg);
  }
}

/*
for (i = 0; i < (0x60000/4); ++i)
{
while (FLASH_ProgramWord(dst, *(uint32_t*)src) != FLASH_COMPLETE_OLD) {HAL_IWDG_Refresh(&hiwdg); }
if ((*(uint32_t *)dst) != (*(uint32_t*)src))
{

FLASH_Lock();

}
src += 4;
dst += 4;
}
*/
LED1_GREEN(1);
//LED2_GREEN(1);
//LED3_GREEN(1);

LED1_RED(0);
LED2_RED(0);
LED3_RED(0);

for (i = 0; i < (0x60000/4); ++i)
{
  for ( err = 0; err < 4; err++)               // 4 attempt to write 
  {
if( FLASH_ProgramWord( dst, *(uint32_t*)src ) != FLASH_COMPLETE_OLD) {HAL_IWDG_Refresh(&hiwdg); LED1_RED(1) ; continue; }  // no ready
if ((*(__IO uint32_t *)dst) != (*(__IO uint32_t*)src)) {HAL_IWDG_Refresh(&hiwdg); LED2_RED(1) ;continue; }                          // bad write
else { goto next_word;}
 }

if( err >= 3 ) 
{
  FLASH_Lock();
  LED3_RED(1) ;
   while (1)
  {
   HAL_IWDG_Refresh(&hiwdg);
  }
}

next_word:
  
src += 4;
dst += 4;
}

LED2_GREEN(1);


exitProgram:
//FLASH_If_Erase2(0x08060000);  
FLASH_Lock();
Delay_us(300000);
 HAL_IWDG_Refresh(&hiwdg);
LED3_GREEN(1);
Delay_us(300000);
 HAL_IWDG_Refresh(&hiwdg);
//__disable_irq ();
  uint32_t JumpAddress ;
  uint32_t Address = 0x08000000;
  typedef void(*pFunction)(void);
  pFunction Jump_To_Application; 



    JumpAddress = *(__IO uint32_t*) (Address + 4);
  Jump_To_Application = (pFunction) JumpAddress;

