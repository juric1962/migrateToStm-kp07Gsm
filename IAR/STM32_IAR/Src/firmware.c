#include "firmware.h"
#include "stm32f4xx_flash.h"
#include "main.h"
#include "stm32f4xx_hal_iwdg.h"
extern IWDG_HandleTypeDef hiwdg;
uint8_t firmwareFlag = 0;



uint32_t flashProgramm(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
  uint32_t result = 3;
  FLASH_If_Init();
  if (FLASH_If_GetWriteProtectionStatus(*FlashAddress) == 0)
  {
    FLASH_If_DisableWriteProtection(*FlashAddress);
    //FLASH_If_Erase(*FlashAddress);
    //result = FLASH_If_Write(FlashAddress, Data , DataLength);
  } 
  else
  {
    //FLASH_If_Erase(*FlashAddress);
   //result = FLASH_If_Write(FlashAddress, Data , DataLength);
  }
  return(result);
}


	  
 static void FLASH_If_Init(void)
{ 
  FLASH_Unlock();  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}

// ??????? ??? ???????????????? ??????? ??????
 uint32_t FLASH_If_Erase(uint32_t StartSector)
{
  
  HAL_IWDG_Refresh(&hiwdg);
  uint32_t UserStartSector = FLASH_Sector_1, i = 0;
  UserStartSector = GetSector(StartSector); 
  for(i = UserStartSector; i <= FLASH_Sector_10; i += 8)
  {
    HAL_IWDG_Refresh(&hiwdg);
    if (FLASH_EraseSector(i,  VoltageRange_3) != FLASH_COMPLETE_OLD)
    {
      return (1);
    }	
  }
  
  return (0);
}



// ?????? ?????? ?? flash
 uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
  uint32_t i = 0;
  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
  {
    if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE_OLD)
    {
      // ???????? ??????????? ????????
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
      {
        // Flash ?? ????????????? SRAM
        LED2_RED(1);

        return(2);
      }
      *FlashAddress += 4;
    }
    else
    {
      // ?????? ??????
      LED2_RED(1);
      return (1);
    }
  }
  //LED1_RED(1);
  return (0);
}

// ????????? ???????? ?? ?? ?????? Flash ?????? ( 1 - ???)  
 static uint16_t FLASH_If_GetWriteProtectionStatus(uint32_t StartSector)
{
  uint32_t UserStartSector = FLASH_Sector_1;
  UserStartSector = GetSector(StartSector); 
  if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
  { 
    return 1;
  }
  else
  { 
    return 0;
  }
}

static uint32_t FLASH_If_DisableWriteProtection(uint32_t StartSector)
{
  __IO uint32_t UserStartSector = FLASH_Sector_1, UserWrpSectors = OB_WRP_Sector_1;
  UserStartSector = GetSector(StartSector);
  UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);
  FLASH_OB_Unlock();
  FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);
  if (FLASH_OB_Launch() != FLASH_COMPLETE_OLD)
  {
    return (2);
  }
  // ?????? ?????????
  return (1);
}

// ????????? ?????? flash ?? ??????  
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;
	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;  
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;  
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;  
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;  
	}
    return sector;
}