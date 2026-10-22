#ifndef __FIRMWARE_H
#define __FIRMWARE_H

#include "stm32f4xx_hal.h"

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) // Сектор 0, 16 кбайт
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) // Сектор 1, 16 кбайт
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) // Сектор 2, 16 кбайт
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) // Сектор 3, 16 кбайт
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) // Сектор 4, 64 кбайт
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) // Сектор 5, 128 кбайт
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) // Сектор 6, 128 кбайт
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) // Сектор 7, 128 кбайт
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) // Сектор 8, 128 кбайт
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) // Сектор 9, 128 кбайт
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) // Сектор 10, 128 кбайт
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) // Сектор 11, 128 кбайт

#define MEMORY_PTR(addr) (*((volatile unsigned long *)(addr)))





#define USER_FLASH_END_ADDRESS        0x080FFFFF
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)


void SelfProgramm (void);

static void FLASH_If_Init(void);
uint32_t flashProgramm(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength);
uint32_t FLASH_If_Erase(uint32_t StartSector);

uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint32_t DataLength);
static uint16_t FLASH_If_GetWriteProtectionStatus(uint32_t StartSector);
static uint32_t FLASH_If_DisableWriteProtection(uint32_t StartSector);
static uint32_t GetSector(uint32_t Address);


typedef  void (*pFunction)(void);

#endif 