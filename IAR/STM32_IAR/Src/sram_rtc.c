#include "sram_rtc.h"
#include "stm32f4xx_hal_rtc.h"
#include "alarms.h"
#include "fram.h"
#include "main.h"
#include "pwm.h"
#include "dynaGram.h"
#include "archive.h"

//------------------------ RTC ------------------------------------------------- 
#define _TBIAS_DAYS		((70 * (uint32_t)365) + 17)
#define _TBIAS_SECS		(_TBIAS_DAYS * (uint32_t)86400)
#define	_TBIAS_YEAR		1900
#define MONTAB(year)		((((year) & 0x03) || ((year) == 0)) ? mos : lmos)
const int16_t	lmos[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
const int16_t	mos[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
#define	Daysto32(year, mon)	(((year - 1) / 4) + MONTAB(year)[mon])
//------------------------------------------------------------------------------
//-------------------- EVENT LOG -----------------------------------------------
volatile EVENT_RECORD_UNION *eventRecord;
volatile uint16_t *eventRecordPtr;
//------------------------------------------------------------------------------
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure; 
volatile RTC_UNIX_TIME_STRUCT RTC_UnixTime;
RTC_HandleTypeDef hrtc; 

volatile ACTIVE_POWER_STRUCT activePower;

void bkpSRAMInit(void)
{
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  HAL_PWREx_EnableBkUpReg();  
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();  
 
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    systemState.eventLogHardwareFault=3; // ������ ������������� �������� ���������� ���������� 
    eventLogWrite(18);      
    eventsCnt.RTC_init_err++;    
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    //Error_Handler();
  }

  __HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{  
  __HAL_RCC_RTC_DISABLE();  
  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();  
}

static void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  sdatestructure.Year = 0x00;
  sdatestructure.Month = RTC_MONTH_JANUARY ;
  sdatestructure.Date = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
  }

  stimestructure.Hours = 0x00;
  stimestructure.Minutes = 0x00;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
  }  
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}

void rtcInit(void)
{
  hrtc.Instance = RTC; 
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 0x7F;
  hrtc.Init.SynchPrediv = 0xFF;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    /* Initialization Error */
  }    

  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
  {
    /* Configure RTC Calendar */    
    systemState.eventLogHardwareFault=4; // ����� �������� RTC
    eventLogWrite(18);      
    eventsCnt.RTC_init_err++;
    RTC_CalendarConfig();
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      /* Turn on LED2: Power on reset occurred */     
    }
    /* Check if Pin Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED1: External reset occurred */
      
    }
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
}

void systemTimeUpdate(void)
{
  HAL_RTC_GetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN); 
  RTC_UnixTime.timeStamp=toUnix(RTC_DateStructure,RTC_TimeStructure);
  RTC_UnixTime.CRC16=modBusCRC16((uint8_t*)&RTC_UnixTime,4);
  bkpSRAM_Write(TIMESTAMP_SRAM_ADDR,(uint8_t*)&RTC_UnixTime,TIMESTAMP_SRAM_LENGTH);
}


uint32_t toUnix(RTC_DateTypeDef RTC_DateStructure, RTC_TimeTypeDef RTC_TimeStructure)
{	/* convert time structure to scalar time */
uint32_t	days;
uint32_t	secs;
int32_t		mon, year;

	/* Calculate number of days. */
	mon = RTC_DateStructure.Month-1;
	year = RTC_DateStructure.Year+2000 - _TBIAS_YEAR;
	days  = Daysto32(year, mon) - 1;
	days += 365 * year;
	days += RTC_DateStructure.Date;
	days -= _TBIAS_DAYS;

	/* Calculate number of seconds. */
	secs  = 3600 * RTC_TimeStructure.Hours;
	secs += 60 * RTC_TimeStructure.Minutes;
	secs += RTC_TimeStructure.Seconds;

	secs += (days * (uint32_t)86400);

	return (secs-946684800L);
}

void fromUnix(RTC_DateTypeDef *RTC_DateStructure, RTC_TimeTypeDef *RTC_TimeStructure, uint32_t secsarg)
{
uint32_t	secs;
int32_t		days;
int32_t		mon;
int32_t		year;
int32_t		i;
const int16_t*	pm;

  secs = secsarg+946684800L;
  days = _TBIAS_DAYS;

  /* days, hour, min, sec */
  days += secs / 86400;		secs = secs % 86400;
  RTC_TimeStructure->Hours = secs / 3600;	secs %= 3600;
  RTC_TimeStructure->Minutes = secs / 60;	RTC_TimeStructure->Seconds = secs % 60;

  /* determine year */
  for (year = days / 365; days < (i = Daysto32(year, 0) + 365*year); ) { --year; }
  days -= i;
  RTC_DateStructure->Year = year + _TBIAS_YEAR-2000;

  /* determine month */
  pm = MONTAB(year);
  for (mon = 12; days < pm[--mon]; );
  RTC_DateStructure->Month = mon + 1;
  RTC_DateStructure->Date = days - pm[mon] + 1;
}

uint8_t bkpSRAM_Write(uint16_t addr,uint8_t *src,uint16_t len)
{
  uint16_t i,j,n;
  n=addr+len;
  j=0;
  if(n<4097)
  {  
    for(i=addr;i<n;i++)
    {
      *(__IO uint8_t *)(BKPSRAM_BASE + i) = src[j];
      j++;
    }    
    return 0;
  }else return 1;
}

uint8_t bkpSRAM_Read(uint16_t addr,uint8_t *dst,uint16_t len)
{
  uint16_t i,j,n;
  n=addr+len;
  j=0;
  if(n<4097)
  {  
    for(i=addr;i<n;i++)
    {
      dst[j]=*(__IO uint8_t *)(BKPSRAM_BASE + i);
      j++;
    }    
    return 0;
  }else return 1;
}
