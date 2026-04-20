#include "def_at.h"
#include "def_link.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "map_ef.h"
#include "map_mbus.h"
#include "ozu_map.h"
#include "sec.h"
#include <inavr.h>
#include <iom1280.h>

extern unsigned int cnt_sec;

extern unsigned int cnt_supervosor;

extern unsigned char simka; // dobavka

extern unsigned char layer_PPP;

extern unsigned long int cnt_outcom, cnt_incom;
extern unsigned char time_cnt_ch_time;

unsigned int crc_m1(unsigned char *ka, unsigned int num, unsigned int crc);
void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                    unsigned int num, unsigned char flag, unsigned char znach);

void lock_it(void);

void fun_state_md(void);
extern void tx_rs485_1(void);
extern void tx_rs485_2(void);
extern void tx_rs232_2(void);
extern void tx_rs232_time(void);
extern void test_fram(void);
extern unsigned long int burst_ds_r(void);

unsigned char check_cts(void);

extern unsigned int func_crc_tii(void);
extern __no_init unsigned int crc_tii;
extern __no_init unsigned int cnt_tii[8];

extern unsigned char Fl_ch_sim;
;                                                   // dobavka
extern unsigned long int cnt_no_link, cnt_link_res; // dobavka

// extern unsigned char count_block_net;

extern struct {
  unsigned char clr_wdt : 1;
  unsigned char fl_out : 1;
  unsigned char from_timer0 : 1;
  unsigned char fl_main : 1;
  unsigned char from_timer3 : 1;
  unsigned char from_timer2 : 1;
  unsigned char from_adc : 1;
} fl_wdt;
unsigned char cnt_clr_wdt, cnt_flag_out;

extern unsigned char cnt_rst_lcp, vol_cnt_rst_lcp;
extern unsigned char cnt_rst_ipcp, vol_cnt_rst_ipcp;
extern unsigned char cnt_rst_pap, vol_cnt_rst_pap;
extern unsigned int cnt_lcp_tm_out, cnt_ipcp_tm_out;
extern unsigned int cnt_pap_tm_out;

extern struct {
  unsigned char over_buf : 1;
  unsigned char send_state : 1;
  unsigned char enable : 1;
} fl_appl_seq1;

extern struct {
  unsigned char ip : 1;
  unsigned char udp : 1;
  unsigned char num_self : 1;
  unsigned char cnt_reset;
} fl_rewrite;

extern struct // структура, описывающая объект передачи по PPP
{
  enum bool link_no;      // связи нет
  enum bool link_waits;   // ожидание квитка на контроль связи
  enum bool link;         // контроль связи
  enum bool nat;          // контроль nat
  unsigned int cnt_link;  // счетчик
  unsigned int cnt_nat;   // счетчик
  unsigned int vol_link;  // значение
  unsigned int vol_nat;   // //значение
  unsigned int vol_waits; // значение времени квитка ожидания контроля связи
  unsigned char cnt_try;  // счетчик попыток получить квиток
  unsigned char vol_try;  // rjkbxtcndj попыток получить квиток
  unsigned int vol_nat_r; // счетчик
} Control;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!структуры приложения
extern struct {
  unsigned char id;
  unsigned int p_in; // указатель свободной ячейки
  unsigned int p_out;
  unsigned int p_out_kv;
  unsigned int crc;
  unsigned int cnt_waits; // счетчик ожидания квитка
  unsigned char cnt_try;  // счетчик попыток получить квиток
  unsigned int l_data;    // длина посылаемых данных
  unsigned char state;
  unsigned char event;
  unsigned int tm_vzat;
  unsigned int vol_tm_vzat;
  enum bool cntr_cl;
  enum bool en_cntr_cl;
  unsigned int cnt_cntr_cl;
  unsigned int vol_cntr_cl;
} Appl_seq;

// unsigned char Appl_seq_des;

// unsigned char state_seq;

// unsigned char Appl_seq_buf[MAX_BUF_SEQ];

extern struct {
  unsigned char en_tx : 1;
  unsigned char kv_waits : 1;
  unsigned char en_povtor : 1;
} fl_appl_seq;

extern struct {
  unsigned char time_485_1;
  unsigned int client_485_1;
  unsigned char start_485_1;
  unsigned char time_485_2;
  unsigned int client_485_2;
  unsigned char start_485_2;
  unsigned char time_232_2;
  unsigned int client_232_2;
  unsigned char start_232_2;
} Life;

extern unsigned char proverka;
extern unsigned char fl_at_mom_232;

extern enum t_event_modem event_modem;

extern unsigned char buf_tx_232[VOL_TX_PPP];
extern unsigned int count_tx_ppp, vol_tx_ppp;

extern unsigned char Regim;
extern unsigned char cnt_stm_tm1, cnt_stm_tm2;
extern unsigned int cnt_key_off;

extern void sending_at_pac(void);

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

extern enum bool command_AT;
extern enum bool fl_cts_232_ignor;
extern unsigned int cnt_cts_off;

extern struct {
  unsigned char on : 1;
} fl_cts_232;

extern struct {
  unsigned char ok : 1;     // требуемый ответ
  unsigned char err : 1;    // ошибочный, нетребуемый ответ
  unsigned char tm_out : 1; // отсутствие ответа
  unsigned char tx_en : 1;  // послать комманду
  unsigned char rx_en : 1;  // принимать ответы
  unsigned char rx_rec : 1; // принят ответ
  // unsigned char tm_out_en:1;//разрешение анализа по превышению времени
  // ожидания ответа
} fl_at_com;

#pragma dataseg = MY_SEG
__no_init unsigned char Rs485_1_buf_rx_tx[MAX_BUF_RS485_1];
__no_init unsigned char Rs485_2_buf_rx_tx[MAX_BUF_RS485_2];
__no_init unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];
#pragma dataseg = default

struct // структура описывающая работу порта "RS485_1"
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

