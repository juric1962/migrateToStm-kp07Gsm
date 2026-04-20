#include "def_at.h"
#include "def_conf.h"
#include "def_link.h"
#include "def_prot.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "map_ef.h"
#include "ozu_map.h"
#include <inavr.h>
#include <iom1280.h>

// адоптированный обрезанный стек PPP 14.09.07

#define C_GLUK1 0x55
#define C_GLUK2 0xaa

void transform_buf(unsigned char *p, unsigned int kol_byte,
                   unsigned int s_rand);

void boot(void);
void r_p_flash(unsigned int adress);
void reload_apl(void);

void send_info(char size, char __flash *p, unsigned char fl_id,
               unsigned char id);

__flash char evc_rld[] = {'E', 'V', 'C', ':', 'r', 'l', 'd'};

__flash char rec_ctrl_ch[] = {'R', 'E', 'C', ':', 'c', 't',
                              'r', 'l', ' ', 'c', 'h'};
__flash char rec_evc[] = {'R', 'E', 'C', ':', 'e', 'v', 'c'};

__flash char ans_in_ctrl_cl[] = {'A', 'N', 'S', '-', '<', 'c',
                                 't', 'r', 'l', ' ', 'c', 'l'};

/*
__flash char req_in_485_1[]={'R','E','Q','-','<','4','8','5','_','1'};
__flash char req_in_485_2[]={'R','E','Q','-','<','4','8','5','_','2'};
__flash char req_in_232[]={'R','E','Q','-','<','2','3','2'};
__flash char req_in_st_contr[]={'R','E','Q','-','<','s','t','
','c','o','n','t','r'};
__flash char req_in_config[]={'R','E','Q','-','<','c','o','n','f','i','g'};
__flash char req_in_prog[]={'R','E','Q','-','<','p','r','o','g'};
*/

__flash char ans_out_485_1[] = {'A', 'N', 'S', '-', '>',
                                '4', '8', '5', '_', '1'};
__flash char ans_out_485_2[] = {'A', 'N', 'S', '-', '>',
                                '4', '8', '5', '_', '2'};
__flash char ans_out_232[] = {'A', 'N', 'S', '-', '>', '2', '3', '2'};
__flash char ans_out_st_contr[] = {'A', 'N', 'S', '-', '>', 's', 't',
                                   ' ', 'c', 'o', 'n', 't', 'r'};
__flash char ans_out_config[] = {'A', 'N', 'S', '-', '>', 'c',
                                 'o', 'n', 'f', 'i', 'g'};
__flash char ans_out_prog[] = {'A', 'N', 'S', '-', '>', 'p', 'r', 'o', 'g'};

//__flash char ans_out_ping[]={'A','N','S','-','>','p','i','n','g'};

// #include "sec.h"

// enum t_version {VER1 = 1 ,VER2,VER3,VER4};
// enum t_type {ZAPR,OTV,SOOB,KVIT};
extern unsigned char simka; // dobavka

extern unsigned int num_self, num_seq_cl, port_udp;
extern unsigned char ip_ls[4];

extern unsigned long int cnt_no_link, vol_cnt_no_link; // dobavka

void delay(unsigned int period);
extern unsigned long int burst_ds_r(void);
unsigned int proc_config(unsigned char *buf_rx_ppp, unsigned char offset,
                         unsigned int count_rx_ppp);
unsigned char check_ln_conf(unsigned char *buf_rx_ppp, unsigned char offset,
                            unsigned int count_rx_ppp);
unsigned char check_cont_485_1(unsigned char *pointer);
unsigned char check_cont_485_2(unsigned char *pointer);

extern unsigned char Regim;
unsigned char proc_udp_data(unsigned char *buf_rx_ppp,
                            unsigned int count_rx_ppp);
unsigned int crc_m1(unsigned char *ka, unsigned int num, unsigned int crc);
unsigned int crc_ozu(void);
extern unsigned int prov_ozu;

void clr_cntr_nat(void);
void form_buf_tx_ppp(void);

unsigned char check_cts(void);
void clr_cntr_link(void);

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                      unsigned int num);
void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                    unsigned int num, unsigned char flag, unsigned char znach);

void send_err485(unsigned char port, unsigned char err, unsigned char id,
                 unsigned int dst);

extern unsigned char point_log_buf;

struct {
  unsigned char ip : 1;
  unsigned char udp : 1;
  unsigned char num_self : 1;
  unsigned char cnt_reset;
} fl_rewrite;

///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!для РРР
extern struct {
  unsigned char data[VOL_RX_PPP]; // сам буфер
  unsigned int ln_data;           // длина данных
  enum bool rec;                  // пакет принят
  enum bool busy;                 // буфер занят
  enum bool check_busy;           // проверка занят ли буфер
} Buf1_rx_ppp, Buf2_rx_ppp;       //

extern struct {

  unsigned char tek_pin : 1;
  unsigned char old_pin : 1;
  unsigned char fl_ch_tc : 1;
  unsigned char real_tc;
  unsigned int count_tc;
  unsigned char type;
  unsigned char cnt_block_tc;
} struct_tcc1, struct_tcc2;

extern struct {
  unsigned char tc : 1;
  unsigned char tc_old : 1;
} sv1, sv2;

extern enum t_event_modem event_modem;

extern unsigned char buf_tx_232[VOL_TX_PPP];
extern unsigned int count_tx_ppp, vol_tx_ppp;
extern unsigned char layer_PPP;

struct {
  unsigned char act_lcp_end : 1;
  unsigned char lcp_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
} fl_lcp;

struct {
  unsigned char act_ipcp_end : 1;
  unsigned char ipcp_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
} fl_ipcp;

struct {
  unsigned char act_ip_end : 1;
  unsigned char ip_tm_cntr_en : 1;
  unsigned char ip_tm_nat_en : 1;
} fl_ip;

struct {
  unsigned char act_pap_end : 1;
  unsigned char pap_tm_out_en : 1;
  unsigned char t0_pl : 1;
  unsigned char t0_mi : 1;
  unsigned char up : 1;
} fl_pap;

struct {
  unsigned char up : 1;
  unsigned char down : 1;
} fl_lcp1;

struct {
  unsigned char up : 1;
  unsigned char down : 1;
} fl_ipcp1;

struct {
  unsigned char nac_rej : 1; // reject - 1
} fl_lcp2;

struct {
  unsigned char nac_rej : 1; // reject - 1
} fl_ipcp2;

unsigned char pap_act_buf, PAP_state, PAP_event;
unsigned char LCP_state, LCP_event, LCP_act_buf[LCP_ACT_VOL];
unsigned char IPCP_state, IPCP_event, IPCP_act_buf[IPCP_ACT_VOL];
unsigned char uk_in_act_LCP, uk_out_act_LCP, uk_in_act_IPCP, uk_out_act_IPCP;

unsigned char cnt_rst_lcp, vol_cnt_rst_lcp;
unsigned char cnt_rst_ipcp, vol_cnt_rst_ipcp;
unsigned char cnt_rst_pap, vol_cnt_rst_pap;

unsigned int cnt_lcp_tm_out, cnt_ipcp_tm_out;
unsigned int cnt_pap_tm_out;
unsigned long int cnt_ip_tm_cntr;
unsigned char rcvd_protokol;

unsigned char i_scr_lcp, i_scan_lcp;
unsigned char i_scj_lcp; // идентификаторы lcp
unsigned char i_str_lcp, i_sta_lcp;

unsigned char i_scr_ipcp, i_scan_ipcp;
unsigned char i_scj_ipcp; // идентификаторы ipcp
unsigned char i_str_ipcp, i_sta_ipcp;

unsigned char i_scr_pap;

unsigned char buf_rejc_opt_lcp[MAX_DL_LCP - 6];
unsigned char buf_ack_opt_lcp[MAX_DL_LCP - 12];
unsigned char buf_nak_opt_lcp[MAX_DL_LCP - 12];
unsigned char buf_rej_opt_lcp[MAX_DL_LCP - 12];

unsigned char buf_rejc_opt_ipcp[MAX_DL_IPCP - 4];
unsigned char buf_ack_opt_ipcp[MAX_DL_IPCP - 10];
unsigned char buf_nak_opt_ipcp[MAX_DL_IPCP - 10];
unsigned char buf_rej_opt_ipcp[MAX_DL_IPCP - 10];

unsigned char ln_rejc_lcp, ln_rej_lcp, ln_ack_lcp, ln_nak_lcp;

unsigned char ln_rejc_ipcp, ln_rej_ipcp, ln_ack_ipcp, ln_nak_ipcp;

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

extern unsigned char ip_self[4];
unsigned char ip_pri_dns[4], ip_sec_dns[4];
// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

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

extern unsigned char Appl_seq_des;
extern unsigned char state_seq;

extern struct {
  unsigned char en_tx : 1;
  unsigned char kv_waits : 1;
  unsigned char en_povtor : 1;
} fl_appl_seq;

extern struct {
  unsigned char over_buf : 1;
  unsigned char send_state : 1;
  unsigned char enable : 1;
} fl_appl_seq1;

// extern unsigned char byte_state_net;

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

unsigned int delay_pre_tx;

//!!!!!!!!!!!!!!!11удаленная загрузка
unsigned char off_buf, total_kol_page;
unsigned int pagee;
unsigned char contr_gluk1, contr_gluk2;
unsigned char *array_in_232;
unsigned char reboot_byte;

struct {
  unsigned char end_pg : 1;   // флаг последней страницы
  unsigned char ch_crc : 1;   // флаг проверки crc
  unsigned char send_kv : 1;  // флаг отсылки квитка
  unsigned char cor_pg : 1;   // флаг корректной страницы дальше
  unsigned char first_pg : 1; // флаг первой страницы
  unsigned char hold_pg : 1;  // флаг захвата данных
  unsigned char send_crc : 1; // флаг посылки опции СРС
  // unsigned char  yes_crc     :1;// верный crc
} fl_pg_out;

unsigned int pg_crc;    // значение crc куска данных
unsigned int cnt_pg;    // реальный счетчик страниц
unsigned char id_pg_kp; // циклический идентификатор страниц контроллера
unsigned char
    id_pg_appl; // циклический идентификатор страниц верхнего приложения

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

extern unsigned char Appl_RS485_1_data_buf[LN_BUF_485_1]; // сам буфер
extern unsigned char Appl_RS485_2_data_buf[LN_BUF_485_2]; // сам буфер
extern unsigned char Appl_RS232_2_data_buf[LN_BUF_232_2]; // сам буфер

extern struct // структура описывающая работу приложения "RS485_1"
{
  enum bool fl_data_buf; // имеется ли забуферизированный пакет для передачи в
                         // порт RS485_1
  unsigned int ln_data_buf;  // забуферизированная длина буфера
  unsigned int dst_buf;      // забуферизированный получатель
  unsigned char cont_buf[8]; // забуферизированный контекст
  unsigned char id_buf;      // забуферизированный id
  unsigned int dst_tek;      // текущий получатель
  unsigned char id_tek;      // текущий id
  unsigned int pre_tx;       // задержка перед передачей
} Appl_RS485_1, Appl_RS485_2, Appl_RS232_2;

/*
extern struct
 {
 unsigned char mon1 : 1;
 unsigned char mon2 : 1;
 } fl_tx485;
*/

extern struct {
  unsigned char busy : 1;
  unsigned char rec : 1;
  unsigned char tm_out : 1;
  unsigned char tx : 1;
  unsigned char over : 1;
  unsigned char buffed : 1;
} fl_485_1, fl_485_2, fl_232_2;

struct // структура, описывающая объект передачи по PPP
{
  enum bool prozr;        // версия
  enum t_version version; // версия
  enum t_type type_pac;   // тип пакета
  unsigned int num_src;   // номер отправителя
  unsigned int num_dst;   // номер получателя
  unsigned char id_pac;   // идентификатор пакета
  unsigned char *p_opt;   // //указатель буфера опций
  unsigned char l_opt;    // длина буфера опций
  unsigned char kol_opt;  // количество опций
  unsigned char *p_data;  // указатель буфера данных
  unsigned int l_data;    // длина данных
} Obj_ppp_tx;

extern unsigned char buf_opt_tr[20];

