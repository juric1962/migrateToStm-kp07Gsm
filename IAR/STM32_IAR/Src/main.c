#include "stm32f4xx_hal.h"
#include "main.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "map_ef.h"
#include "ozu_map.h"
#include "tim.h"
#include "adc_dma_tim8.h"
#include "gpio_inputs.h"
#include "fm25w256_spi.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "uart4.h"
#include "sram_rtc.h"
//#include <inavr.h>
//#include <iom2560.h>
#include <stdlib.h>
#include <string.h>

extern IWDG_HandleTypeDef hiwdg;
void MX_IWDG_Init (void);
void SystemClock_Config(void);
extern unsigned char sel_modul;
void begin_kd_2(void);
uint32_t burst_ds_r(void);
extern void s_port(unsigned char ch);
extern void RdFromFleshToArr(uint16_t adres_flesh, unsigned char *adres_ozu,
                             uint16_t num);
unsigned char check_keys(void);

void begin_ts(void);
void load_par_first_flash(void);

void load_sel_modul(void);
void init_pins_mkd_mod(void);
void init_pins_ts_mod(void);

void begin_ts_mb(void);
void start_a(void);
void check_sim(void);

void init_pins(void);
void init_proc_state(void);
void delay(uint16_t period);
void long_delay(uint32_t period);
void delay_on_contr(void);
void init_proc_state(void);
void init_modem_only(void);
void start_time(void);
void WrArrayToFlesh(uint16_t adres_flesh, unsigned char *adres_ozu,
                    uint16_t num, unsigned char flag, unsigned char znach);
void tx_rs232_time(void);
void load_par_first(void);
void serch_point_log(void);
uint16_t crc_ozu(void);
void monitor_rst(void); 

extern unsigned char Regim;
extern unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];
extern uint16_t prov_ozu;

unsigned char check_memory_map(void);
void vosstan_memory(void);
void monitor_terminal(void); // проверка терминала
void load_par_from_memory(void);
void load_rw_pdp(void);
void load_rw_pdp_r(void);
void monitor_beg_state_seq(unsigned char zad);
void modem_engine(void);
void init_ports_debug(void);
void init_modem_call(void);

void Error_Handler(void);

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
// #pragma dataseg=MY_SEG
//__no_init unsigned char var3;
// #pragma dataseg=default

// #pragma location=0x600
//__no_init unsigned char var1 @ 0xe60;
// #pragma location=0x601
//__no_init unsigned char var2 @ 0xe62;

void main(void) {
  // unsigned char buf[11];
  unsigned char bufalo, I123;
   HAL_Init();
  SystemClock_Config();
  bkpSRAMInit();    
  rtcInit();
  FM25W256_SPI_Init();
  MX_UART1_Init();
  UART1_Receive_IT(&uart1_rx_byte, 1U);
  MX_UART2_Init();
  UART2_Receive_IT(&uart2_rx_byte, 1U);
  MX_UART3_Init();
  UART3_Receive_IT(&uart3_rx_byte, 1U);
  MX_UART4_Init();
   UART4_Receive_IT(&uart4_rx_byte, 1U);
  
  MX_GPIO_TCs_Init();
  MX_GPIO_TU_Init();
  MX_GPIO_EXTI6_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
   MX_TIM10_Init();

  /* Initialize DMA, ADC1 and TIM8 (TIM8 -> TRGO -> ADC1) */
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();
  /* Start ADC in DMA circular mode and start TIM8 to produce triggers */
  ADC1_DMA_Start();
  MX_IWDG_Init ();
  //HAL_IWDG_Start(&iwdg);
  // if(Regim !=MODEM_ONLY)
  

  /////////////////////////  Для отладки потом убрать
  //  UBRR2H=R9600_H;
  //  UBRR2L=R9600_L;
  ////////////////////////


  long_delay(1000000); // для внешней флешки
 // __watchdog_reset();
HAL_IWDG_Refresh(&hiwdg);
  

  
  load_par_first();
  if (check_memory_map() == 1)
    vosstan_memory();

  // s_port(dbbb++);  // dbg
  load_sel_modul();
  if (sel_modul >= 1)
    init_pins_mkd_mod();
  if (sel_modul == 0)
    init_pins_ts_mod();
 
  // s_port(dbbb++);  // dbg
  HAL_IWDG_Refresh(&hiwdg);

  monitor_terminal(); // проверка терминала  13 05 2015!!!!!!!!!!!
                      // s_port(dbbb++);  // dbg
  HAL_IWDG_Refresh(&hiwdg);

  if (check_keys() == 1) {
    while (1) {
      HAL_IWDG_Refresh(&hiwdg);
      S1_YL;
      S2_YL;
      S3_YL;
      S4_YL;
      S5_YL;
    }
  }
  // s_port(dbbb++);  // dbg

  // start_time();
  delay_on_contr();
  // s_port(dbbb++);  // dbg

  // var1=RG_DEBAG;
  // var2=RG_DEBAG;
  //  var3=RG_DEBAG;
  //   Regim=RG_DEBAG;

  // Regim=MODEM_ONLY; // проверка терминала  13 05 2015!!!!!!!!!!!

  //  if((Regim==MODEM_ONLY)||(Regim==MODEM_ONLY_R)){init_modem_only();}
  //  //dobavka 01 09 25

  //  if(Regim==MODEM_CALL) {init_modem_call();}     25 04 2013

  if (Regim == RG_DEBAG) {
    init_ports_debug();
  }

  // s_port(dbbb++);  // dbg

  load_rw_pdp();
  /// s_port(dbbb++);  // dbg
  load_rw_pdp_r();
  // s_port(dbbb++);  // dbg
  HAL_IWDG_Refresh(&hiwdg);
  serch_point_log();
  // s_port(dbbb++);  // dbg
  HAL_IWDG_Refresh(&hiwdg);

  load_par_first_flash();
  // s_port(dbbb++);  // dbg

  load_par_from_memory();
  // s_port(dbbb++);  // dbg
  monitor_rst();
  // s_port(dbbb++);  // dbg

  begin_ts();
  // s_port(dbbb++);  // dbg

  prov_ozu = crc_ozu();
  // s_port(dbbb++);  // dbg

  start_a();
  // s_port(dbbb++);  // dbg

  __enable_irq();

  long_delay(700000);
  long_delay(700000);

  // s_port(dbbb++);  // dbg

  monitor_beg_state_seq(0);

  // s_port(dbbb++);  // dbg
  begin_ts_mb();
  // s_port(dbbb++);  // dbg

  begin_kd_2();
  // s_port(dbbb++);  // dbg

  modem_engine();
}