struct {
  unsigned char busy : 1;
  unsigned char rec : 1;
  unsigned char tm_out : 1;
  unsigned char tx : 1;
  unsigned char over : 1;
  unsigned char buffed : 1;
} fl_485_1, fl_485_2, fl_232_2;

extern struct {
  unsigned char data[VOL_RX_PPP]; // сам буфер
  unsigned int ln_data;           // длина данных
  enum bool rec;                  // пакет принят
  enum bool busy;                 // буфер занят
  enum bool check_busy;           // проверка занят ли буфер
} Buf1_rx_ppp, Buf2_rx_ppp;       //

struct {
  unsigned char fl_7e : 1;    //
  unsigned char fl_7d : 1;    //
  unsigned char switcher : 1; // переключатель буферов
} fl_rx_ppp;

///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!для РРР
extern struct {
  unsigned char act_lcp_end : 1;
  unsigned char lcp_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
} fl_lcp;

extern struct {
  unsigned char act_ipcp_end : 1;
  unsigned char ipcp_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
} fl_ipcp;

extern struct {
  unsigned char act_ip_end : 1;
  unsigned char ip_tm_cntr_en : 1;
  unsigned char ip_tm_nat_en : 1;
} fl_ip;

extern struct {
  unsigned char act_pap_end : 1;
  unsigned char pap_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
  unsigned char up : 1;
} fl_pap;

extern struct {
  unsigned char add_byte : 1;
} fl_reg3;
// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

extern unsigned long int summa_temperatura;
extern unsigned int temperatura;
extern unsigned int count_summa_temperatura;

unsigned int modbus_mem1[SEG1];
extern unsigned long int arr_tii_32[8];

struct {

  unsigned char tek_pin : 1;
  unsigned char old_pin : 1;
  unsigned char fl_ch_tc : 1;
  unsigned char real_tc;
  unsigned int count_tc;
  unsigned char type;
  unsigned char cnt_block_tc;
} struct_tcc1, struct_tcc2;

unsigned long int summa_adc[4];
unsigned char count_summa_adc[4];

/// 11111111111111111111111111111 функции

/*
 struct struct_ts
{

  unsigned char tek_pin :1;
  unsigned char old_pin :1;
  unsigned char fl_ch_tc :1;
//  unsigned char real_tc:1;
  unsigned char count_tc;
};
*/

struct struct_ts str_tc2, str_tc3, str_tc4, str_tc5, str_tc6, str_tc7, str_tc8;
/// 11111111111111111111111111111 функции

unsigned char cnt_tu1, cnt_tu2;

// void opros_ts(unsigned char num, unsigned char port, unsigned char pin,
// struct struct_ts *ts, unsigned int *tii)
void opros_ts(unsigned char num, unsigned char port, unsigned char pin,
              struct struct_ts *ts)

{
  if (port & pin)
    ts->tek_pin = 0;
  else
    ts->tek_pin = 1;
  if (ts->fl_ch_tc == 1) {
    ts->count_tc--;
    if (ts->count_tc == 0) {
      ts->fl_ch_tc = 0;
      //  ts->real_tc=0x01 & (~ts->tek_pin);
      // *tii++;
      //  modbus_mem1[num]=*tii;
      modbus_mem1[AD_TII1 + num]++;
      arr_tii_32[num]++;
      modbus_mem1[AD_TS] =
          (modbus_mem1[AD_TS] & (~(0x01 << num))) | (ts->tek_pin << num);
    }
  } else {
    if (ts->tek_pin != ts->old_pin) {
      ts->fl_ch_tc = 1;
      ts->count_tc = 30;
    }
  }
  ts->old_pin = ts->tek_pin;
}

void sending_ppp_proverka(void) {
  if (count_tx_ppp >= 20) {
    // UCSR0B=UCSR0B & ~TXEN;!!!!
    // UCSR0B=UCSR0B & ~TXCIE; !!!!
    S2_OFF;
    return;
  }

  UDR0 = buf_tx_232[count_tx_ppp];
  if (Regim == RG_DEBAG)
    UDR3 = buf_tx_232[count_tx_ppp];
  count_tx_ppp++;
}

void sending_ppp_pac(void) {
  cnt_supervosor = 0;

  if (count_tx_ppp >= (vol_tx_ppp - 1)) {

    if (fl_lcp.act_lcp_end == 0)
      fl_lcp.act_lcp_end = 1;
    if (fl_pap.act_pap_end == 0)
      fl_pap.act_pap_end = 1;
    if (fl_ipcp.act_ipcp_end == 0)
      fl_ipcp.act_ipcp_end = 1;
    if (fl_ip.act_ip_end == 0)
      fl_ip.act_ip_end = 1;
    fl_reg3.add_byte = 0;
    //  UCSR0B=UCSR0B & ~TXEN;!!!!!!!
    //  UCSR0B=UCSR0B & ~TXCIE; !!!!
    S2_OFF;
    return;
  }

  if (fl_reg3.add_byte == 1) {
    fl_reg3.add_byte = 0;
    if (buf_tx_232[count_tx_ppp] == 0x7e) {
      UDR0 = 0x5e;
      if (Regim == RG_DEBAG)
        UDR3 = 0x5e;
      return;
    }
    if (buf_tx_232[count_tx_ppp] == 0x7d) {
      UDR0 = 0x5d;
      if (Regim == RG_DEBAG)
        UDR3 = 0x5d;
      return;
    }
    UDR0 = buf_tx_232[count_tx_ppp] + 0x20;
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[count_tx_ppp] + 0x20;
    return;
  }

  count_tx_ppp++;

  if (count_tx_ppp == (vol_tx_ppp - 1))
    goto exit_tx;

  if ((buf_tx_232[count_tx_ppp] == 0x7e) ||
      (buf_tx_232[count_tx_ppp] == 0x7d)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    if (Regim == RG_DEBAG)
      UDR3 = 0x7d;
    return;
  }

  if ((buf_tx_232[count_tx_ppp] == 17) || (buf_tx_232[count_tx_ppp] == 19)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    if (Regim == RG_DEBAG)
      UDR3 = 0x7d;
    return;
  }

  if ((buf_tx_232[count_tx_ppp] < 0x20) && (fl_ip.act_ip_end == 1)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    if (Regim == RG_DEBAG)
      UDR3 = 0x7d;
    return;
  }

exit_tx:
  UDR0 = buf_tx_232[count_tx_ppp];
  if (Regim == RG_DEBAG)
    UDR3 = buf_tx_232[count_tx_ppp];
}