__flash unsigned int fcstab[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48,
    0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 0x1081, 0x0108,
    0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb,
    0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399,
    0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e,
    0xfae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e,
    0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd,
    0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285,
    0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44,
    0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 0x6306, 0x728f, 0x4014,
    0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5,
    0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3,
    0x242a, 0x16b1, 0x0738, 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862,
    0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e,
    0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 0x18c1,
    0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483,
    0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50,
    0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710,
    0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7,
    0x6e6e, 0x5cf5, 0x4d7c, 0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1,
    0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72,
    0x3efb, 0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e,
    0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf,
    0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 0xf78f, 0xe606, 0xd49d,
    0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c,
    0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

void prov_uk_in(void) {
  unsigned char temp;

  uk_in_act_LCP++;
  if (uk_in_act_LCP >= LCP_ACT_VOL)
    uk_in_act_LCP = 0;
  if (uk_out_act_LCP == 0)
    temp = LCP_ACT_VOL - 1;
  else
    temp = uk_out_act_LCP - 1;
  //   if(uk_in_act_LCP==temp){write_log_info(ST_ERROR,ERR3);lock_it();}//переполнение
  //   буфера
  if (uk_in_act_LCP == temp)
    event_modem = EVM_PPP_ERR; // переполнение буфера
}

void prov_uk_in_ipcp(void) {
  unsigned char temp;

  uk_in_act_IPCP++;
  if (uk_in_act_IPCP >= IPCP_ACT_VOL)
    uk_in_act_IPCP = 0;
  if (uk_out_act_IPCP == 0)
    temp = IPCP_ACT_VOL - 1;
  else
    temp = uk_out_act_IPCP - 1;
  //  if(uk_in_act_IPCP==temp){write_log_info(ST_ERROR,ERR4);lock_it();}//переполнение
  //  буфера
  if (uk_in_act_IPCP == temp)
    event_modem = EVM_PPP_ERR; // переполнение буфера
}

unsigned int pppfcs16(unsigned int fcs, unsigned char *cp, unsigned int len)

{

  while (len--)
    fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
  return (fcs);
}

void run_lcp_act(void) {
  unsigned char i;

  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp;

  switch (LCP_act_buf[uk_out_act_LCP]) {
  case TLU:

    fl_pap.up = 1;
    fl_lcp.act_lcp_end = 0;
    PAP_state = START_ST;
    layer_PPP = LAYER_PAP;
    fl_lcp.act_lcp_end = 1;
    break;
  case TLD:
    fl_lcp.act_lcp_end = 0;
    layer_PPP = LAYER_LCP;
    fl_lcp.act_lcp_end = 1;
    break;

  case IRC:
    fl_lcp.act_lcp_end = 0;
    cnt_lcp_tm_out = VOL_LCP_TM_OUT;
    //  cnt_rst_lcp=vol_cnt_rst_lcp;
    fl_lcp.lcp_tm_out_en = 1;
    fl_lcp.act_lcp_end = 1;

    break;

  case SCR:

    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;

    buf_tx_232[5] = CONF_REQ;
    buf_tx_232[6] = i_scr_lcp;

    buf_tx_232[7] = 0;
    buf_tx_232[8] = 0x0e;

    buf_tx_232[9] = 2;
    buf_tx_232[10] = 6;

    buf_tx_232[11] = 0;
    buf_tx_232[12] = 0x0a;
    // buf_tx_232[12]=0;
    buf_tx_232[13] = 0;
    buf_tx_232[14] = 0;

    buf_tx_232[15] = 7;
    buf_tx_232[16] = 2;

    buf_tx_232[17] = 8;
    buf_tx_232[18] = 2;

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
    buf_tx_232[19] = temp.bytes[0];
    buf_tx_232[20] = temp.bytes[1];

    buf_tx_232[21] = 0x7e;

    count_tx_ppp = 0;
    vol_tx_ppp = buf_tx_232[8] + 8;

    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    S2_RD;
    UDR0 = buf_tx_232[0];
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[0];

    break;

  case SCA:
    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;

    buf_tx_232[5] = CONF_ACK;
    buf_tx_232[6] = i_scan_lcp;

    temp.word = ln_ack_lcp + 4;
    buf_tx_232[7] = temp.bytes[1];
    buf_tx_232[8] = temp.bytes[0];

    temp.word = temp.word + 4;
    vol_tx_ppp = temp.word + 4;

    for (i = 0; i < ln_ack_lcp; i++)
      buf_tx_232[9 + i] = buf_ack_opt_lcp[i];

    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
    buf_tx_232[9 + ln_ack_lcp] = temp.bytes[0];
    buf_tx_232[10 + ln_ack_lcp] = temp.bytes[1];
    buf_tx_232[11 + ln_ack_lcp] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    S2_RD;
    UDR0 = buf_tx_232[0];
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[0];

    break;
  case SCN:

    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;

    if (fl_lcp2.nac_rej == 0) {
      buf_tx_232[5] = CONF_NAK;

      buf_tx_232[6] = i_scan_lcp;

      temp.word = ln_nak_lcp + 4;
      buf_tx_232[7] = temp.bytes[1];
      buf_tx_232[8] = temp.bytes[0];

      temp.word = temp.word + 4;
      vol_tx_ppp = temp.word + 4;

      for (i = 0; i < ln_nak_lcp; i++)
        buf_tx_232[9 + i] = buf_nak_opt_lcp[i];
      temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
      buf_tx_232[9 + ln_nak_lcp] = temp.bytes[0];
      buf_tx_232[10 + ln_nak_lcp] = temp.bytes[1];
      buf_tx_232[11 + ln_nak_lcp] = 0x7e;

      count_tx_ppp = 0;
      UCSR0A = UCSR0A | TXC;
      UCSR0B = UCSR0B | TXEN;
      UCSR0B = UCSR0B | TXCIE;
      if (check_cts() == 1)
        return;
      S2_RD;
      UDR0 = buf_tx_232[0];
      if (Regim == RG_DEBAG)
        UDR3 = buf_tx_232[0];

    } else {
      fl_lcp2.nac_rej = 0;
      buf_tx_232[5] = CONF_REJ;
      buf_tx_232[6] = i_scan_lcp;

      temp.word = ln_rej_lcp + 4;
      buf_tx_232[7] = temp.bytes[1];
      buf_tx_232[8] = temp.bytes[0];

      temp.word = temp.word + 4;
      vol_tx_ppp = temp.word + 4;

      for (i = 0; i < ln_rej_lcp; i++)
        buf_tx_232[9 + i] = buf_rej_opt_lcp[i];
      temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
      buf_tx_232[9 + ln_rej_lcp] = temp.bytes[0];
      buf_tx_232[10 + ln_rej_lcp] = temp.bytes[1];
      buf_tx_232[11 + ln_rej_lcp] = 0x7e;

      count_tx_ppp = 0;
      UCSR0A = UCSR0A | TXC;
      UCSR0B = UCSR0B | TXEN;
      UCSR0B = UCSR0B | TXCIE;
      if (check_cts() == 1)
        return;
      S2_RD;
      UDR0 = buf_tx_232[0];
      if (Regim == RG_DEBAG)
        UDR3 = buf_tx_232[0];
    }

    break;
  }
}

void monitor_act_PAP(void) {

  // unsigned char i;
  unsigned char temp1;

  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp;

  if ((pap_act_buf == SCR) && (fl_pap.act_pap_end == 1) &&
      (fl_lcp.act_lcp_end == 1) && (fl_ipcp.act_ipcp_end == 1) &&
      (fl_ip.act_ip_end == 1)) {

    pap_act_buf = NO_EVENT;

    fl_pap.act_pap_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xc0;
    buf_tx_232[2] = 0x23;

    buf_tx_232[3] = CONF_REQ;

    i_scr_pap++;
    buf_tx_232[4] = i_scr_pap;

    if (simka == SIM_BASE) {
      RdFromFleshToArr(A_C_GPRS, &temp1, 1); // количество байт APN
      RdFromFleshToArr(A_C_GPRS + temp1 + 1, &temp.bytes[0],
                       1); // количество логина
      RdFromFleshToArr(A_C_GPRS + temp1 + temp.bytes[0] + 2, &temp.bytes[1],
                       1); // количество пароля

      if ((temp.bytes[0] == 0) || (temp.bytes[0] > MAX_VOL_US_NAME))
        buf_tx_232[7] = 0;
      else {
        buf_tx_232[7] = temp.bytes[0];
        RdFromFleshToArr(A_C_GPRS + temp1 + 2, &buf_tx_232[8], temp.bytes[0]);
      }

      if ((temp.bytes[1] == 0) || (temp.bytes[1] > MAX_VOL_PSW))
        buf_tx_232[8 + buf_tx_232[7]] = 0;
      else {
        buf_tx_232[8 + buf_tx_232[7]] = temp.bytes[1];
        RdFromFleshToArr(A_C_GPRS + temp1 + temp.bytes[0] + 3,
                         &buf_tx_232[9 + buf_tx_232[7]], temp.bytes[1]);
      }
    } else //!!!!!!!!!!dobavka
    {
      RdFromFleshToArr(A_CR_GPRS, &temp1, 1); // количество байт APN
      RdFromFleshToArr(A_CR_GPRS + temp1 + 1, &temp.bytes[0],
                       1); // количество логина
      RdFromFleshToArr(A_CR_GPRS + temp1 + temp.bytes[0] + 2, &temp.bytes[1],
                       1); // количество пароля

      if ((temp.bytes[0] == 0) || (temp.bytes[0] > MAX_VOL_US_NAME))
        buf_tx_232[7] = 0;
      else {
        buf_tx_232[7] = temp.bytes[0];
        RdFromFleshToArr(A_CR_GPRS + temp1 + 2, &buf_tx_232[8], temp.bytes[0]);
      }

      if ((temp.bytes[1] == 0) || (temp.bytes[1] > MAX_VOL_PSW))
        buf_tx_232[8 + buf_tx_232[7]] = 0;
      else {
        buf_tx_232[8 + buf_tx_232[7]] = temp.bytes[1];
        RdFromFleshToArr(A_CR_GPRS + temp1 + temp.bytes[0] + 3,
                         &buf_tx_232[9 + buf_tx_232[7]], temp.bytes[1]);
      }
    }
    //!!!!!!!!!!dobavka

    temp.word = 6 + buf_tx_232[7] + buf_tx_232[8 + buf_tx_232[7]];

    buf_tx_232[5] = temp.bytes[1];
    buf_tx_232[6] = temp.bytes[0];

    temp.word = temp.word + 2;
    vol_tx_ppp = temp.word + 4;

    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
    buf_tx_232[vol_tx_ppp - 3] = temp.bytes[0];
    buf_tx_232[vol_tx_ppp - 2] = temp.bytes[1];

    buf_tx_232[vol_tx_ppp - 1] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;

    S2_RD;
    UDR0 = buf_tx_232[0];
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[0];
  }
}

void monitor_event_PAP(void) {
  if (fl_pap.up == 1) // UP
  {
    fl_pap.up = 0;
    switch (PAP_state) {
    case START_ST:
      vol_cnt_rst_pap = VOL_PAP_MC;

      // IRC;   //!!!!!!!!!!!!!!111
      cnt_pap_tm_out = VOL_PAP_TM_OUT;
      // cnt_rst_pap=vol_cnt_rst_pap;
      fl_pap.pap_tm_out_en = 1;
      fl_pap.act_pap_end = 1;
      pap_act_buf = SCR;
      PAP_state = SEND_REQ;
      return;
    }

    return;
  }

  if (fl_pap.t0_pl == 1) // TO_PL
  {
    fl_pap.t0_pl = 0;
    switch (PAP_state) {

    case SEND_REQ:
      // vol_cnt_rst_pap=VOL_PAP_MC;
      cnt_pap_tm_out = VOL_PAP_TM_OUT;
      pap_act_buf = SCR;
      PAP_state = SEND_REQ;
      return;
    }

    return;
  }

  switch (PAP_event) {

  case RCA:

    PAP_event = NO_EVENT;
    switch (PAP_state) {

    case SEND_REQ:
      // TLU;

      fl_pap.up = 0;
      PAP_state = OPEN_ST;
      IPCP_state = START_ST;
      fl_ipcp.act_ipcp_end = 1;
      fl_ipcp1.up = 1;
      fl_ipcp1.down = 0;
      layer_PPP = LAYER_IPCP;

      return;
    }
    return;
  }
}

void monitor_act_LCP(void) {

  if ((uk_in_act_LCP != uk_out_act_LCP) && (fl_lcp.act_lcp_end == 1) &&
      (fl_pap.act_pap_end == 1) && (fl_ipcp.act_ipcp_end == 1) &&
      (fl_ip.act_ip_end == 1)) {
    run_lcp_act();
    uk_out_act_LCP++;
    if (uk_out_act_LCP >= LCP_ACT_VOL)
      uk_out_act_LCP = 0;
  }
}

void monitor_event_LCP(void) {
  if (fl_lcp1.up == 1) // UP
  {

    fl_lcp1.up = 0;
    switch (LCP_state) {
    case START_ST:

      // vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }

    return;
  }

  if (fl_lcp.t0_pl == 1) // TO_PL
  {
    fl_lcp.t0_pl = 0;
    switch (LCP_state) {

    case SEND_REQ:
      //  vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case REC_ACK:
      // vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      // vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_ACK;
      return;
    }

    return;
  }

  switch (LCP_event) {
  case RCR_PL:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case SEND_REQ:
      LCP_act_buf[uk_in_act_LCP] = SCA;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case REC_ACK:

      LCP_act_buf[uk_in_act_LCP] = SCA;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = TLU;
      prov_uk_in();
      LCP_state = OPEN_ST;

      return;
    case SEND_ACK:
      LCP_act_buf[uk_in_act_LCP] = SCA;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      //    vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCA;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    }
    return;
  case RCR_MI:

    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case SEND_REQ:

      LCP_act_buf[uk_in_act_LCP] = SCN;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      LCP_act_buf[uk_in_act_LCP] = SCN;
      prov_uk_in();
      LCP_state = REC_ACK;

      return;
    case SEND_ACK:
      LCP_act_buf[uk_in_act_LCP] = SCN;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      //     vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCN;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }
    return;
  case RCA:

    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case SEND_REQ:

      //  vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_state = REC_ACK;

      return;
    case REC_ACK:
      //    vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      //   vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = TLU;
      prov_uk_in();
      LCP_state = OPEN_ST;

      return;
    case OPEN_ST:
      //    vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }
    return;
  case RCN:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case SEND_REQ:
      //   vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      //    vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      //    vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      //     vol_cnt_rst_lcp=VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }
  }
}

////////////////мониторы IPCP

void run_ipcp_act(void) {
  unsigned char i;

  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp;

  switch (IPCP_act_buf[uk_out_act_IPCP]) {
  case TLU:

    event_modem = EVM_PPP_OK;

    layer_PPP = LAYER_IP;
    fl_ip.act_ip_end = 1;
    fl_ipcp.act_ipcp_end = 1;

    Control.link_waits = FALSE;
    Control.link = FALSE;
    Control.nat = FALSE;

    if (Control.vol_link != 0)
      fl_ip.ip_tm_cntr_en = 1;
    if (Control.vol_nat != 0)
      fl_ip.ip_tm_nat_en = 1;
    Control.cnt_try = 0;
    Control.cnt_link = BEG_CNTR_LINK;
    clr_cntr_nat();

    Control.cnt_nat = BEG_CNTR_LINK - 1; // пустой пакет!!!!

    break;

  case IRC:
    fl_ipcp.act_ipcp_end = 0;
    cnt_ipcp_tm_out = VOL_IPCP_TM_OUT;
    cnt_rst_ipcp = vol_cnt_rst_ipcp;
    fl_ipcp.ipcp_tm_out_en = 1;
    fl_ipcp.act_ipcp_end = 1;

    break;

  case SCR:

    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    buf_tx_232[3] = CONF_REQ;
    buf_tx_232[4] = i_scr_ipcp;

    buf_tx_232[5] = 0;
    buf_tx_232[6] = 10; // без DNS

    //  buf_tx_232[6]=22;//c DNS

    buf_tx_232[7] = 3; // запрос IP контроллера
    buf_tx_232[8] = 6;
    buf_tx_232[9] = ip_self[0];
    buf_tx_232[10] = ip_self[1];
    buf_tx_232[11] = ip_self[2];
    buf_tx_232[12] = ip_self[3];

    //  buf_tx_232[13]=0x81;      //запрос IP_PRI_DNS
    //  buf_tx_232[14]=6;
    //  buf_tx_232[15]=ip_pri_dns[0];
    //  buf_tx_232[16]=ip_pri_dns[1];
    // buf_tx_232[17]=ip_pri_dns[2];
    // buf_tx_232[18]=ip_pri_dns[3];

    // buf_tx_232[19]=0x83;      //запрос IP_SEC_DNS
    // buf_tx_232[20]=6;
    // buf_tx_232[21]=ip_sec_dns[0];
    // buf_tx_232[22]=ip_sec_dns[1];
    // buf_tx_232[23]=ip_sec_dns[2];
    // buf_tx_232[24]=ip_sec_dns[3];

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[6] + 2) ^ 0xffff;
    buf_tx_232[13] = temp.bytes[0]; // без DNS
    buf_tx_232[14] = temp.bytes[1]; // без DNS

    buf_tx_232[15] = 0x7e; // без DNS
    //  buf_tx_232[25]=temp.bytes[0];//c DNS
    //  buf_tx_232[26]=temp.bytes[1]; //c DNS
    //  buf_tx_232[27]=0x7e; //c DNS

    count_tx_ppp = 0;
    vol_tx_ppp = buf_tx_232[6] + 6;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    S2_RD;
    UDR0 = buf_tx_232[0];
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[0];

    break;

  case SCA:
    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    buf_tx_232[3] = CONF_ACK;
    buf_tx_232[4] = i_scan_ipcp;

    temp.word = ln_ack_ipcp + 4;
    buf_tx_232[5] = temp.bytes[1];
    buf_tx_232[6] = temp.bytes[0];

    temp.word = temp.word + 2;
    vol_tx_ppp = temp.word + 4;

    for (i = 0; i < ln_ack_ipcp; i++)
      buf_tx_232[7 + i] = buf_ack_opt_ipcp[i];

    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
    buf_tx_232[7 + ln_ack_ipcp] = temp.bytes[0];
    buf_tx_232[8 + ln_ack_ipcp] = temp.bytes[1];
    buf_tx_232[9 + ln_ack_ipcp] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    S2_RD;
    UDR0 = buf_tx_232[0];
    if (Regim == RG_DEBAG)
      UDR3 = buf_tx_232[0];

    break;
  case SCN:

    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    if (fl_ipcp2.nac_rej == 0) {
      buf_tx_232[3] = CONF_NAK;

      buf_tx_232[4] = i_scan_ipcp;

      temp.word = ln_nak_ipcp + 4;
      buf_tx_232[5] = temp.bytes[1];
      buf_tx_232[6] = temp.bytes[0];

      temp.word = temp.word + 2;
      vol_tx_ppp = temp.word + 4;

      for (i = 0; i < ln_nak_ipcp; i++)
        buf_tx_232[7 + i] = buf_nak_opt_ipcp[i];
      temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
      buf_tx_232[7 + ln_nak_ipcp] = temp.bytes[0];
      buf_tx_232[8 + ln_nak_ipcp] = temp.bytes[1];
      buf_tx_232[9 + ln_nak_ipcp] = 0x7e;

      count_tx_ppp = 0;
      UCSR0A = UCSR0A | TXC;
      UCSR0B = UCSR0B | TXEN;
      UCSR0B = UCSR0B | TXCIE;
      if (check_cts() == 1)
        return;
      S2_RD;
      UDR0 = buf_tx_232[0];
      if (Regim == RG_DEBAG)
        UDR3 = buf_tx_232[0];

    } else {
      fl_ipcp2.nac_rej = 0;
      buf_tx_232[3] = CONF_REJ;
      buf_tx_232[4] = i_scan_ipcp;

      temp.word = ln_rej_ipcp + 4;
      buf_tx_232[5] = temp.bytes[1];
      buf_tx_232[6] = temp.bytes[0];

      temp.word = temp.word + 2;
      vol_tx_ppp = temp.word + 4;

      for (i = 0; i < ln_rej_ipcp; i++)
        buf_tx_232[7 + i] = buf_rej_opt_ipcp[i];
      temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;
      buf_tx_232[7 + ln_rej_ipcp] = temp.bytes[0];
      buf_tx_232[8 + ln_rej_ipcp] = temp.bytes[1];
      buf_tx_232[9 + ln_rej_ipcp] = 0x7e;

      count_tx_ppp = 0;
      UCSR0A = UCSR0A | TXC;
      UCSR0B = UCSR0B | TXEN;
      UCSR0B = UCSR0B | TXCIE;
      if (check_cts() == 1)
        return;
      S2_RD;
      UDR0 = buf_tx_232[0];
      if (Regim == RG_DEBAG)
        UDR3 = buf_tx_232[0];
    }

    break;
  }
}

void monitor_act_IPCP(void) {

  if ((uk_in_act_IPCP != uk_out_act_IPCP) && (fl_lcp.act_lcp_end == 1) &&
      (fl_pap.act_pap_end == 1) && (fl_ipcp.act_ipcp_end == 1) &&
      (fl_ip.act_ip_end == 1)) {
    run_ipcp_act();
    uk_out_act_IPCP++;
    if (uk_out_act_IPCP >= IPCP_ACT_VOL)
      uk_out_act_IPCP = 0;
  }
}

void monitor_event_IPCP(void) {
  if (fl_ipcp1.up == 1) // UP
  {

    fl_ipcp1.up = 0;
    switch (IPCP_state) {
    case START_ST:
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }

    return;
  }

  if (fl_ipcp.t0_pl == 1) // TO_PL
  {
    fl_ipcp.t0_pl = 0;
    switch (IPCP_state) {

    case SEND_REQ:
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;
      return;
    case REC_ACK:
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      //  vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;
      return;
    }

    return;
  }

  switch (IPCP_event) {
  case RCR_PL:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case SEND_REQ:
      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    case REC_ACK:

      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = TLU;
      prov_uk_in_ipcp();
      IPCP_state = OPEN_ST;

      return;
    case SEND_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    case OPEN_ST:

      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();

      return;
    }
    return;
  case RCR_MI:

    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case SEND_REQ:

      IPCP_act_buf[uk_in_act_IPCP] = SCN;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = SCN;
      prov_uk_in_ipcp();
      IPCP_state = REC_ACK;

      return;
    case SEND_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = SCN;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;

      /*
      case OPEN_ST:

                  // vol_cnt_rst_ipcp=VOL_IPCP_MC;
                   IPCP_act_buf[uk_in_act_IPCP]=IRC;
                   prov_uk_in_ipcp();
                   IPCP_act_buf[uk_in_act_IPCP]=SCR;
                   prov_uk_in_ipcp();
                   IPCP_act_buf[uk_in_act_IPCP]=SCN;
                   prov_uk_in_ipcp();
                   IPCP_state=SEND_REQ;


                   return;
        */
    }
    return;
  case RCA:

    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case SEND_REQ:

      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_state = REC_ACK;

      return;
    case REC_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case SEND_ACK:

      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = TLU;
      prov_uk_in_ipcp();
      IPCP_state = OPEN_ST;

      return;
      /*
      case OPEN_ST:
                  // vol_cnt_rst_ipcp=VOL_IPCP_MC;
     //подумать    ?????????????????????????????

                   IPCP_act_buf[uk_in_act_IPCP]=IRC;
                   prov_uk_in_ipcp();
                   IPCP_act_buf[uk_in_act_IPCP]=SCR;
                   prov_uk_in_ipcp();
                   IPCP_state=SEND_REQ;


                   return;
                   */
    }
    return;
  case RCN:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case SEND_REQ:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;

    case OPEN_ST:
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      // vol_cnt_rst_ipcp=VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }
    return;
  }
}

