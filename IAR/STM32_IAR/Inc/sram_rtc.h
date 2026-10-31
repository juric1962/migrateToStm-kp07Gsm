
#include "stm32f4xx_hal.h"


extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure; 
extern RTC_HandleTypeDef hrtc; 


void rtcInit(void);

uint32_t toUnix(RTC_DateTypeDef RTC_DateStructure, RTC_TimeTypeDef RTC_TimeStructure);
void fromUnix(RTC_DateTypeDef *RTC_DateStructure, RTC_TimeTypeDef *RTC_TimeStructure, uint32_t secsarg);

void bkpSRAMInit(void);
uint8_t bkpSRAM_Write(uint16_t addr,uint8_t *src,uint16_t len);
uint8_t bkpSRAM_Read(uint16_t addr,uint8_t *dst,uint16_t len);