void recive_buf1(unsigned char temp) {

  // if(Regim==RG_DEBAG)UDR1=temp;  //!!!!!!!! poka

  if (fl_rx_ppp.fl_7e == 1) {

    if (Buf1_rx_ppp.ln_data >=
        VOL_RX_PPP) // проверка на переполнение первого буфера
    {
      Buf1_rx_ppp.ln_data = 0;
      fl_rx_ppp.fl_7e = 0;
      fl_rx_ppp.fl_7d = 0;
      S2_OFF;

      // S3_GR;//!!!!proverka
      return;
    }

    // //////////укладка
    if (temp == 0x7d) {
      fl_rx_ppp.fl_7d = 1;
      return;
    } else {
      if (fl_rx_ppp.fl_7d == 1) {

        Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = temp ^ 0x20;
        Buf1_rx_ppp.ln_data++;
        fl_rx_ppp.fl_7d = 0;
        /*
        if((temp>=0x20)&&(temp<0x40)){Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data]=temp-0x20;Buf1_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        if(temp==0x5e){Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data]=0x7e;Buf1_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        if(temp==0x5d){Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data]=0x7d;Buf1_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        Buf1_rx_ppp.ln_data=0;
        fl_rx_ppp.fl_7e=0;
        fl_rx_ppp.fl_7d=0;
        S3_RD;//!!!!proverka
        S2_OFF;
        */

        return;
      } else {
        Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = temp;
        Buf1_rx_ppp.ln_data++;

        if (temp == 0x7e) // нет переполнения
        {
          if (Buf1_rx_ppp.ln_data <= 2) // если подряд два 0x7e
          {
            //  if(ppp_packet.rcvd==1)???????????????
            /*
            if((layer_PPP==LAYER_IP)&&(Buf1_rx_ppp.ln_data==2))
            {
             S4_RD;//proverka!!!!!
             while(1)__watchdog_reset();//proverka!!!!!
            }
            */

            fl_rx_ppp.fl_7e = 1;
            Buf1_rx_ppp.data[0] = temp;
            Buf1_rx_ppp.ln_data = 1;
            return;
          }

          S2_OFF;
          fl_rx_ppp.fl_7e = 0;
          fl_rx_ppp.switcher = 1;
          Buf1_rx_ppp.rec = TRUE;
          Buf1_rx_ppp.busy = TRUE;
          if (Buf2_rx_ppp.busy == TRUE) {

            Buf2_rx_ppp.check_busy = TRUE;
            UCSR0B = UCSR0B & ~RXEN;
            UCSR0B = UCSR0B & ~RXCIE;
          }
          return;
        }
      }
    }

    return;
  } // к пекет бегин
  else {
    if (temp == 0x7e) {
      //  if(ppp_packet.rcvd==1)???????????????
      fl_rx_ppp.fl_7e = 1;
      Buf1_rx_ppp.data[0] = temp;
      Buf1_rx_ppp.ln_data = 1;
      S2_GR;
    }
  }
}

void recive_buf2(unsigned char temp) {

  // if(Regim==RG_DEBAG)UDR1=temp;  //!!!!!!!! poka

  if (fl_rx_ppp.fl_7e == 1) {

    if (Buf2_rx_ppp.ln_data >=
        VOL_RX_PPP) // проверка на переполнение первого буфера
    {
      Buf2_rx_ppp.ln_data = 0;
      fl_rx_ppp.fl_7e = 0;
      fl_rx_ppp.fl_7d = 0;
      S2_OFF;
      //     S3_GR;//!!!!proverka
      return;
    }

    // //////////укладка
    if (temp == 0x7d) {
      fl_rx_ppp.fl_7d = 1;
      return;
    } else {
      if (fl_rx_ppp.fl_7d == 1) {
        Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = temp ^ 0x20;
        Buf2_rx_ppp.ln_data++;
        fl_rx_ppp.fl_7d = 0;

        /*
        if((temp>=0x20)&&(temp<0x40)){Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data]=temp-0x20;Buf2_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        if(temp==0x5e){Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data]=0x7e;Buf2_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        if(temp==0x5d){Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data]=0x7d;Buf2_rx_ppp.ln_data++;fl_rx_ppp.fl_7d=0;return;}
        Buf2_rx_ppp.ln_data=0;
        fl_rx_ppp.fl_7e=0;
        fl_rx_ppp.fl_7d=0;
        S3_RD;//!!!!proverka
        S2_OFF;
        */
        return;
      } else {
        Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = temp;
        Buf2_rx_ppp.ln_data++;

        if (temp == 0x7e) // нет переполнения
        {
          if (Buf2_rx_ppp.ln_data <= 2) // если подряд два 0x7e
          {
            /*
             if((layer_PPP==LAYER_IP)&&(Buf2_rx_ppp.ln_data==2))
            {
             S5_RD;//proverka!!!!!
             while(1)__watchdog_reset();//proverka!!!!!
            }
            */

            //  if(ppp_packet.rcvd==1)???????????????
            fl_rx_ppp.fl_7e = 1;
            Buf2_rx_ppp.data[0] = temp;
            Buf2_rx_ppp.ln_data = 1;
            return;
          }

          S2_OFF;
          fl_rx_ppp.fl_7e = 0;
          fl_rx_ppp.switcher = 0;
          Buf2_rx_ppp.rec = TRUE;
          Buf2_rx_ppp.busy = TRUE;
          if (Buf1_rx_ppp.busy == TRUE) {

            Buf1_rx_ppp.check_busy = TRUE;
            UCSR0B = UCSR0B & ~RXEN;
            UCSR0B = UCSR0B & ~RXCIE;
          }
          return;
        }
      }
    }

    return;
  } // к пекет бегин
  else {
    if (temp == 0x7e) {
      //  if(ppp_packet.rcvd==1)???????????????
      fl_rx_ppp.fl_7e = 1;
      Buf2_rx_ppp.data[0] = temp;
      Buf2_rx_ppp.ln_data = 1;
      S2_GR;
    }
  }
}
// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