////////////////  конец мониторы IPCP

unsigned int calc_crc_ip(unsigned char *p, unsigned int count) {

  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp1;

  union {
    unsigned char bytes[4];
    unsigned long int long_word;
  } temp2;

  temp1.word = 0;
  temp2.long_word = 0;

  while (count > 1) {
    /*  This is the inner loop */
    temp1.bytes[0] = *p;
    temp1.bytes[1] = *(p + 1);
    temp2.long_word = temp2.long_word + temp1.word;
    p = p + 2;
    count -= 2;
  }

  /*  Add left-over byte, if any */
  if (count > 0) {
    temp1.bytes[0] = 0;
    temp1.bytes[1] = *p;
    temp2.long_word = temp2.long_word + temp1.word;
  }

  /*  Fold 32-bit sum to 16 bits */
  while (temp2.long_word >> 16)
    temp2.long_word = (temp2.long_word & 0xffff) + (temp2.long_word >> 16);

  temp1.bytes[0] = ~temp2.bytes[0];
  temp1.bytes[1] = ~temp2.bytes[1];

  return (temp1.word);
}

unsigned int calc_crc_udp_2(unsigned char *buf_rx_ppp, unsigned int count) {
  unsigned char *p;

  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp1;

  union {
    unsigned char bytes[4];
    unsigned long int long_word;
  } temp2;
  p = buf_rx_ppp + 22;
  temp1.word = 0;
  temp2.long_word = 0;

  while (count > 1) {
    /*  This is the inner loop */
    temp1.bytes[0] = *p;
    temp1.bytes[1] = *(p + 1);
    temp2.long_word = temp2.long_word + temp1.word;
    p = p + 2;
    count -= 2;
  }

  /*  Add left-over byte, if any */
  if (count > 0) {
    // temp1.bytes[0]=0;
    // temp1.bytes[1]=*p;
    temp1.bytes[0] = *p;
    temp1.bytes[1] = 0;
    temp2.long_word = temp2.long_word + temp1.word;
  }

  temp1.bytes[0] = buf_rx_ppp[14];
  temp1.bytes[1] = buf_rx_ppp[15];
  temp2.long_word = temp2.long_word + temp1.word;
  temp1.bytes[0] = buf_rx_ppp[16];
  temp1.bytes[1] = buf_rx_ppp[17];
  temp2.long_word = temp2.long_word + temp1.word;

  temp1.bytes[0] = buf_rx_ppp[18];
  temp1.bytes[1] = buf_rx_ppp[19];
  temp2.long_word = temp2.long_word + temp1.word;
  temp1.bytes[0] = buf_rx_ppp[20];
  temp1.bytes[1] = buf_rx_ppp[21];
  temp2.long_word = temp2.long_word + temp1.word;

  temp1.bytes[0] = 0;
  temp1.bytes[1] = 0x11;
  temp2.long_word = temp2.long_word + temp1.word;

  temp1.bytes[0] = buf_rx_ppp[26]; // длина
  temp1.bytes[1] = buf_rx_ppp[27];
  temp2.long_word = temp2.long_word + temp1.word;

  /*  Fold 32-bit sum to 16 bits */
  while (temp2.long_word >> 16)
    temp2.long_word = (temp2.long_word & 0xffff) + (temp2.long_word >> 16);

  temp1.bytes[0] = ~temp2.bytes[0];
  temp1.bytes[1] = ~temp2.bytes[1];

  return (temp1.word);
}

