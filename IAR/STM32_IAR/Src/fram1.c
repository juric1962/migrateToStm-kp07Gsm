#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_def.h"
#include "sram_rtc.h"
//#include "dfpin.h"
#include "map_mbus.h"
#include "ozu_map.h"
//#include <inavr.h>
//#include <iom2560.h>
extern unsigned long int unix;
extern unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];

char erri2c;
struct { // в двоичном коде
  char r_sec;
  char r_min;
  char r_hor;
  char r_day;
  char r_date;
  char r_month;
  char r_year;
  char r_control;
} real_time;

extern unsigned int modbus_mem1[SEG1];

extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure; 

uint32_t toUnix(RTC_DateTypeDef RTC_DateStructure, RTC_TimeTypeDef RTC_TimeStructure);

///////////////////////////////  функции для работы с последовательной флеш


uint32_t burst_ds_r(void) {
 
   // ïðè ðàáîòå ñ STM çàïîëíÿåì ñâîþ ñòðóêòóðó èç ñèñòåìíîãî âðåìåíè STM 
   // ïîëüçóåìñÿ åãî ôóíêöèÿìè
   
   real_time.r_sec=RTC_TimeStructure.Seconds;
   real_time.r_min=RTC_TimeStructure.Minutes;
   real_time.r_hor=RTC_TimeStructure.Hours;
   
   real_time.r_date=RTC_DateStructure.Date;
   real_time.r_month=RTC_DateStructure.Month;
   real_time.r_year=RTC_DateStructure.Year;
   
   unix=toUnix(RTC_DateStructure,RTC_TimeStructure);
   
   return(unix);
   
  
}



//////////EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
// работа с памятью