//////////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = TIMER0_COMPA_vect
__interrupt void TIMER0_COMPA_interrupt(void) {

  fl_wdt.from_timer0 = 1;

  // __enable_interrupt();

  if (PING & TCC1)
    struct_tcc1.tek_pin = 1;
  else
    struct_tcc1.tek_pin = 0;

  if (struct_tcc1.fl_ch_tc == 1) {
    struct_tcc1.count_tc--;
    if (struct_tcc1.count_tc == 0) {
      struct_tcc1.fl_ch_tc = 0;
      struct_tcc1.real_tc = 0x01 & (~struct_tcc1.tek_pin);
      modbus_mem1[AD_TS] =
          (modbus_mem1[AD_TS] & (~(0x01 << 14))) | (struct_tcc1.real_tc << 14);
    }
  } else {
    if (struct_tcc1.tek_pin != struct_tcc1.old_pin) {
      struct_tcc1.fl_ch_tc = 1;
      struct_tcc1.count_tc = 882;
    }
  }
  struct_tcc1.old_pin = struct_tcc1.tek_pin;

  if (PINA & TCC2)
    struct_tcc2.tek_pin = 1;
  else
    struct_tcc2.tek_pin = 0;

  if (struct_tcc2.fl_ch_tc == 1) {
    struct_tcc2.count_tc--;
    if (struct_tcc2.count_tc == 0) {
      struct_tcc2.fl_ch_tc = 0;
      struct_tcc2.real_tc = 0x01 & (~struct_tcc2.tek_pin);
      modbus_mem1[AD_TS] =
          (modbus_mem1[AD_TS] & (~(0x01 << 15))) | (struct_tcc2.real_tc << 15);
    }
  } else {
    if (struct_tcc2.tek_pin != struct_tcc2.old_pin) {
      struct_tcc2.fl_ch_tc = 1;
      struct_tcc2.count_tc = 882;
    }
  }
  struct_tcc2.old_pin = struct_tcc2.tek_pin;

  opros_ts(1, PINK, IO2, &str_tc2);
  opros_ts(2, PINA, IO3, &str_tc3);
  opros_ts(3, PINK, IO8, &str_tc4);
  opros_ts(4, PINJ, IO4, &str_tc5);
  opros_ts(5, PINA, IO5, &str_tc6);
  opros_ts(6, PINK, IO6, &str_tc7);
  opros_ts(7, PINA, IO7, &str_tc8);
}
//////////////////////////////////////////////1/16000000///////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = TIMER2_COMPA_vect // 1 миллисекунда
__interrupt void TIMER2_COMPA_interrupt(void) {

  unsigned char temp;

  fl_wdt.from_timer2 = 1;

  cnt_clr_wdt++;
  if (cnt_clr_wdt >= 250) {
    cnt_clr_wdt = 0;
    fl_wdt.clr_wdt = 1;
    cnt_flag_out++;
    if (cnt_flag_out >= 50) {
      cnt_flag_out = 0;
      fl_wdt.fl_out = 1;
    }
  }

  if (cnt_sec < DEF_CNT_SEC)
    cnt_sec++;

  if (fl_cts_232.on == 1) {
    if (fl_cts_232_ignor == TRUE) {
      fl_cts_232.on = 0;
      if (count_tx_ppp == 0) {
        S2_RD;
        UDR0 = buf_tx_232[0];
        if (Regim == RG_DEBAG)
          UDR3 = buf_tx_232[0];
      } else
        sending_ppp_pac();
      ///     return;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    // if ((PINE & CTS0)==0)
    if (PINE & CTS0) {
      cnt_cts_off = 0;
      fl_cts_232.on = 0;
      if (count_tx_ppp == 0) {
        S2_RD;
        UDR0 = buf_tx_232[0];
        if (Regim == RG_DEBAG)
          UDR3 = buf_tx_232[0];
      } else
        sending_ppp_pac();
    } else {
      cnt_cts_off++;
      if (cnt_cts_off >= 10000) {
        cnt_cts_off = 0;
        event_modem = EVM_CTS_ERR;
      }
    }
  }

  fun_state_md();

  if (Buf1_rx_ppp.check_busy == TRUE) {
    if (Buf1_rx_ppp.busy == FALSE) {
      Buf1_rx_ppp.check_busy = FALSE;
      temp = UDR0;
      UCSR0B = UCSR0B | RXEN;
      UCSR0B = UCSR0B | RXCIE;
    }
  }

  if (Buf2_rx_ppp.check_busy == TRUE) {
    if (Buf2_rx_ppp.busy == FALSE) {
      Buf2_rx_ppp.check_busy = FALSE;
      temp = UDR0;
      UCSR0B = UCSR0B | RXEN;
      UCSR0B = UCSR0B | RXCIE;
    }
  }

  if (cnt_key_off == 0)
    goto next_step00;
  cnt_key_off--;
  if (cnt_key_off == 0)
    CLR_PWRK;

next_step00:

  if (At_com.cnt_tm_out == 0)
    goto next_step0;
  {
    At_com.cnt_tm_out--;
    if (At_com.cnt_tm_out == 0) {
      UCSR0B = UCSR0B & ~RXEN;
      UCSR0B = UCSR0B & ~RXCIE;
      At_com.cnt_rx_out = 0;
      fl_at_com.tm_out = 1;
      fl_at_com.rx_rec = 0;
      fl_at_com.rx_en = 0;
      fl_at_com.err = 0;
      fl_at_com.ok = 0;
    }
  }

next_step0:

  if (At_com.cnt_rx_out == 0)
    goto next_step1;
  At_com.cnt_rx_out--;
  if (At_com.cnt_rx_out == 0) {
    UCSR0B = UCSR0B & ~RXEN;
    UCSR0B = UCSR0B & ~RXCIE;
    fl_at_com.rx_rec = 1;
    S2_OFF;
  } // счетчик межбайтовый промежуток
next_step1:

  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
  // обработка по порту RS-485 №1
  if (Rs485_1.cnt_tm_pre_tx == 0)
    goto nxt_step2;
  Rs485_1.cnt_tm_pre_tx--;
  if (Rs485_1.cnt_tm_pre_tx == 0)
    UCSR3B = UCSR3B | UDRIE; // enable transmit 485_1

nxt_step2:

  if (Rs485_1.cnt_tm_out == 0)
    goto nxt_step3;
  Rs485_1.cnt_tm_out--;
  if (Rs485_1.cnt_tm_out == 0) {
    S3_OFF;
    UCSR3B = UCSR3B & ~(RXEN | RXCIE);
    Rs485_1.cnt_bt_rx_tx = 0;
    Rs485_1.cnt_tm_rx_out = 0;
    fl_485_1.rec = 0;
    fl_485_1.tm_out = 1;
  }
nxt_step3:

  if (Rs485_1.cnt_tm_rx_out == 0)
    goto nxt_step4;
  Rs485_1.cnt_tm_rx_out--;
  if (Rs485_1.cnt_tm_rx_out == 0) {
    S3_OFF;
    fl_485_1.rec = 1;
    Rs485_1.cnt_tm_out = 0;
    fl_485_1.tm_out = 0;
    UCSR3B = UCSR3B & ~(RXEN | RXCIE);
  }
nxt_step4:
  if (Rs485_1.cnt_tm_tx_out == 0)
    goto lable_tm21;
  Rs485_1.cnt_tm_tx_out--;
  if (Rs485_1.cnt_tm_tx_out == 1) {
    CLR_RTS1;
  } /*togle to receiv mode*/
  if (Rs485_1.cnt_tm_tx_out == 0) {
    CLR_RTS1;
    S3_OFF;
    Rs485_1.cnt_tm_out = Rs485_1.vol_tm_out;
    UCSR3B = UCSR3B | RXEN | RXCIE;
  } // togle to receiv mode ;

  // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

lable_tm21:
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
  // обработка по порту RS-485 №2
  if (Rs485_2.cnt_tm_pre_tx == 0)
    goto next_step2;
  Rs485_2.cnt_tm_pre_tx--;
  if (Rs485_2.cnt_tm_pre_tx == 0)
    UCSR1B = UCSR1B | UDRIE; // enable transmit 485_1

next_step2:

  if (Rs485_2.cnt_tm_out == 0)
    goto next_step3;
  Rs485_2.cnt_tm_out--;
  if (Rs485_2.cnt_tm_out == 0) {
    S4_OFF;
    UCSR1B = UCSR1B & ~(RXEN | RXCIE);
    Rs485_2.cnt_bt_rx_tx = 0;
    Rs485_2.cnt_tm_rx_out = 0;
    fl_485_2.rec = 0;
    fl_485_2.tm_out = 1;
  }
next_step3:

  if (Rs485_2.cnt_tm_rx_out == 0)
    goto next_step4;
  Rs485_2.cnt_tm_rx_out--;
  if (Rs485_2.cnt_tm_rx_out == 0) {
    S4_OFF;
    fl_485_2.rec = 1;
    Rs485_2.cnt_tm_out = 0;
    fl_485_2.tm_out = 0;
    UCSR1B = UCSR1B & ~(RXEN | RXCIE);
  }
next_step4:
  if (Rs485_2.cnt_tm_tx_out == 0)
    goto lable_tm22;
  Rs485_2.cnt_tm_tx_out--;
  if (Rs485_2.cnt_tm_tx_out == 1) {
    CLR_RTS3;
  } /*togle to receiv mode*/
  if (Rs485_2.cnt_tm_tx_out == 0) {
    CLR_RTS3;
    S4_OFF;
    Rs485_2.cnt_tm_out = Rs485_2.vol_tm_out;
    UCSR1B = UCSR1B | RXEN | RXCIE;
  } // togle to receiv mode ;

  // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

lable_tm22:
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
  // обработка по порту RS-232 №2
  if (Rs232_2.cnt_tm_pre_tx == 0)
    goto nt_step2;
  Rs232_2.cnt_tm_pre_tx--;
  if (Rs232_2.cnt_tm_pre_tx == 0)
    UCSR2B = UCSR2B | UDRIE; // enable transmit 232_1

nt_step2:

  if (Rs232_2.cnt_tm_out == 0)
    goto nt_step3;
  Rs232_2.cnt_tm_out--;
  if (Rs232_2.cnt_tm_out == 0) {

    S5_OFF;
    UCSR2B = UCSR2B & ~(RXEN | RXCIE);
    Rs232_2.cnt_bt_rx_tx = 0;
    Rs232_2.cnt_tm_rx_out = 0;
    fl_232_2.rec = 0;
    fl_232_2.tm_out = 1;
  }
nt_step3:

  if (Rs232_2.cnt_tm_rx_out == 0)
    goto nt_step4;
  Rs232_2.cnt_tm_rx_out--;
  if (Rs232_2.cnt_tm_rx_out == 0) {
    S5_OFF;
    fl_232_2.rec = 1;
    Rs232_2.cnt_tm_out = 0;
    fl_232_2.tm_out = 0;
    UCSR2B = UCSR2B & ~(RXEN | RXCIE);
  }
nt_step4:
  if (Rs232_2.cnt_tm_tx_out == 0)
    goto lable_tm23;
  Rs232_2.cnt_tm_tx_out--;
  if (Rs232_2.cnt_tm_tx_out == 1) {
    CLR_RTS2;
  } /*togle to receiv mode*/
  if (Rs232_2.cnt_tm_tx_out == 0) {
    CLR_RTS2;
    S5_OFF;
    Rs232_2.cnt_tm_out = Rs232_2.vol_tm_out;
    UCSR2B = UCSR2B | RXEN | RXCIE;
  } // togle to receiv mode ;

  // EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

lable_tm23:
  /// таймер рестарта LCP
  if (fl_lcp.lcp_tm_out_en == 1) {
    if (cnt_lcp_tm_out == 0)
      goto lable_tm24;
    cnt_lcp_tm_out--;
    if (cnt_lcp_tm_out == 0)
      fl_lcp.t0_pl = 1;
    /*
       cnt_lcp_tm_out--;
       if(cnt_lcp_tm_out==0)
          {
          cnt_lcp_tm_out=VOL_LCP_TM_OUT;
          if(cnt_rst_lcp==0)
             {
              cnt_lcp_tm_out=vol_cnt_rst_lcp;
              fl_lcp.t0_mi=1;
             }
             else
               {
               cnt_rst_lcp--;
               fl_lcp.t0_pl=1;
               }
          }
    */
  }
lable_tm24:;
  //////////////////////////////////////////

  /// таймер рестарта PAP
  if (fl_pap.pap_tm_out_en == 1) {
    if (cnt_pap_tm_out == 0)
      goto lable_tm25;
    cnt_pap_tm_out--;
    if (cnt_pap_tm_out == 0)
      fl_pap.t0_pl = 1;
    /*
            if(cnt_pap_tm_out==0)
             {
             cnt_pap_tm_out=VOL_PAP_TM_OUT;
             if(cnt_rst_pap==0)
                {
                 cnt_pap_tm_out=vol_cnt_rst_pap;
                 fl_pap.t0_mi=1;
                }
                else
                  {
                  cnt_rst_pap--;
                  fl_pap.t0_pl=1;
                  }
             }
  */
  }
lable_tm25:;
  //////////////////////////////////////////

  /// таймер рестарта IPCP
  if (fl_ipcp.ipcp_tm_out_en == 1) {

    if (cnt_ipcp_tm_out == 0)
      goto lable_tm26;
    cnt_ipcp_tm_out--;
    if (cnt_ipcp_tm_out == 0)
      fl_ipcp.t0_pl = 1;
    /*
     cnt_ipcp_tm_out--;
     if(cnt_ipcp_tm_out==0)
        {
        cnt_ipcp_tm_out=VOL_IPCP_TM_OUT;
        if(cnt_rst_ipcp==0)
           {
            cnt_ipcp_tm_out=vol_cnt_rst_ipcp;
            fl_ipcp.t0_mi=1;
           }
           else
             {
             cnt_rst_ipcp--;
             fl_ipcp.t0_pl=1;
             }
        } */
  }

lable_tm26:;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = TIMER3_COMPA_vect
__interrupt void TIMER3_COMPA_interrupt(void) {

  fl_wdt.from_timer3 = 1;

  if (cnt_tu1 != 0) {
    cnt_tu1--;
    if (cnt_tu1 == 0) {
      TU1_OFF;
    }
  }

  if (cnt_tu2 != 0) {
    cnt_tu2--;
    if (cnt_tu2 == 0) {
      TU2_OFF;
    }
  }

  if (time_cnt_ch_time != 0)
    time_cnt_ch_time--;

  cnt_supervosor++;

  /*
  if((PINB & CTS_232)==CTS_232)
                  {
                  cnt_term++;
                  if(cnt_term>=4)lock_it();
                  }
                  else cnt_term=0;
 */

  // if(count_block_net!=0) count_block_net--;

  if (struct_tcc1.cnt_block_tc != 0)
    struct_tcc1.cnt_block_tc--;
  if (struct_tcc2.cnt_block_tc != 0)
    struct_tcc2.cnt_block_tc--;

  if (Appl_seq.cntr_cl == TRUE) {
    if (Appl_seq.cnt_cntr_cl != 0) {
      Appl_seq.cnt_cntr_cl--;
      if (Appl_seq.cnt_cntr_cl == 0)
        Appl_seq.en_cntr_cl = TRUE;
    }
  }

  if (cnt_stm_tm1 == 0)
    goto next_tm00;
  cnt_stm_tm1--;
  if (cnt_stm_tm1 == 0)
    event_modem = EVM_TM1;
next_tm00:

  if (cnt_stm_tm2 == 0)
    goto next_tm001;
  cnt_stm_tm2--;
  if (cnt_stm_tm2 == 0)
    lock_it();

next_tm001:

  if (fl_rewrite.cnt_reset == 0)
    goto next_tm01;
  fl_rewrite.cnt_reset--;
  if (fl_rewrite.cnt_reset == 0) {

    __disable_interrupt();

    crc_tii = func_crc_tii();

    *(unsigned long int *)&buf_tx_232[0] = burst_ds_r();
    *(unsigned long int *)&buf_tx_232[4] = cnt_outcom;
    *(unsigned long int *)&buf_tx_232[8] = cnt_incom;
    *(unsigned int *)&buf_tx_232[L_TRAF - 2] =
        crc_m1(&buf_tx_232[0], L_TRAF - 2, 0xffff);
    WrArrayToFlesh(A_TRAF, &buf_tx_232[0], L_TRAF, 0, 0);
    lock_it();
  }
next_tm01:

  /// таймер контроля IP
  if (Appl_seq.tm_vzat == 0)
    goto next_tm0;
  Appl_seq.tm_vzat--;
  if (Appl_seq.tm_vzat == 0)
    Appl_seq.event = EV_TM_OUT;
next_tm0:

  if (Appl_seq.cnt_waits == 0)
    goto next_tm1;
  Appl_seq.cnt_waits--;
  if (Appl_seq.cnt_waits == 0) {

    //   Appl_seq.cnt_try++;
    fl_appl_seq.kv_waits = 0;
    fl_appl_seq.en_povtor = 1;
    Appl_seq.l_data = 0;

    /*
    if(Appl_seq.cnt_try>=2)
     {
     Appl_seq.cnt_cntr_cl=0;
     Appl_seq.cntr_cl=TRUE;
     Appl_seq.en_cntr_cl=TRUE;
     Appl_seq.cnt_try=0;
     fl_appl_seq.en_povtor=0;
     }
     */
  }
next_tm1:

  if (fl_ip.ip_tm_cntr_en == 1) {

    if (Control.cnt_link == 0)
      goto next_tm2;

    Control.cnt_link--;
    if (Control.cnt_link == 0) {

      fl_appl_seq1.enable = 1;

      // Control.cnt_link=Control.vol_link;
      Control.link = TRUE;
      if (Control.link_waits == TRUE) {
        // if(Control.vol_try!=0)
        //   {
        Control.cnt_try++;
        if (Control.cnt_try >= Control.vol_try) {
          Control.link = FALSE;
          event_modem = EVM_MS_LMT;
          Control.link_no = TRUE; // признак связи нет.
        }
        // }
      }

      Control.link_waits = FALSE;
    }
  }

next_tm2:

  if (fl_ip.ip_tm_nat_en == 1) /// таймер контроля NAT
  {
    Control.cnt_nat--;
    if (Control.cnt_nat == 0) {
      if (simka == SIM_RES)
        Control.cnt_nat = Control.vol_nat_r;
      else
        Control.cnt_nat = Control.vol_nat;
      Control.nat = TRUE;
    }
  }

  //!!!!!!!!!!!!!dobavka
  if (cnt_no_link == 0)
    goto next_tm3;
  cnt_no_link--;
  if (cnt_no_link == 0)
    Fl_ch_sim = 1;
next_tm3:

  if (cnt_link_res == 0)
    goto next_tm4;
  cnt_link_res--;
  if (cnt_link_res == 0)
    Fl_ch_sim = 1;
next_tm4:

  // EEEEEEEEEEEEEE dobavka

  if (Life.time_485_1 == 0)
    goto next_tm5;
  Life.time_485_1--;
  if (Life.time_485_1 == 0)
    Life.start_485_1 = 0;
next_tm5:

  if (Life.time_485_2 == 0)
    goto next_tm6;
  Life.time_485_2--;
  if (Life.time_485_2 == 0)
    Life.start_485_2 = 0;

next_tm6:

  if (Life.time_232_2 == 0)
    goto next_tm7;
  Life.time_232_2--;
  if (Life.time_232_2 == 0)
    Life.start_232_2 = 0;
next_tm7:;
  //////////////////////////////////////////
}

#pragma vector = USART0_RX_vect

__interrupt void USART0_RX_interrupt(void)

{
  unsigned char data;
  data = UDR0;
  if ((Regim == MODEM_ONLY) || (Regim == MODEM_ONLY_R)) {
    UDR2 = data;
    return;
  } // dobavka
  if (Regim == RG_DEBAG)
    UDR1 = data;

  if (command_AT == TRUE) {
    if (fl_at_com.rx_en == 0)
      return; // dobavka 08.11.2007

    S2_GR;
    At_com.cnt_rx_out = At_com.vol_rx_out;
    At_com.cnt_tm_out = 0;
    if (At_com.cnt_rx < LN_BUF_AT) {
      At_com.buf[At_com.cnt_rx] = data;
      At_com.cnt_rx++;
    }
    return;
  }

  cnt_incom++;
  if (fl_rx_ppp.switcher == 0)
    recive_buf1(data);
  else
    recive_buf2(data);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma vector = USART0_TX_vect
__interrupt void USART0_TX_interrupt(void)

{

  if (command_AT == TRUE)
    sending_at_pac();

  else {

    if (fl_cts_232_ignor == TRUE) {
      sending_ppp_pac();
      return;
    }

    cnt_outcom++;

    if (check_cts() == 1)
      return;
    sending_ppp_pac();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#pragma vector=USART0_UDRE_vect

           __interrupt  void USART0_UDRE_interrupt(void)
           {
             sending_ppp_proverka();
           }


*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = USART1_RX_vect

__interrupt void USART1_RX_interrupt(void)

{
  unsigned char data;

  data = UDR1;
  S4_GR;
  Rs485_2.cnt_tm_rx_out = Rs485_2.vol_tm_rx_out;
  Rs485_2.cnt_tm_out = 0;
  if (Rs485_2.cnt_bt_rx_tx < MAX_BUF_RS485_2) {
    Rs485_2_buf_rx_tx[Rs485_2.cnt_bt_rx_tx] = data;
    Rs485_2.cnt_bt_rx_tx++;
  } else
    fl_485_2.over = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma vector = USART1_UDRE_vect

__interrupt void USART1_UDRE_interrupt(void)

{

  if (Rs485_2.cnt_bt_rx_tx == 0)
    goto end_tx1;

  UDR1 = *Rs485_2.p_data485++;
  Rs485_2.cnt_bt_rx_tx--;
  return;
end_tx1:
  UCSR1B = UCSR1B & ~UDRIE;
  Rs485_2.cnt_bt_rx_tx = 0;
  Rs485_2.cnt_tm_tx_out = Rs485_2.vol_tm_tx_out;
  if (Rs485_2.cnt_tm_tx_out == 0) {
    S4_OFF;
    Rs485_2.cnt_tm_out = Rs485_2.vol_tm_out;
    CLR_RTS3;                       // togle to receiv mode
    UCSR1B = UCSR1B | RXEN | RXCIE; // togle to receiv mode ;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = USART2_RX_vect

__interrupt void USART2_RX_interrupt(void)

{
  unsigned char data;
  data = UDR2;

  if (fl_at_mom_232 == 1) {
    if (vol_tx_ppp >= VOL_TX_PPP)
      vol_tx_ppp = 0;
    buf_tx_232[vol_tx_ppp] = data;
    vol_tx_ppp++;
    return;
  }

  if ((Regim == MODEM_ONLY) || (Regim == MODEM_ONLY_R)) {
    UDR0 = data;
    return;
  } // dobavka
  S5_GR;
  Rs232_2.cnt_tm_rx_out = Rs232_2.vol_tm_rx_out;
  Rs232_2.cnt_tm_out = 0;
  if (Rs232_2.cnt_bt_rx_tx < MAX_BUF_RS232_2) {
    Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx] = data;
    Rs232_2.cnt_bt_rx_tx++;
  } else
    fl_232_2.over = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma vector = USART2_UDRE_vect

__interrupt void USART2_UDRE_interrupt(void)

{

  if (Rs232_2.cnt_bt_rx_tx == 0)
    goto end_tx;

  UDR2 = *Rs232_2.p_data485++;
  Rs232_2.cnt_bt_rx_tx--;
  return;
end_tx:
  UCSR2B = UCSR2B & ~UDRIE;
  Rs232_2.cnt_bt_rx_tx = 0;
  Rs232_2.cnt_tm_tx_out = Rs232_2.vol_tm_tx_out;
  if (Rs232_2.cnt_tm_tx_out == 0) {
    S5_OFF;
    Rs232_2.cnt_tm_out = Rs232_2.vol_tm_out;
    CLR_RTS2;                       // togle to receiv mode
    UCSR2B = UCSR2B | RXEN | RXCIE; // togle to receiv mode ;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma vector = USART3_RX_vect

__interrupt void USART3_RX_interrupt(void)

{
  unsigned char data;

  data = UDR3;
  S3_GR;
  Rs485_1.cnt_tm_rx_out = Rs485_1.vol_tm_rx_out;
  Rs485_1.cnt_tm_out = 0;
  if (Rs485_1.cnt_bt_rx_tx < MAX_BUF_RS485_1) {
    Rs485_1_buf_rx_tx[Rs485_1.cnt_bt_rx_tx] = data;
    Rs485_1.cnt_bt_rx_tx++;
  } else
    fl_485_1.over = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma vector = USART3_UDRE_vect

__interrupt void USART3_UDRE_interrupt(void)

{

  if (Rs485_1.cnt_bt_rx_tx == 0)
    goto end_tx2;

  UDR3 = *Rs485_1.p_data485++;
  Rs485_1.cnt_bt_rx_tx--;
  return;
end_tx2:
  UCSR3B = UCSR3B & ~UDRIE;
  Rs485_1.cnt_bt_rx_tx = 0;
  Rs485_1.cnt_tm_tx_out = Rs485_1.vol_tm_tx_out;
  if (Rs485_1.cnt_tm_tx_out == 0) {
    S3_OFF;
    Rs485_1.cnt_tm_out = Rs485_1.vol_tm_out;
    CLR_RTS1;                       // togle to receiv mode
    UCSR3B = UCSR3B | RXEN | RXCIE; // togle to receiv mode ;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#pragma vector=ADC_vect
 __interrupt  void ADC_interrupt(void)
 {
  // temperatura=ADC;

   fl_wdt.from_adc=1;

  //  __enable_interrupt();

   count_summa_temperatura++;
   summa_temperatura=summa_temperatura+ADC;

       if(count_summa_temperatura>=1024)
          {
          count_summa_temperatura=0;
          temperatura=summa_temperatura>>10;
          summa_temperatura=0;
        }

       ADCSRA=ADCSRA | 0x47;
 }

*/

#pragma vector = ADC_vect
__interrupt void ADC_interrupt(void) {
  char i;
  // три замера каждый замер по каналу через 115 мсек
  //  1/11059200 *13*128*256*3=115 msec

  fl_wdt.from_adc = 1;

  i = ADMUX;
  i = i & 0x07;
  count_summa_adc[i]++;
  summa_adc[i] = summa_adc[i] + ADC;

  if (count_summa_adc[i] == 0) {
    if (i == 0) {
      modbus_mem1[AD_TEMP] = summa_adc[0] >> 8;
      temperatura = modbus_mem1[AD_TEMP];
    } else if (i == 1)
      modbus_mem1[AD_TIT1] = summa_adc[1] >> 8;
    else
      modbus_mem1[AD_TIT1 + 1] = summa_adc[3] >> 8;

    summa_adc[i] = 0;
  }

  if (i < 3) {

    i++;
    if (i == 2)
      i = 3;

    ADCSRB = 0x08;
    ADMUX = (ADMUX & (~0x07)) | i;
    ADCSRA = ADCSRA | 0x47;
  } else {
    ADCSRB = 0;
    ADMUX = 0x40;
    ADCSRA = ADCSRA | 0x47;
  }
}

#pragma vector = PCINT2_vect
__interrupt void PCINT2_interrupt(void) {
  if (PINK & IO1)
    modbus_mem1[AD_TS] = modbus_mem1[AD_TS] & (~0x01);
  else {
    modbus_mem1[AD_TS] = modbus_mem1[AD_TS] | 0x01;
    // cnt_tii[0]++;
    // modbus_mem1[0]=cnt_tii[0];
    modbus_mem1[AD_TII1]++;
    arr_tii_32[0]++;
  }
}