unsigned char proc_ppp_packet(unsigned char *buf_rx_ppp,
                              unsigned int count_rx_ppp) {
  unsigned int i;
  unsigned char vol_length_opt;
  unsigned char ind_opt, count_length_opt;

  struct {
    unsigned char mru : 1;
    unsigned char accm : 1;
    unsigned char aut_prot : 1;
    unsigned char pfc : 1;
    unsigned char acfc : 1;
    unsigned char zap_opt : 1;
    unsigned char ip_self : 1;
    unsigned char ip_pri_dns : 1;
  } fl_opt;

  union {
    unsigned char bytes[2];
    unsigned int word;
  } Crc_out, temp, length;

  if (count_rx_ppp <= 5)
    return (1);

  // подсчет FCS
  if (pppfcs16(PPPINITFCS16, &buf_rx_ppp[1], count_rx_ppp - 2) != PPPGOODFCS16)
    return (1);

  if ((buf_rx_ppp[1] == 0xff) && (buf_rx_ppp[2] == 0x03) &&
      (buf_rx_ppp[3] == 0xc0) && (buf_rx_ppp[4] == 0x21) &&
      (layer_PPP != LAYER_HW))
    rcvd_protokol = PR_LCP;
  if ((buf_rx_ppp[1] == 0xc0) && (buf_rx_ppp[2] == 0x23) &&
      (layer_PPP == LAYER_PAP))
    rcvd_protokol = PR_PAP;
  if ((buf_rx_ppp[1] == 0x80) && (buf_rx_ppp[2] == 0x21) &&
      ((layer_PPP == LAYER_IP) || (layer_PPP == LAYER_IPCP)))
    rcvd_protokol = PR_IPCP;
  if ((buf_rx_ppp[1] == 0x21) && (layer_PPP == LAYER_IP))
    rcvd_protokol = PR_IP;

  switch (rcvd_protokol) {
  case PR_LCP:

    // здесь  проверка общей длины
    length.bytes[1] = buf_rx_ppp[7];
    length.bytes[0] = buf_rx_ppp[8];
    if (length.word != count_rx_ppp - 8)
      return (1);
    if (count_rx_ppp > MAX_DL_LCP)
      return (1); // не воспринимать длинные пакеты

    vol_length_opt = count_rx_ppp - 12;
    count_length_opt = 0;
    ind_opt = 9;

    ln_rejc_lcp = 0;
    ln_ack_lcp = 0;
    ln_nak_lcp = 0;
    ln_rej_lcp = 0;

    switch (buf_rx_ppp[5]) {
    case CONF_REQ:
      // (если вообще не те опции то сброс кода дать)
      // проверка предложенной конфигурации от сервера
      // если нужная конфигурация то событие RCR+
      // если нет то RCR-;

      /*
            for(i=0;i<vol_length_opt;i++)              //соглашение на любую
         конфигурацию
                {
                buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                ln_ack_lcp++;
                //count_length_opt++;
                ind_opt++;
                }

           LCP_event=RCR_PL;
           i_scan_lcp=buf_rx_ppp[6];    // присвоение идентификатора
           return(1);
              */

      fl_opt.zap_opt = 0;

    next_opt_lcp_11:
      if (count_length_opt == vol_length_opt)
        goto end_pcp_opt_11;
      // обработаны все опции
      switch (buf_rx_ppp[ind_opt]) {
      case AUT_PROT:

        if ((buf_rx_ppp[ind_opt + 2] != 0xc0) ||
            (buf_rx_ppp[ind_opt + 3] != 0x23)) {
          fl_opt.zap_opt = 1;

          buf_nak_opt_lcp[ln_nak_lcp] = 3;
          ln_nak_lcp++;
          buf_nak_opt_lcp[ln_nak_lcp] = 4;
          ln_nak_lcp++;
          buf_nak_opt_lcp[ln_nak_lcp] = 0xc0;
          ln_nak_lcp++;
          buf_nak_opt_lcp[ln_nak_lcp] = 0x23;
          ln_nak_lcp++;

          //  count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
          //  ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
        } else
          for (i = 0; i < buf_rx_ppp[ind_opt + 1]; i++) {
            buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + i];
            ln_ack_lcp++;
          }

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_11;

      default:
        for (i = 0; i < buf_rx_ppp[ind_opt + 1]; i++) {
          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + i];
          ln_ack_lcp++;
        }
        for (i = 0; i < buf_rx_ppp[ind_opt + 1]; i++) {
          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + i];
          ln_nak_lcp++;
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_11;
      }

    end_pcp_opt_11:

      if (fl_opt.zap_opt == 0)
        LCP_event = RCR_PL;
      else
        LCP_event = RCR_MI;
      fl_opt.zap_opt = 0;
      i_scan_lcp = buf_rx_ppp[6]; // присвоение идентификатора
      return (1);

      /*

        next_opt_lcp_1:  if(count_length_opt==vol_length_opt) {

                                                              goto
  end_pcp_opt_1; }   //обработаны все опции

                        switch(buf_rx_ppp[ind_opt])
                           {
                             case MRU:

                                  if (buf_rx_ppp[ind_opt+1]!=4)
                                       {
                                         for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                                         LCP_event=RUC;
                                         return(1);
                                         }
                                  if
  ((buf_rx_ppp[ind_opt+2]==DL_PPP_IN_HI)&&(buf_rx_ppp[ind_opt+3]==DL_PPP_IN_LO))
                                        {
                                         fl_opt.mru=1;
                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+2];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+3];
                                         ln_ack_lcp++;
                                        }
                                      else
                                         {
                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+2];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+3];
                                         ln_nak_lcp++;
                                         }
                                  count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                  ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                  goto next_opt_lcp_1;

                             case ACCM:
                                   if (buf_rx_ppp[ind_opt+1]!=6)
                                       {
                                         for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                                         LCP_event=RUC;
                                         return(1);
                                         }

                                     if (
                                            (buf_rx_ppp[ind_opt+2]==0)&&
                                            (buf_rx_ppp[ind_opt+3]==0)&&
                                            (buf_rx_ppp[ind_opt+4]==0)&&
                                            (buf_rx_ppp[ind_opt+5]==0)
                                         )

                                        {

                                         fl_opt.accm=1;
                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+2];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+3];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+4];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+5];
                                         ln_ack_lcp++;
                                        }
                                      else
                                         {
                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+2];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+3];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+4];
                                         ln_nak_lcp++;

                                         buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+5];
                                         ln_nak_lcp++;
                                         }
                                  count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                  ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                  goto next_opt_lcp_1;



                             case AUT_PROT:
                                       if (((buf_rx_ppp[ind_opt+1]-2)%2)!=0)
                                           {
                                            for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                                            LCP_event=RUC;
                                            return(1);
                                           }
                                       for(i=0;i<(buf_rx_ppp[ind_opt+1]-2);i=i+2)
                                          {
                                          if
  ((buf_rx_ppp[ind_opt+2+i]==0xc0)&&(buf_rx_ppp[ind_opt+3+i]==0x23))
                                               {

                                                fl_opt.aut_prot=1;

                                                buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                                                ln_ack_lcp++;

                                                buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+1];
                                                ln_ack_lcp++;

                                                buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+2+i];
                                                ln_ack_lcp++;

                                                buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+3+i];
                                                ln_ack_lcp++;
                                               }
                                             else
                                              {

                                              if (fl_opt.zap_opt==0)
                                                  {
                                                   fl_opt.zap_opt=1;
                                                   buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt];
                                                   ln_nak_lcp++;
                                                   buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+1];
                                                   ln_nak_lcp++;
                                                   }
                                              buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+2+i];
                                              ln_nak_lcp++;
                                               buf_nak_opt_lcp[ln_nak_lcp]=buf_rx_ppp[ind_opt+3+i];
                                              ln_nak_lcp++;
                                             }

                                            }
                                  count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                  ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                  goto next_opt_lcp_1;

                             case PFC:
                                   if (buf_rx_ppp[ind_opt+1]!=2)
                                       {
                                         for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                                         LCP_event=RUC;
                                         return(1);
                                         }


                                         fl_opt.pfc=1;
                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_ack_lcp++;

                                         count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                         ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                         goto next_opt_lcp_1;


                             case ACFC:
                                    if (buf_rx_ppp[ind_opt+1]!=2)
                                       {
                                         for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                                         LCP_event=RUC;
                                         return(1);
                                         }


                                         fl_opt.acfc=1;
                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt];
                                         ln_ack_lcp++;

                                         buf_ack_opt_lcp[ln_ack_lcp]=buf_rx_ppp[ind_opt+1];
                                         ln_ack_lcp++;

                                         count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                         ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                         goto next_opt_lcp_1;

                             default:

                                         for(i=0;i<buf_rx_ppp[ind_opt+1];i++){buf_rej_opt_lcp[ln_rej_lcp]=buf_rx_ppp[ind_opt+i];ln_rej_lcp++;}
                                         count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                                         ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                                   goto next_opt_lcp_1;
                           }

  end_pcp_opt_1:




                        if (ln_rej_lcp!=0)
                          {

                          LCP_event=RCR_MI;
                          fl_lcp2.nac_rej=1;
                          return(1);
                          }




                       if(fl_opt.accm==0)
                             {
                             buf_nak_opt_lcp[ln_nak_lcp]=ACCM;
                             ln_nak_lcp++;
                             buf_nak_opt_lcp[ln_nak_lcp]=6;
                             ln_nak_lcp++;
                             buf_nak_opt_lcp[ln_nak_lcp]=0;
                             ln_nak_lcp++;
                             buf_nak_opt_lcp[ln_nak_lcp]=0;
                             ln_nak_lcp++;
                             buf_nak_opt_lcp[ln_nak_lcp]=0;
                             ln_nak_lcp++;
                             buf_nak_opt_lcp[ln_nak_lcp]=0;
                             ln_nak_lcp++;
                             }

                       if(fl_opt.aut_prot==0)
                             {




                             buf_nak_opt_lcp[ln_nak_lcp]=AUT_PROT;
                             ln_nak_lcp++;

                             buf_nak_opt_lcp[ln_nak_lcp]=4;
                             ln_nak_lcp++;

                             buf_nak_opt_lcp[ln_nak_lcp]=0xc0;
                             ln_nak_lcp++;

                             buf_nak_opt_lcp[ln_nak_lcp]=0x23;
                             ln_nak_lcp++;
                             }
                       if(fl_opt.pfc==0)
                             {
                             buf_nak_opt_lcp[ln_nak_lcp]=PFC;
                             ln_nak_lcp++;

                             buf_nak_opt_lcp[ln_nak_lcp]=2;
                             ln_nak_lcp++;
                             }
                      if(fl_opt.acfc==0)
                             {
                              buf_nak_opt_lcp[ln_nak_lcp]=ACFC;
                             ln_nak_lcp++;

                             buf_nak_opt_lcp[ln_nak_lcp]=2;
                             ln_nak_lcp++;
                             }




                          if (ln_nak_lcp!=0)
                          {

                          LCP_event=RCR_MI;
                          fl_lcp2.nac_rej=0;
                          i_scan_lcp=buf_rx_ppp[6];     // присвоение
  идентификатора return(1);
                          }

                          if (ln_ack_lcp!=0)
                          {


                          LCP_event=RCR_PL;
                          i_scan_lcp=buf_rx_ppp[6];    // присвоение
  идентификатора return(1);
                          }



                       break;
                       */

    case CONF_ACK:
      // проверка опций (если вообще не те опции то сброс кода дать)
      // опции та, то событие RCA

      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      // не обрабатывать опции
      // сразу RCA
      LCP_event = RCA;
      i_scr_lcp++; // приращение идентификатора
      return (1);

      /*
next_opt_lcp_2:  if(count_length_opt==vol_length_opt) goto end_pcp_opt_2;
//обработаны все опции

         switch(buf_rx_ppp[ind_opt])
            {
              case ACCM:
                    if (buf_rx_ppp[ind_opt+1]!=6)
                        {
                          for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                          LCP_event=RUC;
                          return(1);
                          }

                      if (
                             (buf_rx_ppp[ind_opt+2]==0)&&
                             (buf_rx_ppp[ind_opt+3]==0x0a)&&
                             (buf_rx_ppp[ind_opt+4]==0)&&
                             (buf_rx_ppp[ind_opt+5]==0)
                          )  fl_opt.accm=1;




                   count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                   ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                   goto next_opt_lcp_2;

                case PFC:
                    if (buf_rx_ppp[ind_opt+1]!=2)
                        {
                          for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                          LCP_event=RUC;
                          return(1);
                          }

                          fl_opt.pfc=1;
                          count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                          ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                          goto next_opt_lcp_2;


              case ACFC:
                     if (buf_rx_ppp[ind_opt+1]!=2)
                        {
                          for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                          LCP_event=RUC;
                          return(1);
                          }

                          fl_opt.acfc=1;


                          count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                          ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
                          goto next_opt_lcp_2;

              default:
for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
                          LCP_event=RUC;
                          return(1);

            }


end_pcp_opt_2:      if((fl_opt.accm==1)&&(fl_opt.acfc==1)&&(fl_opt.pfc==1))
          {

           LCP_event=RCA;
           i_scr_lcp++;//приращение идентификатора
           return(1);
          }
          else
          {
          for(i=5;i<=count_rx_ppp-4;i++){buf_rejc_opt_lcp[ln_rejc_lcp]=buf_rx_ppp[i];ln_rejc_lcp++;}
          LCP_event=RUC;
          return(1);
          }
          */

    case CONF_NAK:
      //!!!!! подумать
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      i_scr_lcp++;  // приращение идентификатора
      LCP_event = RCN;

      return (1);
    case CONF_REJ:
      // !!!!! подумать!!!!!!!!
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      i_scr_lcp++;  // приращение идентификатора
      LCP_event = RCN;

      return (1);
    }

    break;

  case PR_PAP:

    // здесь  проверка общей длины
    length.bytes[1] = buf_rx_ppp[5];
    length.bytes[0] = buf_rx_ppp[6];
    if (length.word != count_rx_ppp - 6)
      return (1);
    if (count_rx_ppp > MAX_DL_PAP)
      return (1); // не воспринимать длинные пакеты

    switch (buf_rx_ppp[3]) {
    case CONF_ACK:
      if (buf_rx_ppp[4] != i_scr_pap)
        return (1); // проверка идентификатора
      PAP_event = RCA;
      return (1);
    }

    break;

  case PR_IPCP:

    // здесь  проверка общей длины
    length.bytes[1] = buf_rx_ppp[5];
    length.bytes[0] = buf_rx_ppp[6];
    if (length.word != count_rx_ppp - 6)
      return (1);
    if (count_rx_ppp > MAX_DL_IPCP)
      return (1); // не воспринимать длинные пакеты

    vol_length_opt = count_rx_ppp - 10;
    count_length_opt = 0;
    ind_opt = 7;

    ln_rejc_ipcp = 0;
    ln_ack_ipcp = 0;
    ln_nak_ipcp = 0;
    ln_rej_ipcp = 0;

    switch (buf_rx_ppp[3]) {
    case CONF_REQ:

      // (если вообще не те опции то сброс кода дать)
      // проверка предложенной конфигурации от сервера
      // если нужная конфигурация то событие RCR+
      // если нет то RCR-;

      for (i = 0; i < vol_length_opt; i++) {
        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + i];
        ln_ack_ipcp++;
      }
      IPCP_event = RCR_PL;
      i_scan_ipcp = buf_rx_ppp[4]; // присвоение идентификатора
      return (1);

      // Подтверждать все!!!!!!!!!!

    case CONF_ACK:
      // проверка опций (если вообще не те опции то сброс кода дать)
      // опции та, то событие RCA
      //  led2_on;    //пока
      //  lock_it();    // пока

      if (buf_rx_ppp[4] != i_scr_ipcp)
        return (1); // проверка идентификатора

      IPCP_event = RCA;
      i_scr_ipcp++; // приращение идентификатора
      return (1);
      // НИЧЕГО ДАЛЬШЕ НЕ РАЗБИРАТЬ!!!!!!!!!

    case CONF_NAK:
      // то RCN но все равно запрос тех же опций

      if (buf_rx_ppp[4] != i_scr_ipcp)
        return (1); // проверка идентификатора

    next_opt_ipcp_3:
      if (count_length_opt == vol_length_opt)
        goto end_ipcp_opt_3; // обработаны все опции

      switch (buf_rx_ppp[ind_opt]) {

      case IP_SELF:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          // for(i=3;i<=count_rx_ppp-4;i++){buf_rejc_opt_ipcp[ln_rejc_ipcp]=buf_rx_ppp[i];ln_rejc_ipcp++;}
          // IPCP_event=RUC;
          return (1);
        }

        ip_self[0] = buf_rx_ppp[ind_opt + 2];
        ip_self[1] = buf_rx_ppp[ind_opt + 3];
        ip_self[2] = buf_rx_ppp[ind_opt + 4];
        ip_self[3] = buf_rx_ppp[ind_opt + 5];

        fl_opt.ip_self = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_3;

      case IP_PRI_DNS:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          //   for(i=3;i<=count_rx_ppp-4;i++){buf_rejc_opt_ipcp[ln_rejc_ipcp]=buf_rx_ppp[i];ln_rejc_ipcp++;}
          //   IPCP_event=RUC;
          return (1);
        }

        ip_pri_dns[0] = buf_rx_ppp[ind_opt + 2];
        ip_pri_dns[1] = buf_rx_ppp[ind_opt + 3];
        ip_pri_dns[2] = buf_rx_ppp[ind_opt + 4];
        ip_pri_dns[3] = buf_rx_ppp[ind_opt + 5];

        fl_opt.ip_pri_dns = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_3;

      case IP_SEC_DNS:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          //  for(i=3;i<=count_rx_ppp-4;i++){buf_rejc_opt_ipcp[ln_rejc_ipcp]=buf_rx_ppp[i];ln_rejc_ipcp++;}
          //  IPCP_event=RUC;
          return (1);
        }

        ip_sec_dns[0] = buf_rx_ppp[ind_opt + 2];
        ip_sec_dns[1] = buf_rx_ppp[ind_opt + 3];
        ip_sec_dns[2] = buf_rx_ppp[ind_opt + 4];
        ip_sec_dns[3] = buf_rx_ppp[ind_opt + 5];
        fl_opt.mru = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_3;

      default: // for(i=3;i<=count_rx_ppp-4;i++){buf_rejc_opt_ipcp[ln_rejc_ipcp]=buf_rx_ppp[i];ln_rejc_ipcp++;}
               // IPCP_event=RUC;
        return (1);
      }

    end_ipcp_opt_3:
      i_scr_ipcp++; // приращение идентификатора
      IPCP_event = RCN;
      return (1);

    case CONF_REJ:
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[4] != i_scr_ipcp)
        return (1); // проверка идентификатора
      i_scr_ipcp++; // приращение идентификатора
      IPCP_event = RCN;

      return (1);
    }

    break;

  case PR_IP:

    if (count_rx_ppp < (33 + MIN_TR_HEAD))
      return (1);

    Crc_out.bytes[0] = buf_rx_ppp[12]; // проверка CRC IP
    Crc_out.bytes[1] = buf_rx_ppp[13];
    buf_rx_ppp[12] = 0;
    buf_rx_ppp[13] = 0;
    temp.word = calc_crc_ip(&buf_rx_ppp[2], 20);
    if (Crc_out.word != temp.word)
      return (1);

    if ((buf_rx_ppp[18] != ip_self[0]) || // сверка со своим IP
        (buf_rx_ppp[19] != ip_self[1]) || (buf_rx_ppp[20] != ip_self[2]) ||
        (buf_rx_ppp[21] != ip_self[3]))
      return (1);

    if (buf_rx_ppp[11] != 0x11)
      return (1); // сверка с протоколом UDP

    Crc_out.bytes[0] = buf_rx_ppp[28]; // проверка CRC UDP
    Crc_out.bytes[1] = buf_rx_ppp[29];
    buf_rx_ppp[28] = 0;
    buf_rx_ppp[29] = 0;
    temp.bytes[1] = buf_rx_ppp[26];
    temp.bytes[0] = buf_rx_ppp[27];
    temp.word = calc_crc_udp_2(&buf_rx_ppp[0], temp.word);
    if (Crc_out.word != temp.word)
      return (1);

    Crc_out.bytes[1] = port_udp >> 8;
    Crc_out.bytes[0] = port_udp;
    if ((buf_rx_ppp[24] != Crc_out.bytes[1]) || // проверка UDP порта
        (buf_rx_ppp[25] != Crc_out.bytes[0]))
      return (1);

    if (proc_udp_data(buf_rx_ppp, count_rx_ppp) == 0)
      return (1);

    break;
  }

  return (1);
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void send_no_sinc(unsigned char id, unsigned int dst, unsigned char error) {
  if (fl_ip.act_ip_end != 1)
    return;
  fl_ip.act_ip_end = 0;
  clr_cntr_nat();
  clr_cntr_link();
  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER2;     // версия
  Obj_ppp_tx.type_pac = OTV;     // тип пакета
  Obj_ppp_tx.num_src = num_self; // номер отправителя
  Obj_ppp_tx.num_dst = dst;      // номер получателя

  Obj_ppp_tx.id_pac = id;            // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций

  Obj_ppp_tx.l_opt = 3; // длина буфера опций
  buf_opt_tr[0] = KOD_OP_ERROR;
  buf_opt_tr[1] = 3;
  buf_opt_tr[2] = error;

  // Obj_ppp_tx.kol_opt=1;// количество опций

  Obj_ppp_tx.l_data = 0; // длина данных
  Obj_ppp_tx.p_data =
      &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  S2_RD;
  UDR0 = buf_tx_232[0];
  if (Regim == RG_DEBAG)
    UDR3 = buf_tx_232[0];
  return;
}

