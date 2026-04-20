#include "dfpin.h"
#include "dfproc.h"
#include "ozu_map.h"
#include <iom1280.h>

extern unsigned char Rs485_1_buf_rx_tx[MAX_BUF_RS485_1];
extern unsigned char Rs485_2_buf_rx_tx[MAX_BUF_RS485_2];
extern unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];
extern struct { // в двоичном коде
  char r_sec;
  char r_min;
  char r_hor;
  char r_day;
  char r_date;
  char r_month;
  char r_year;
  char r_control;
} real_time;

extern struct // структура описывающая работу порта "RS485_1"
{
  unsigned int cnt_bt_rx_tx;  // счетчик байтов на прием-передачу
  unsigned int cnt_tm_tx_out; // счетчик времени на удержание rts после передачи
  unsigned int cnt_tm_pre_tx; // счетчик времени на удержание rts перед передачи
  unsigned int
      vol_tm_tx_out; // предел счетчик времени на удержание rts после передачи
  unsigned int cnt_tm_rx_out; //  счетчик времени на определение конца приема
  unsigned int vol_tm_rx_out; //  предел времени на определение конца приема
  unsigned int cnt_tm_out;    // счетчик времени на прием
  unsigned int vol_tm_out;    // предел счетчика времени на прием
  unsigned char *p_data485;   // указатель на буфер передачи
} Rs485_1, Rs485_2, Rs232_2;

extern struct {
  unsigned char busy : 1;
  unsigned char rec : 1;
  unsigned char tm_out : 1;
  unsigned char tx : 1;
  unsigned char over : 1;
  unsigned char buffed : 1;
} fl_485_1, fl_485_2, fl_232_2;

extern unsigned long int burst(void);
extern unsigned long int burst_ds_r(void);
extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                             unsigned int num);
extern void
WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
               unsigned int num, unsigned char flag,
               unsigned char znach); // запись конфигурации кп во флеш

unsigned char proverka;

void tx_rs485_1(void) {
  unsigned int i;

  Rs485_1.cnt_bt_rx_tx = 10;
  for (i = 0; i < Rs485_1.cnt_bt_rx_tx; i++)
    Rs485_1_buf_rx_tx[i] = i;
  Rs485_1.p_data485 = &Rs485_1_buf_rx_tx[0];
  // здесь должна быть функция настройки порта по контексту.

  UBRR3H = R2400_H;
  UBRR3L = R2400_L;

  UCSR3B = UCSR3B & ~(RXCIE | RXEN);

  Rs485_1.cnt_tm_tx_out = 0;
  Rs485_1.vol_tm_tx_out = 15;

  Rs485_1.cnt_tm_pre_tx = 20; // 20 миллисекунд

  Rs485_1.cnt_tm_rx_out = 0;
  Rs485_1.vol_tm_rx_out = 10; // 5 миллисекунд

  Rs485_1.cnt_tm_out = 0;
  Rs485_1.vol_tm_out = 600; // 600 миллисекунд

  SET_RTS1; // rts на передачу
}

void tx_rs485_2(void) {
  unsigned int i;

  Rs485_2.cnt_bt_rx_tx = 10;
  for (i = 0; i < Rs485_2.cnt_bt_rx_tx; i++)
    Rs485_2_buf_rx_tx[i] = i;
  Rs485_2.p_data485 = &Rs485_2_buf_rx_tx[0];
  // здесь должна быть функция настройки порта по контексту.

  UBRR1H = R2400_H;
  UBRR1L = R2400_L;

  UCSR1B = UCSR1B & ~(RXCIE | RXEN);

  Rs485_2.cnt_tm_tx_out = 0;
  Rs485_2.vol_tm_tx_out = 15;

  Rs485_2.cnt_tm_pre_tx = 20; // 20 миллисекунд

  Rs485_2.cnt_tm_rx_out = 0;
  Rs485_2.vol_tm_rx_out = 10; // 5 миллисекунд

  Rs485_2.cnt_tm_out = 0;
  Rs485_2.vol_tm_out = 600; // 600 миллисекунд

  SET_RTS3; // rts на передачу
}

