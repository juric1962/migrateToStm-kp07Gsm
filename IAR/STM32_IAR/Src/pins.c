#include "def_at.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "map_ad.h"
#include "map_mbus.h"
#include "ozu_map.h"
//#include <inavr.h>
//#include <iom2560.h>
#include <string.h>


void control_temperatura(void);
extern void long_delay(unsigned long int period);
char At_buf[10];
extern unsigned char Regim;
extern unsigned char buf_tx_232[VOL_TX_PPP];
unsigned char point_GSM;
char cnt_rx1;
void lock_it(void);
void s_port(unsigned char vv);
extern struct {
  char buf[LN_BUF_AT];
  unsigned char cnt_tx;
  unsigned char cnt_rx;
  unsigned char ln_buf;
  unsigned char list_com[VOL_LIST]; // перечень исполняемых команд
  unsigned char ln_list;            // длина перечня
  unsigned char cnt_com;            // счетчик команд
  unsigned int cnt_tm_out;          // счетчик времени ожидания ответа
  unsigned int vol_tm_out;          // предел времени ожидания ответа
  unsigned int cnt_rx_out;          // счетчик межбайтовый промежуток
  unsigned int vol_rx_out;          // предел межбайтового промежутка
} At_com;

extern struct {
  unsigned char ok : 1;        // требуемый ответ
  unsigned char err : 1;       // ошибочный, нетребуемый ответ
  unsigned char tm_out : 1;    // отсутствие ответа
  unsigned char tx_en : 1;     // послать комманду
  unsigned char rx_en : 1;     // принимать ответы
  unsigned char rx_rec : 1;    // принят ответ
  unsigned char greg_ereg : 1; // пакетник GPRS or LTE
  // unsigned char tm_out_en:1;//разрешение анализа по превышению времени
  // ожидания ответа
} fl_at_com;



void init_pins_ts_mod(void) { PIN_IN_TS1_8; }

void init_pins_mkd_mod(void) {
  PIN_IN_MKD1_5;
}




void send_to_sim900(char num) {
  char i;
  ///UCSR0B = UCSR0B | TXEN;
  ///UCSR0B = UCSR0B | RXEN;
  ///UCSR0B = UCSR0B & ~TXCIE;

  //   UCSR0B=UCSR0B & ~RXCIE;
  UCSR0B = UCSR0B | RXCIE; // разрешить прием от SIM модуля
  HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
  for (i = 0; i < num; i++) {

  ///wait_ready:
  ///  if ((UCSR0A & 0x20) == 0)
  ///    goto wait_ready;
  ///  UDR0 = buf_tx_232[i];

    HAL_UART_Transmit(&huart1, &buf_tx_232[i], 1, 10);
  }
  cnt_rx1 = 0;
  //   long_delay_wait_answer(500000); // УВЕЛИЧИЛ В 5 РАЗ
}

void fun_init_sim900(void) {
 /// UCSR0B = 0;
  // UBRR0H=R9600_H;
  // UBRR0L=R9600_L;

  // A7682
  //UBRR0H = R115200_H;
  //UBRR0L = R115200_L;

  SET_SIM1; // dobavka
  s_port('.');
  CLR_PWR; // выключить питание
  SET_PWRK;
  HAL_IWDG_Refresh(&iwdg);
  long_delay(10000000 / 4); //

  SET_PWR;
  CLR_PWRK;

 HAL_IWDG_Refresh(&iwdg);
  long_delay(10000000 / 4); //
  s_port('.');
  long_delay(10000000 / 4); //>3 sek
  SET_PWRK;
 HAL_IWDG_Refresh(&iwdg);
  long_delay(10000000); //>3 sek
  s_port('.');
  CLR_PWRK;


  CLR_DTR0;
  CLR_RTS0;
  HAL_IWDG_Refresh(&iwdg);  
  long_delay(1000000 / 4); //>1.5 sek
  s_port('.');
  SET_DTR0;
  SET_RTS0;

  ///UCSR0B = UCSR0B | TXEN;
  ///UCSR0B = UCSR0B | RXEN;
  ///UCSR0B = UCSR0B & ~TXCIE;


    HAL_UART_DeInit(&huart1);
    huart1.Init.BaudRate   = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.Parity     = UART_PARITY_NONE;
    huart1.Init.StopBits   = UART_STOPBITS_1;
    HAL_UART_Init(&huart1);



  /*
  UCSR0B=UCSR0B | RXCIE;  //

   UCSR2B=UCSR2B | RXEN;  //
   UCSR2B=UCSR2B | TXEN;  //
   UCSR2B=UCSR2B & ~TXCIE;  //
   UCSR2B=UCSR2B | RXCIE;  //

   Regim=MODEM_ONLY;  //
   __enable_interrupt();

   strcpy(At_buf, "AT+CBST=0,0,1");      // АВТО моду
   send_at();

     UBRR0H=R4800_H;
     UBRR0L=R4800_L;

  //  strcpy(At_buf, "atv0");
  //  send_at();
    */

  __enable_irq();
}


///////////////////////////////////////////////


//////////////////////////////////////////


void init_ports_debug(void) {
  // Инициализация порта №1 485 на передачу данных в терминал
  
  SET_RTS3;

  // Инициализация порта №2 RS-232 №2
  
  CLR_RTS2;
  

  // Инициализация порта №3 485 на передачу данных в терминал
  
  SET_RTS1;

  S3_YL;
  S4_YL;
  // S5_YL;
}