unsigned char proc_paging(void) {
  if (fl_pg_out.hold_pg == 1) {

    if ((fl_pg_out.first_pg == 1) && (cnt_pg == 0)) {
      cnt_pg = 0;
      fl_pg_out.first_pg = 0;
      fl_pg_out.end_pg = 0;
      fl_pg_out.hold_pg = 0;
      fl_pg_out.ch_crc = 0;
      fl_pg_out.cor_pg = 0;
      // дать ошибку рассинхронизация
      return (1);
    }

    if ((fl_pg_out.first_pg == 1) &&
        (cnt_pg != 0)) { // это в случае когда процесс прерван а потом заново
                         // возобновляется
      fl_pg_out.hold_pg = 1;
      fl_pg_out.first_pg = 0;
      cnt_pg = 0;
      id_pg_kp = id_pg_appl + 1;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    }

    if (id_pg_kp == id_pg_appl) {
      cnt_pg++;
      id_pg_kp++;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    } else {
      fl_pg_out.cor_pg = 0;
      if (id_pg_appl == (id_pg_kp - 1))
        goto check_end_pg;
      else {
        cnt_pg = 0;
        fl_pg_out.first_pg = 0;
        fl_pg_out.end_pg = 0;
        fl_pg_out.hold_pg = 0;
        fl_pg_out.ch_crc = 0;
        fl_pg_out.cor_pg = 0;
        // дать ошибку рассинхронизация
        return (1);
      }
    }

  } else {
    if (fl_pg_out.first_pg == 1) {
      fl_pg_out.hold_pg = 1;
      fl_pg_out.first_pg = 0;
      cnt_pg = 0;
      id_pg_kp = id_pg_appl + 1;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    } else {
      cnt_pg = 0;
      fl_pg_out.first_pg = 0;
      fl_pg_out.end_pg = 0;
      fl_pg_out.hold_pg = 0;
      fl_pg_out.ch_crc = 0;
      fl_pg_out.cor_pg = 0;
      // дать ошибку рассинхронизация
      return (1);
    }
  }

check_end_pg:

  if (fl_pg_out.end_pg == 1) {
    fl_pg_out.hold_pg = 0;
    fl_pg_out.first_pg = 0;
  }

  return (0);
}