void tx_rs232_2(void) {
  unsigned int i;

  Rs232_2.cnt_bt_rx_tx = 10;
  for (i = 0; i < Rs232_2.cnt_bt_rx_tx; i++)
    Rs232_2_buf_rx_tx[i] = i;
  Rs232_2.p_data485 = &Rs232_2_buf_rx_tx[0];
  // здесь должна быть функция настройки порта по контексту.

  UBRR2H = R2400_H;
  UBRR2L = R2400_L;

  UCSR2B = UCSR2B & ~(RXCIE | RXEN);

  Rs232_2.cnt_tm_tx_out = 0;
  Rs232_2.vol_tm_tx_out = 15;

  Rs232_2.cnt_tm_pre_tx = 20; // 20 миллисекунд

  Rs232_2.cnt_tm_rx_out = 0;
  Rs232_2.vol_tm_rx_out = 10; // 5 миллисекунд

  Rs232_2.cnt_tm_out = 0;
  Rs232_2.vol_tm_out = 600; // 600 миллисекунд

  SET_RTS2; // rts на передачу
}

void tx_rs232_time(void) {
  // if (proverka!=1)return;
  // proverka=0;
  proverka++;

  UCSR2B = UCSR2B & ~(RXCIE | RXEN);

  //   burst_ds_r();
  burst();
  Rs232_2.cnt_bt_rx_tx = 13;
  // Rs232_2.cnt_bt_rx_tx=7;
  // Rs232_2_buf_rx_tx[1]=real_time.r_sec;
  // Rs232_2_buf_rx_tx[2]=real_time.r_min;
  // Rs232_2_buf_rx_tx[3]=real_time.r_hor;
  // Rs232_2_buf_rx_tx[4]=real_time.r_date;
  // Rs232_2_buf_rx_tx[5]=real_time.r_month;
  // Rs232_2_buf_rx_tx[6]=real_time.r_year;
  Rs232_2_buf_rx_tx[0] = proverka;
  Rs232_2.p_data485 = &Rs232_2_buf_rx_tx[0];
  // здесь должна быть функция настройки порта по контексту.

  UBRR2H = R2400_H;
  UBRR2L = R2400_L;

  Rs232_2.cnt_tm_tx_out = 0;
  Rs232_2.vol_tm_tx_out = 5;

  Rs232_2.cnt_tm_pre_tx = 5; // 20 миллисекунд

  Rs232_2.cnt_tm_rx_out = 0;
  Rs232_2.vol_tm_rx_out = 10; // 5 миллисекунд

  Rs232_2.cnt_tm_out = 0;
  Rs232_2.vol_tm_out = 10; // 600 миллисекунд

  SET_RTS2; // rts на передачу
}

void test_fram(void) {

  UCSR2B = UCSR2B & ~(RXCIE | RXEN);

  Rs232_2.cnt_bt_rx_tx = 10;
  RdFromFleshToArr(10, &Rs232_2_buf_rx_tx[0], Rs232_2.cnt_bt_rx_tx);
  Rs232_2.p_data485 = &Rs232_2_buf_rx_tx[0];
  // здесь должна быть функция настройки порта по контексту.

  UBRR2H = R2400_H;
  UBRR2L = R2400_L;

  Rs232_2.cnt_tm_tx_out = 0;
  Rs232_2.vol_tm_tx_out = 5;

  Rs232_2.cnt_tm_pre_tx = 5; // 20 миллисекунд

  Rs232_2.cnt_tm_rx_out = 0;
  Rs232_2.vol_tm_rx_out = 10; // 5 миллисекунд

  Rs232_2.cnt_tm_out = 0;
  Rs232_2.vol_tm_out = 10; // 600 миллисекунд

  SET_RTS2; // rts на передачу
}