unsigned char proc_option(unsigned char *buf_rx_ppp, unsigned int ind,
                          unsigned int count_rx_opt) {

  unsigned char tt;
  // return 0 ошибка, игнорировать, весь некорректный пакет
  //  return 1 - прочитаны все опции (при поступившем запросе) дальше выход и
  //  выдача ответ на запрос return 2 - ответить сразу размером страницы
  unsigned char cnt = 0;

next_option:

  switch (buf_rx_ppp[ind]) {
  case KOD_OP_ERROR:

    if (buf_rx_ppp[ind + 1] != L_ERR_NO_INTABL)
      return (0);
    if (buf_rx_ppp[ind + 2] != ERR_NO_INTABL)
      return (0);
    if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != KVIT)
      return (0);
    if (*(unsigned int *)&buf_rx_ppp[ind + 3] == num_seq_cl) {

      Appl_seq.cnt_cntr_cl = 0;
      Appl_seq.cntr_cl = TRUE;
      Appl_seq.en_cntr_cl = TRUE;
      Appl_seq.cnt_try = 0;
      fl_appl_seq.en_povtor = 0;
      Appl_seq.l_data = 0;
      Appl_seq.cnt_waits = 0;
      fl_appl_seq.kv_waits = 0;
    }

    cnt_no_link = vol_cnt_no_link; // dobavka

    Control.link_waits = FALSE;
    Control.cnt_link = Control.vol_link;
    Control.link = FALSE;

    Control.cnt_try = 0;
    Control.link_no = FALSE;
    return (0);

  case KOD_OP_LUFKIN:
    if (buf_rx_ppp[ind + 1] != L_OP_LUFKIN)
      return (0);
    if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
      return (0);
    delay_pre_tx = *(unsigned int *)&buf_rx_ppp[ind + 2];
    if (delay_pre_tx > 5000) {
      delay_pre_tx = 0;
      return (0);
    } // 5 секунд
    return (1);

  case KOD_OP_SIZE_PG:
    if (buf_rx_ppp[ind + 1] != L_OP_SIZE_PG)
      return (0);
    if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
      return (0);
    return (2);

  case KOD_WR_PG:
    if (buf_rx_ppp[ind + 1] != L_OP_WR_PG)
      return (0);
    if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
      return (0);
    if (buf_rx_ppp[ind + 2] & FIRST_PG)
      fl_pg_out.first_pg = 1;
    else
      fl_pg_out.first_pg = 0;
    if (buf_rx_ppp[ind + 2] & NO_END_PG)
      fl_pg_out.end_pg = 0;
    else
      fl_pg_out.end_pg = 1;
    if (buf_rx_ppp[ind + 2] & NADO_KV)
      fl_pg_out.send_kv = 1;
    else
      fl_pg_out.send_kv = 0;
    id_pg_appl = buf_rx_ppp[ind + 3];
    break;

  case KOD_WR_PG_CRC:

    fl_pg_out.ch_crc = 0;
    if (buf_rx_ppp[ind + 1] != L_OP_WR_PG_CRC)
      return (0);
    if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
      return (0);
    fl_pg_out.ch_crc = 1;
    pg_crc = *(unsigned int *)&buf_rx_ppp[ind + 2];
    break;

  default:
    return (0);
  }

  // здесь высчитывается длина заполнителя
  tt = 4 - ((buf_rx_ppp[ind + 1] + 4) % 4);
  if (tt == 4)
    tt = 0;
  ///////////////

  cnt = cnt + buf_rx_ppp[ind + 1] + tt;
  if (cnt >= count_rx_opt)
    return (1); // все опции просчитаны

  ind = ind + buf_rx_ppp[ind + 1] + tt;
  // if(ind>=count_rx_opt)return(1);
  goto next_option;
}

void send_size_pg(unsigned char id, unsigned int dst) {
  if (fl_ip.act_ip_end != 1)
    return;
  fl_ip.act_ip_end = 0;
  clr_cntr_nat();
  clr_cntr_link();
  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER2;     // версия
  Obj_ppp_tx.type_pac = OTV;     // тип пакета
  Obj_ppp_tx.num_src = num_self; // номер отправителя
  Obj_ppp_tx.num_dst = dst;      // номер получателя

  Obj_ppp_tx.id_pac = id;            // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций

  Obj_ppp_tx.l_opt = 4; // длина буфера опций
  buf_opt_tr[0] = KOD_OP_SIZE_PG;
  buf_opt_tr[1] = L_OP_SIZE_PG_OTV;
  *(unsigned int *)&buf_opt_tr[2] = 256;

  // Obj_ppp_tx.kol_opt=1;// количество опций

  Obj_ppp_tx.l_data = 0; // длина данных
  Obj_ppp_tx.p_data =
      &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  S2_RD;
  UDR0 = buf_tx_232[0];
  if (Regim == RG_DEBAG)
    UDR3 = buf_tx_232[0];
  return;
}

/*
 void send_otv_ping(unsigned char id,unsigned int dst)
    {
       if(fl_ip.act_ip_end!=1)return;
          fl_ip.act_ip_end=0;
          clr_cntr_nat();
          clr_cntr_link();
          Obj_ppp_tx.prozr=FALSE;
          Obj_ppp_tx.version=VER2;      //версия
          Obj_ppp_tx.type_pac=OTV;     //тип пакета
          Obj_ppp_tx.num_src=num_self; // номер отправителя
          Obj_ppp_tx.num_dst=dst; // номер получателя

          Obj_ppp_tx.id_pac=id; // идентификатор пакета

          Obj_ppp_tx.kol_opt=0;// количество опций
          Obj_ppp_tx.l_opt=0;// длина буфера опций

          Obj_ppp_tx.l_data=0;   // длина данных


         form_buf_tx_ppp();
         UCSR0A=UCSR0A | TXC;
         UCSR0B=UCSR0B | TXEN;
         UCSR0B=UCSR0B | TXCIE;
         if(check_cts()==1) return;
         S2_RD;
         UDR0=buf_tx_232[0];
         if(Regim==RG_DEBAG) UDR3=buf_tx_232[0];
         return;
    }

 */

unsigned char proc_udp_data(unsigned char *buf_rx_ppp,
                            unsigned int count_rx_ppp) {
  unsigned int length, crc;
  // unsigned char *p_data;
  unsigned char length_head;
  unsigned char offset, i;
  // unsigned int ii;

  // проверка длины данных UDP
  length = *(unsigned int *)&buf_rx_ppp[UDP_LN];
  if (length < MIN_TR_HEAD)
    return (0);

  // проверка длины транспорта
  length = *(unsigned int *)&buf_rx_ppp[TR_LEN];
  if (length < MIN_TR_HEAD)
    return (0);
  if (length > count_rx_ppp - 5 - 20 - 8)
    return (0);

  transform_buf(&buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_LEN] - 7,
                *(unsigned int *)&buf_rx_ppp[TR_KOD]);

  *(unsigned int *)&buf_rx_ppp[TR_KOD] = 0;

  crc = *(unsigned int *)&buf_rx_ppp[TR_CRC];
  *(unsigned int *)&buf_rx_ppp[TR_CRC] = 0;

  // проверка crc
  if (crc !=
      crc_m1(&buf_rx_ppp[TR_SRC], *(unsigned int *)&buf_rx_ppp[TR_LEN], 0xffff))
    return (0);

  if ((buf_rx_ppp[TR_V] >> 6) == VER2) // версия протокола VER2
  {

    if (*(unsigned int *)&buf_rx_ppp[TR_DST] != num_self)
      return (0);

    length_head = (buf_rx_ppp[TR_V] & 0x0f) * 4;

    if (length_head < MIN_TR_HEAD)
      return (0);
    if (length_head > 50)
      return (0); // пока ограничил

    length = count_rx_ppp - 5 - 20 - 8 - length_head;

    if (length == 0) // если длина данных 0 то контроль связи и проверка
    {

      if (length_head > MIN_TR_HEAD) {
        if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD) ==
            2) {
          send_size_pg(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        }
        return (0);
      }

      /*
      if(((buf_rx_ppp[TR_V]>>4)&0x03)==ZAPR)
      {
        send_otv_ping(buf_rx_ppp[TR_ID],*(unsigned int*)&buf_rx_ppp[TR_SRC]);
        send_info(sizeof(ans_out_ping),ans_out_ping,1,buf_rx_ppp[TR_ID]);
        return(0);
      }

      */

      if (*(unsigned int *)&buf_rx_ppp[TR_SRC] != NUM_SRV_LINK)
        return (0);
      //                  if(buf_rx_ppp[TR_ID]!=ID_CC)return(0);

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != KVIT)
        return (0);

      send_info(sizeof(rec_ctrl_ch), rec_ctrl_ch, 1, buf_rx_ppp[TR_ID]);

      if (Control.link_waits == TRUE) {
        cnt_no_link = vol_cnt_no_link; // dobavka
        Control.link_waits = FALSE;
        Control.cnt_link = Control.vol_link;
        Control.cnt_try = 0;
        Control.link_no = FALSE;
        return (1);
      }
      return (0);
    }

    // дальнейший разбор

    clr_cntr_nat();

    offset = 2 + 20 + 8 + length_head;

    if (buf_rx_ppp[offset + SL_LS_PROT] == PROT_SL_LS) {
      // разбор команд
      // пока одна команда - наличие отсутствие клиента в таблице маршрутизации
      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != OTV)
        return (0);
      if (buf_rx_ppp[offset + SL_LS_PROT + 1] != COM_CNTR_CL)
        return (0);
      if (*(unsigned int *)&buf_rx_ppp[offset + SL_LS_PROT + 2] !=
          L_OTV_CNTR_CL)
        return (0);
      if (*(unsigned int *)&buf_rx_ppp[offset + SL_LS_PROT + 4] != num_seq_cl)
        return (0);
      if (buf_rx_ppp[offset + SL_LS_PROT + 6] != 0) { // присутствие в таблице

        Appl_seq.en_cntr_cl = FALSE;
        Appl_seq.cntr_cl = FALSE;
        Appl_seq.cnt_cntr_cl = 0;

        // присутствие в таблице
      }
      cnt_no_link = vol_cnt_no_link; // dobavka
      Control.link_waits = FALSE;
      Control.cnt_link = Control.vol_link;
      Control.link = FALSE;
      Control.cnt_try = 0;
      Control.link_no = FALSE;

      send_info(sizeof(ans_in_ctrl_cl), ans_in_ctrl_cl, 1, buf_rx_ppp[TR_ID]);
      return (1);
    }

    if (buf_rx_ppp[offset + C1_PROT] != PROT_C1)
      return (0);

    switch (buf_rx_ppp[offset + C1_PORT]) {
    case PORT485_1:

      if (length < 11) {
        // можно отправиь сообщение об ошибке "некорректные данные"
        send_info(sizeof(ans_out_485_1), ans_out_485_1, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_1, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (length > LN_BUF_485_1 + 10) {
        // можно отправиь сообщение об ошибке "слишком длинные данные для
        // приложения"
        send_info(sizeof(ans_out_485_1), ans_out_485_1, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_1, RS_OVER_BUF_TX, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      // здесь должна быть проверка контекста, если все нормально то дальше
      // забит контекст то можно послать служебное сообщение

      if (check_cont_485_1(&buf_rx_ppp[offset + C1_CONT]) == 1) {
        send_info(sizeof(ans_out_485_1), ans_out_485_1, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_1, RS_NO_CONT, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (Appl_RS485_1.fl_data_buf == TRUE) {
        // то можно послать служебное сообщение что буфер переполнен
        send_info(sizeof(ans_out_485_1), ans_out_485_1, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_1, RS_BUSY, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        break;
      }

      if (Regim == RG_DEBAG) { // включен тестовый режим!!!
        send_info(sizeof(ans_out_485_1), ans_out_485_1, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_1, RS_TEST, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      Appl_RS485_1.pre_tx = 0;
      if (length_head > MIN_TR_HEAD) {
        if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD) != 1)
          return (0);
        Appl_RS485_1.pre_tx = delay_pre_tx; // задержка перед передачей
      }

      if (fl_485_1.busy == 1)
        Appl_RS485_1.fl_data_buf = TRUE;
      else
        fl_485_1.tx = 1;
      Appl_RS485_1.dst_buf = *(unsigned int *)&buf_rx_ppp[TR_SRC];
      Appl_RS485_1.id_buf = buf_rx_ppp[TR_ID];

      for (i = 0; i < 8; i++)
        Appl_RS485_1.cont_buf[i] = buf_rx_ppp[offset + C1_CONT + i];

      Appl_RS485_1.ln_data_buf = length - 10;
      for (i = 0; i < Appl_RS485_1.ln_data_buf; i++)
        Appl_RS485_1_data_buf[i] = buf_rx_ppp[offset + C1_DATA_RS + i];

      break;
    case PORT485_2:

      if (length < 11) {
        // можно отправиь сообщение об ошибке "некорректные данные"
        send_info(sizeof(ans_out_485_2), ans_out_485_2, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_2, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (length > LN_BUF_485_2 + 10) {
        // можно отправиь сообщение об ошибке "слишком длинные данные для
        // приложения"
        send_info(sizeof(ans_out_485_2), ans_out_485_2, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_2, RS_OVER_BUF_TX, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      // здесь должна быть проверка контекста, если все нормально то дальше
      // забит контекст то можно послать служебное сообщение
      if (check_cont_485_1(&buf_rx_ppp[offset + C1_CONT]) == 1) {
        send_info(sizeof(ans_out_485_2), ans_out_485_2, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_2, RS_NO_CONT, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (Appl_RS485_2.fl_data_buf == TRUE) {
        // то можно послать служебное сообщение что буфер переполнен
        send_info(sizeof(ans_out_485_2), ans_out_485_2, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_2, RS_BUSY, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        break;
      }

      if (Regim == RG_DEBAG) { // включен тестовый режим!!!
        send_info(sizeof(ans_out_485_2), ans_out_485_2, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS485_2, RS_TEST, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      Appl_RS485_2.pre_tx = 0;
      if (length_head > MIN_TR_HEAD) {
        if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD) != 1)
          return (0);
        Appl_RS485_2.pre_tx = delay_pre_tx; // задержка перед передачей
      }

      if (fl_485_2.busy == 1)
        Appl_RS485_2.fl_data_buf = TRUE;
      else
        fl_485_2.tx = 1;
      Appl_RS485_2.dst_buf = *(unsigned int *)&buf_rx_ppp[TR_SRC];
      Appl_RS485_2.id_buf = buf_rx_ppp[TR_ID];
      for (i = 0; i < 8; i++)
        Appl_RS485_2.cont_buf[i] = buf_rx_ppp[offset + C1_CONT + i];
      Appl_RS485_2.ln_data_buf = length - 10;
      for (i = 0; i < Appl_RS485_2.ln_data_buf; i++)
        Appl_RS485_2_data_buf[i] = buf_rx_ppp[offset + C1_DATA_RS + i];

      break;

    case PORT232_2:

      if (length < 11) {
        // можно отправиь сообщение об ошибке "некорректные данные"
        send_info(sizeof(ans_out_232), ans_out_232, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS232_2, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (length > LN_BUF_232_2 + 10) {
        // можно отправиь сообщение об ошибке "слишком длинные данные для
        // приложения"
        send_info(sizeof(ans_out_232), ans_out_232, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS232_2, RS_OVER_BUF_TX, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      // здесь должна быть проверка контекста, если все нормально то дальше
      // забит контекст то можно послать служебное сообщение
      if (check_cont_485_1(&buf_rx_ppp[offset + C1_CONT]) == 1) {
        send_info(sizeof(ans_out_232), ans_out_232, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS232_2, RS_NO_CONT, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (Appl_RS232_2.fl_data_buf == TRUE) {
        // то можно послать служебное сообщение что буфер переполнен
        send_info(sizeof(ans_out_232), ans_out_232, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS232_2, RS_BUSY, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        break;
      }

      if (Regim == RG_DEBAG) { // включен тестовый режим!!!
        send_info(sizeof(ans_out_232), ans_out_232, 1, buf_rx_ppp[TR_ID]);
        send_err485(NUM_RS232_2, RS_TEST, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      Appl_RS232_2.pre_tx = 0;
      if (length_head > MIN_TR_HEAD) {
        if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD) != 1)
          return (0);
        Appl_RS232_2.pre_tx = delay_pre_tx; // задержка перед передачей
      }

      if (fl_232_2.busy == 1)
        Appl_RS232_2.fl_data_buf = TRUE;
      else
        fl_232_2.tx = 1;
      Appl_RS232_2.dst_buf = *(unsigned int *)&buf_rx_ppp[TR_SRC];
      Appl_RS232_2.id_buf = buf_rx_ppp[TR_ID];
      for (i = 0; i < 8; i++)
        Appl_RS232_2.cont_buf[i] = buf_rx_ppp[offset + C1_CONT + i];
      Appl_RS232_2.ln_data_buf = length - 10;
      for (i = 0; i < Appl_RS232_2.ln_data_buf; i++)
        Appl_RS232_2_data_buf[i] = buf_rx_ppp[offset + C1_DATA_RS + i];

      break;

    case PORT_SEQ:
      if (length < 5)
        return (0);
      if (*(unsigned int *)&buf_rx_ppp[TR_SRC] != num_seq_cl)
        return (0);

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) == KVIT) {
        if (buf_rx_ppp[offset + C1_DATA] != KVITOK_SEQ)
          return (0);
        if (length != 5)
          return (0);
        if (*(unsigned int *)&buf_rx_ppp[offset + C1_DATA + 1] != L_KVITOK)
          return (0);
      }

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) == ZAPR) {
        if (buf_rx_ppp[offset + C1_DATA] != ZAPR_TC)
          return (0);
        if (length != 5)
          return (0);
        if (*(unsigned int *)&buf_rx_ppp[offset + C1_DATA + 1] != L_ZAPR_TC)
          return (0);

        if (fl_ip.act_ip_end != 1)
          return (0);
        fl_ip.act_ip_end = 0;
        clr_cntr_nat();
        clr_cntr_link();
        Obj_ppp_tx.prozr = FALSE;
        Obj_ppp_tx.version = VER2;     // версия
        Obj_ppp_tx.type_pac = OTV;     // тип пакета
        Obj_ppp_tx.num_src = num_self; // номер отправителя
        Obj_ppp_tx.num_dst =
            *(unsigned int *)&buf_rx_ppp[TR_SRC]; // номер получателя
        Obj_ppp_tx.id_pac = buf_rx_ppp[TR_ID];    // идентификатор пакета
        Obj_ppp_tx.p_opt = &buf_opt_tr[0];        // //указатель буфера опций
        Obj_ppp_tx.l_opt = 0;                     // длина буфера опций
        buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
        buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_SEQ;
        buf_tx_232[TR_OP_DATA + C1_DATA] = SEQ_STATE;
        *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1] = L_STATE;
        *(unsigned long int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 3] =
            burst_ds_r();

        if (struct_tcc1.type == TC_SHL)
          buf_tx_232[TR_OP_DATA + C1_DATA + 8] = state_seq;
        else
          buf_tx_232[TR_OP_DATA + C1_DATA + 7] = DES_SEQ;

        if (Appl_seq_des != 0)
          buf_tx_232[TR_OP_DATA + C1_DATA + 7] = DES_SEQ;

        if ((struct_tcc1.type == TC_SHL) || (struct_tcc1.type == TC_SUHOI))
          buf_tx_232[TR_OP_DATA + C1_DATA + 8] = sv1.tc & 0x01;
        else {
          buf_tx_232[TR_OP_DATA + C1_DATA + 7] = DES_SEQ;
          buf_tx_232[TR_OP_DATA + C1_DATA + 8] = DES_SEQ;
        }

        if ((struct_tcc2.type == TC_SHL) || (struct_tcc2.type == TC_SUHOI))
          buf_tx_232[TR_OP_DATA + C1_DATA + 9] = sv2.tc & 0x01;
        else
          buf_tx_232[TR_OP_DATA + C1_DATA + 9] = DES_SEQ;

        /*

           if(Appl_seq_des==0)
            {
          //   buf_tx_232[TR_OP_DATA+C1_DATA+7]=state_seq; !!!!!!!!!!!!!!!!!!!!
          //   buf_tx_232[TR_OP_DATA+C1_DATA+8]=sv_tc.bytes[0] &
          0x01;!!!!!!!!!!!!!!!!!!!
             buf_tx_232[TR_OP_DATA+C1_DATA+9]=byte_state_net & 0x01;
             }
             else buf_tx_232[TR_OP_DATA+C1_DATA+7]=DES_SEQ;

           */

        Obj_ppp_tx.l_data = 5 + L_STATE; // длина данных
        Obj_ppp_tx.p_data =
            &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

        send_info(sizeof(ans_out_st_contr), ans_out_st_contr, 1,
                  buf_rx_ppp[TR_ID]);

        form_buf_tx_ppp();
        UCSR0A = UCSR0A | TXC;
        UCSR0B = UCSR0B | TXEN;
        UCSR0B = UCSR0B | TXCIE;
        if (check_cts() == 1)
          return (1);
        S2_RD
        UDR0 = buf_tx_232[0];
        if (Regim == RG_DEBAG)
          UDR3 = buf_tx_232[0];
        return (1);
      }

      if (fl_appl_seq.kv_waits == 1) {
        if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != KVIT)
          return (0);
        if (Appl_seq.id != buf_rx_ppp[TR_ID])
          return (0); // проверка идентификатора

        Appl_seq.cnt_waits = 0;
        fl_appl_seq.kv_waits = 0;

        cnt_no_link = vol_cnt_no_link; // dobavka
        Control.link_waits = FALSE;
        Control.cnt_link = Control.vol_link;
        Control.cnt_try = 0;
        Control.link_no = FALSE;

        fl_appl_seq.en_povtor = 0;
        // КВИТИРОВАНИЕ
        Appl_seq.p_out = Appl_seq.p_out_kv;
        Appl_seq.l_data = 0;
        if (fl_appl_seq1.over_buf == 1) {
          fl_appl_seq1.over_buf = 0;
          fl_appl_seq1.send_state = 1;
        }
        send_info(sizeof(rec_evc), rec_evc, 1, buf_rx_ppp[TR_ID]);
        return (1);
      }

      break;
    case PORT_SYS:
      break;
    case PORT_CONF:
      if (length < 5)
        return (0);
      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
        return (0);

      if (check_ln_conf(&buf_rx_ppp[0], offset, count_rx_ppp) == 1)
        return (0);

      if ((buf_rx_ppp[offset + C1_DATA] & 0x7f) == CONF_LOG) {
        if (length > 5)
          return (0);
        if (*(unsigned int *)&buf_rx_ppp[offset + C1_DATA + 1] != L_CONF_LOG)
          return (0);
        if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
          // стирание лог файла
          __watchdog_reset();
          WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);
          __watchdog_reset();
          point_log_buf = 0;
          *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1] =
              L_CONF_LOG + 1;
          Obj_ppp_tx.l_data = 5 + L_CONF_LOG + 1;
          //  ii=0;
        } else {
          // чтение
          RdFromFleshToArr(BEG_BUF_LOG, &buf_tx_232[TR_OP_DATA + C1_DATA + 4],
                           (L_LOG * 6));
          *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1] =
              L_CONF_LOG + (L_LOG * 6) + 1;
          Obj_ppp_tx.l_data = 5 + L_CONF_LOG + (L_LOG * 6) + 1;
          // ii=;
        }

        buf_tx_232[TR_OP_DATA + C1_DATA] = buf_rx_ppp[offset + C1_DATA];
        buf_tx_232[TR_OP_DATA + C1_DATA + 3] = 0x00; // OK
        goto otv_conf;
      }

      Obj_ppp_tx.l_data = proc_config(&buf_rx_ppp[0], offset, count_rx_ppp);
      Obj_ppp_tx.l_data = Obj_ppp_tx.l_data + 2;

    otv_conf:
      clr_cntr_nat();
      if (fl_ip.act_ip_end != 1)
        return (0);
      fl_ip.act_ip_end = 0;
      clr_cntr_link();
      Obj_ppp_tx.prozr = FALSE;
      Obj_ppp_tx.version = VER2;     // версия
      Obj_ppp_tx.type_pac = OTV;     // тип пакета
      Obj_ppp_tx.num_src = num_self; // номер отправителя
      Obj_ppp_tx.num_dst =
          *(unsigned int *)&buf_rx_ppp[TR_SRC]; // номер получателя
      Obj_ppp_tx.id_pac = buf_rx_ppp[TR_ID];    // идентификатор пакета
      Obj_ppp_tx.p_opt = &buf_opt_tr[0];        // //указатель буфера опций
      Obj_ppp_tx.l_opt = 0;                     // длина буфера опций
      Obj_ppp_tx.kol_opt = 0;                   // количество опций

      buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
      buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_CONF;

      Obj_ppp_tx.p_data =
          &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

      send_info(sizeof(ans_out_config), ans_out_config, 1, buf_rx_ppp[TR_ID]);
      form_buf_tx_ppp();

      if ((fl_rewrite.udp == 1) || (fl_rewrite.udp == 1) ||
          (fl_rewrite.num_self == 1)) {
        RdFromFleshToArr(A_IP_PAR, &buf_rx_ppp[0], L_IP_PAR);
        if (fl_rewrite.num_self == 1)
          num_self = *(unsigned int *)&buf_rx_ppp[OFS_NUM];
        if (fl_rewrite.udp == 1)
          port_udp = *(unsigned int *)&buf_rx_ppp[OFS_PORT];
        if (fl_rewrite.ip == 1) {
          ip_ls[0] = buf_rx_ppp[OFS_IP];
          ip_ls[1] = buf_rx_ppp[OFS_IP + 1];
          ip_ls[2] = buf_rx_ppp[OFS_IP + 2];
          ip_ls[3] = buf_rx_ppp[OFS_IP + 3];
        }
        fl_rewrite.ip = 0;
        fl_rewrite.udp = 0;
        fl_rewrite.num_self = 0;
        prov_ozu = crc_ozu();

        Control.cnt_link = 3;
        Control.link_waits = FALSE;
        Control.cnt_try = 0;
        Control.link_no = FALSE;
      }

      UCSR0A = UCSR0A | TXC;
      UCSR0B = UCSR0B | TXEN;
      UCSR0B = UCSR0B | TXCIE;
      if (check_cts() == 1)
        return (1);
      S2_RD
      UDR0 = buf_tx_232[0];
      if (Regim == RG_DEBAG)
        UDR3 = buf_tx_232[0];

      return (1);

      // break;

    case PORT_PROG:

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != ZAPR)
        return (0);

      if (length_head == MIN_TR_HEAD)
        return (0);
      if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD) != 1)
        return (0);

      if (length != 5 + L_COM_CODE_MEM_Z)
        return (0);
      if ((buf_rx_ppp[offset + C1_PORT + 1]) != COM_CODE_MEM)
        return (0);
      if ((*(unsigned int *)&buf_rx_ppp[offset + C1_PORT + 2]) !=
          L_COM_CODE_MEM_Z)
        return (0);

      if (proc_paging() == 1) {
        send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                     NO_SINC_PG);
        return (0);
      }

      if (cnt_pg > 251) //(от 0 до 251) //можно грузить не более 252 стр
      {
        cnt_pg = 0;
        fl_pg_out.send_crc = 0;
        fl_pg_out.hold_pg = 0;
        fl_pg_out.ch_crc = 0;
        fl_pg_out.end_pg = 0;
        fl_pg_out.cor_pg = 0;
        send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                     OVER_PG);
        return (0);
      }

      reboot_byte = 0;

      if (fl_pg_out.cor_pg == 1) {
        fl_pg_out.cor_pg = 0;

        // произвести запись страницы
        pagee = cnt_pg;
        pagee = pagee << 8;
        __disable_interrupt();
        __watchdog_reset();
        off_buf = offset + C1_PORT + 4;
        RAMPZ = 1;
        array_in_232 = &buf_rx_ppp[0];
        contr_gluk1 = C_GLUK1;
        contr_gluk2 = C_GLUK2;
        boot();

        contr_gluk1 = 0;
        contr_gluk2 = 0;
        RAMPZ = 0;
        __watchdog_reset();
        __enable_interrupt();

        if (fl_pg_out.end_pg == 1) {

          total_kol_page = cnt_pg + 1;
          fl_pg_out.end_pg = 0;
          // fl_pg_out.yes_crc=0; //обнулить флаг перезагрузки
          if (fl_pg_out.ch_crc == 1) {

            // здесь проверка crc всего куска

            length = 0xffff;
            __disable_interrupt();

            for (i = 0; i < total_kol_page; i++) {
              array_in_232 = &buf_rx_ppp[offset + C1_PORT + 4];
              r_p_flash(i * 256);
              length = crc_m1(&buf_rx_ppp[offset + C1_PORT + 4], 256, length);
              __watchdog_reset();
            }
            __enable_interrupt();

            if (length == pg_crc) {
              // fl_pg_out.yes_crc=1;
              // выставить флаг перезагрузки буферов
              reboot_byte = 0x55;
            } else {
              total_kol_page = 0;
              reboot_byte = 0;
            }

            fl_pg_out.send_crc = 1;
          }

          fl_pg_out.hold_pg = 0;
          fl_pg_out.ch_crc = 0;
        }
      }

      // fl_pg_out.yes_crc=0;

      if (fl_pg_out.send_kv == 1) {
        // выдать квиток с учетом записи опции crc записанного куска

        if (reboot_byte != 0x55) {
          if (fl_ip.act_ip_end != 1) {
            fl_pg_out.send_crc = 0;
            reboot_byte = 0;
            return (0);
          }
        }

        fl_ip.act_ip_end = 0;
        clr_cntr_nat();
        clr_cntr_link();
        Obj_ppp_tx.prozr = FALSE;
        Obj_ppp_tx.version = VER2;     // версия
        Obj_ppp_tx.type_pac = OTV;     // тип пакета
        Obj_ppp_tx.num_src = num_self; // номер отправителя
        Obj_ppp_tx.num_dst =
            *(unsigned int *)&buf_rx_ppp[TR_SRC]; // номер получателя

        Obj_ppp_tx.id_pac = buf_rx_ppp[TR_ID]; // идентификатор пакета
        Obj_ppp_tx.p_opt = &buf_opt_tr[0];     // //указатель буфера опций
        Obj_ppp_tx.l_opt = 0;

        if (fl_pg_out.send_crc == 1) {
          Obj_ppp_tx.l_opt = 4; // длина буфера опций
          buf_opt_tr[0] = KOD_WR_PG_CRC;
          buf_opt_tr[1] = L_OP_WR_PG_CRC;
          *(unsigned int *)&buf_opt_tr[2] = length; // это crc
        }
        fl_pg_out.send_crc = 0;
        // Obj_ppp_tx.kol_opt=1;// количество опций

        buf_tx_232[TR_OP_DATA + C1_PROT + 10] = PROT_C1;
        buf_tx_232[TR_OP_DATA + C1_PROT + 11] = PORT_PROG;
        buf_tx_232[TR_OP_DATA + C1_PROT + 12] = COM_CODE_MEM;
        *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_PROT + 13] =
            L_COM_CODE_MEM_A;
        buf_tx_232[TR_OP_DATA + C1_PROT + 15] = 0x00;                     // OK
        *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_PROT + 16] = cnt_pg; // OK

        Obj_ppp_tx.l_data = 5 + L_COM_CODE_MEM_A; // длина данных
        Obj_ppp_tx.p_data =
            &buf_tx_232[TR_OP_DATA + C1_PROT + 10]; // указатель буфера данных

        send_info(sizeof(ans_out_prog), ans_out_prog, 1, buf_rx_ppp[TR_ID]);

        form_buf_tx_ppp();
        UCSR0A = UCSR0A | TXC;
        UCSR0B = UCSR0B | TXEN;
        UCSR0B = UCSR0B | TXCIE;

        if (reboot_byte != 0x55) {
          // if(check_cts()==1){S5_RD; while(1)__watchdog_reset(); return(0);}
          if (check_cts() == 1)
            return (0);
        }

        S2_RD;
        UDR0 = buf_tx_232[0];
        if (Regim == RG_DEBAG)
          UDR3 = buf_tx_232[0];
      }

      if (reboot_byte == 0x55) {
        for (i = 0; i < 30; i++) {
          __watchdog_reset();
          delay(45000);
        }

        __disable_interrupt();
        WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);

        off_buf = 0;
        SREG = 0;
        WDTCSR = 0x18;
        WDTCSR = 0x0f;
        __watchdog_reset();
        contr_gluk1 = C_GLUK1;
        contr_gluk2 = C_GLUK2;
        send_info(sizeof(evc_rld), evc_rld, 0, 0);
        reload_apl();
      }

      return (1);
    }

  } // для версии VER2

  return (0);
}

////////////////EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

void analiz_ppp_rx(void) {

  if (fl_ipcp.act_ipcp_end == 0)
    return;
  if (fl_ip.act_ip_end == 0)
    return;

  if (Buf1_rx_ppp.rec == TRUE) {

    proc_ppp_packet(&Buf1_rx_ppp.data[0], Buf1_rx_ppp.ln_data);
    Buf1_rx_ppp.ln_data = 0;
    Buf1_rx_ppp.rec = FALSE;
    Buf1_rx_ppp.busy = FALSE;
    return;
  }

  if (Buf2_rx_ppp.rec == TRUE) {

    proc_ppp_packet(&Buf2_rx_ppp.data[0], Buf2_rx_ppp.ln_data);
    Buf2_rx_ppp.ln_data = 0;
    Buf2_rx_ppp.rec = FALSE;
    Buf2_rx_ppp.busy = FALSE;
    return;
  }
}
