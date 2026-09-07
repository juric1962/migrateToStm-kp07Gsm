#include "bpa.h"
#include "def_at.h"
#include "def_conf.h"
#include "def_link.h"
#include "def_log.h"
#include "def_prot.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "map_ef.h"
#include "map_mbus.h"
#include "ozu_map.h"
#include "ver_api.h"
#include <inavr.h>
#include <iom2560.h>

// адоптированный обрезанный стек PPP 14.09.07

#define C_GLUK1 0x55
#define C_GLUK2 0xaa

extern unsigned char k_a_tit[2];
extern union {
  unsigned char mb[SEG3 * 2]; // байтовый массив
} c2_byte;                    // 1я страница конфигурации

extern union {
  unsigned char mb[SEG99 * 2]; // байтовый массив
} c1_byte;                     // страница конфигурации ГЗУ

unsigned int swap(unsigned int a) { return (a << 8) | (a >> 8); }

extern unsigned char obmen_full;
extern char bit_registr1;
extern unsigned char nomer_tab;
extern unsigned int sost;
extern char zadan_ys;

char read_ohr(void);

void test_vzat(void);
void test_snat(void);
void test_alarm(void);
void test_t_vhod(void);
void test_t_vihod(void);

void refresh_a(char m);
void RdFromFleshToArrInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                         unsigned int num);
void WrArrayToFleshInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                       unsigned int num, unsigned char flag,
                       unsigned int znach); // запись конфигурации кп во флеш

unsigned char crc_485(unsigned char num, unsigned char *p);

extern unsigned char bufer[2];

void lock_it(void);
extern void write_log_info(unsigned char sost, unsigned char mesto);

extern void set_rlt(unsigned char address, unsigned char *data);

void transform_buf(unsigned char *p, unsigned int kol_byte,
                   unsigned int s_rand);

void boot_HUGE(void);
void r_p_flash(unsigned int adress);
void reload_apl_HUGE(void);

void send_info(char size, char __flash *p, unsigned char fl_id,
               unsigned char id);

//__flash char gluk[]= {'g','l','u','k'};
//__flash char gluk1[]= {'g','l','u','k','1'};

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
__flash char ans_out_mbus[] = {'A', 'N', 'S', '-', '>', 'm', 'b', 'u', 's'};

//__flash char ans_out_ping[]={'A','N','S','-','>','p','i','n','g'};

//__flash char prov1[]={'P','R','O','V','1'};
//__flash char prov2[]={'P','R','O','V','2'};

#define NO_MODEL 0x2
#define NO_MAP 0x7
#define DATA_MONTH 0x06
#define DATA_DATA 0x1a
#define GOD 0x11

__flash unsigned char new_version[] = {0x06,       0xcd,      0x00, 0x07,
                                       0x00,       NO_MODEL,  0x00, NO_MAP,
                                       DATA_MONTH, DATA_DATA, 0x00, GOD};

extern unsigned int ust_vhod;

extern unsigned int c_config_tc[SEG8];

extern unsigned char sel_modul;

extern char bit_level, bit_level_psm, sost_psm;
extern unsigned long int unix;

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

unsigned int proc_modbus(unsigned char *pnt_buf, unsigned char length);
unsigned char proc_kamintel(unsigned char *pnt_buf, unsigned char length);

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

unsigned char compress_off;

extern unsigned long int cnt_outcom, cnt_incom;

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

struct {
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
char ip_change;
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

extern unsigned int func_crc_tii(void);
extern __no_init unsigned int crc_tii;
extern __no_init unsigned int cnt_tii[8];

extern unsigned char cnt_tu1, cnt_tu2;

extern unsigned int modbus_mem1[SEG1];

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

// extern unsigned char byte_state_net;

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

unsigned int delay_pre_tx;

//!!!!!!!!!!!!!!!11удаленная загрузка
__no_init unsigned int pagee @0xe60;
__no_init unsigned char *array_in_232 @0xe62;
__no_init unsigned char off_buf @0xe6a;
__no_init unsigned int total_kol_page @0xe6b; // счетчик страниц словный
__no_init unsigned char contr_gluk1 @0xe6d;
__no_init unsigned char contr_gluk2 @0xe6e;

#pragma dataseg = MY_STACK1
__no_init unsigned char saver_global_var[2];
#pragma dataseg = default

/*
// компилятор не поддерживает ICALL вызовы
// придется писать программу на ассмблере
// надо выцарапать данные с дальней области
  __farfunc char proba_1(char a, char b) @ "BOOTLOADER"
{
  return(a+b);
}
*/

void move_to_saver(void);
void pop_from_saver(void);

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
unsigned int cnt_pg_i;  //  счетчик страниц
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

void com_disp_mbus(void);

unsigned long int hash_tabletka2(void) {
  int point_pos;

  unsigned long int hash_kod;
  union {
    unsigned long int kod;
    unsigned char b_kod[4];
  } hash;

  //  proba_1(0,0);
  hash_kod = 0;

  for (point_pos = 0; point_pos < (4 * 253); point_pos = point_pos + 4) {

    RdFromFleshToArr(A_SEG6 + point_pos, &hash.b_kod[0], 4);
    hash_kod = hash_kod ^ hash.kod;
  }
  return (hash_kod);
}

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

void send_terminate_lcp(void)

{
  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp;

  fl_lcp.act_lcp_end = 0;
  buf_tx_232[0] = 0x7e;
  buf_tx_232[1] = 0xff;
  buf_tx_232[2] = 0x03;
  buf_tx_232[3] = 0xc0;
  buf_tx_232[4] = 0x21;

  buf_tx_232[5] = TERM_REQ;
  buf_tx_232[6] = 0;

  buf_tx_232[7] = 0;
  buf_tx_232[8] = 0x04;

  temp.word =
      pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
  buf_tx_232[9] = temp.bytes[0];
  buf_tx_232[10] = temp.bytes[1];

  buf_tx_232[11] = 0x7e;

  count_tx_ppp = 0;
  vol_tx_ppp = buf_tx_232[8] + 8;

  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  S2_RD;
  UDR0 = buf_tx_232[0];
  if (Regim == RG_DEBAG)
    UDR3 = buf_tx_232[0];
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

    //           compress_off=1;

    if (compress_off == 0) {
      buf_tx_232[7] = 0;
      buf_tx_232[8] = 0x0e;
    } else {
      buf_tx_232[7] = 0;
      buf_tx_232[8] = 0x0a;
    }

    buf_tx_232[9] = 2;
    buf_tx_232[10] = 6;

    buf_tx_232[11] = 0;
    buf_tx_232[12] = 0x0a;
    // buf_tx_232[12]=0;
    buf_tx_232[13] = 0;
    buf_tx_232[14] = 0;

    if (compress_off == 0) {

      buf_tx_232[15] = 7;
      buf_tx_232[16] = 2;
      buf_tx_232[17] = 8;
      buf_tx_232[18] = 2;
      temp.word =
          pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
      buf_tx_232[19] = temp.bytes[0];
      buf_tx_232[20] = temp.bytes[1];
      buf_tx_232[21] = 0x7e;

    }

    else {

      temp.word =
          pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
      buf_tx_232[15] = temp.bytes[0];
      buf_tx_232[16] = temp.bytes[1];
      buf_tx_232[17] = 0x7e;
    }

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

  unsigned char k;
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

    if (compress_off == 1) {
      for (k = 0; k < (vol_tx_ppp - 1); k++)
        buf_tx_232[vol_tx_ppp + 1 - k] = buf_tx_232[vol_tx_ppp - 1 - k];
      buf_tx_232[1] = 0xff;
      buf_tx_232[2] = 0x03;
      temp.word = temp.word + 2;
      vol_tx_ppp = temp.word + 4;
    }

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

    if (compress_off == 0) {
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

      vol_tx_ppp = buf_tx_232[6] + 6;
    }

    else {
      buf_tx_232[1] = 0xff;
      buf_tx_232[2] = 0x03;
      buf_tx_232[3] = 0x80;
      buf_tx_232[4] = 0x21;

      buf_tx_232[5] = CONF_REQ;
      buf_tx_232[6] = i_scr_ipcp;

      buf_tx_232[7] = 0;
      buf_tx_232[8] = 10; // без DNS

      //  buf_tx_232[6]=22;//c DNS

      buf_tx_232[9] = 3; // запрос IP контроллера
      buf_tx_232[10] = 6;
      buf_tx_232[11] = ip_self[0];
      buf_tx_232[12] = ip_self[1];
      buf_tx_232[13] = ip_self[2];
      buf_tx_232[14] = ip_self[3];

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
          pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
      buf_tx_232[15] = temp.bytes[0]; // без DNS
      buf_tx_232[16] = temp.bytes[1]; // без DNS

      buf_tx_232[17] = 0x7e; // без DNS
      //  buf_tx_232[25]=temp.bytes[0];//c DNS
      //  buf_tx_232[26]=temp.bytes[1]; //c DNS
      //  buf_tx_232[27]=0x7e; //c DNS

      vol_tx_ppp = buf_tx_232[8] + 8;
    }

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

    if (compress_off == 1) {
      for (i = 0; i < (vol_tx_ppp - 1); i++)
        buf_tx_232[vol_tx_ppp + 1 - i] = buf_tx_232[vol_tx_ppp - 1 - i];
      buf_tx_232[1] = 0xff;
      buf_tx_232[2] = 0x03;
      temp.word = temp.word + 2;
      vol_tx_ppp = temp.word + 4;
    }

    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;

    /*
    buf_tx_232[7+ln_ack_ipcp]=temp.bytes[0];
    buf_tx_232[8+ln_ack_ipcp]=temp.bytes[1];
    buf_tx_232[9+ln_ack_ipcp]=0x7e;
    */

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

      if (compress_off == 1) {
        for (i = 0; i < (vol_tx_ppp - 1); i++)
          buf_tx_232[vol_tx_ppp + 1 - i] = buf_tx_232[vol_tx_ppp - 1 - i];
        buf_tx_232[1] = 0xff;
        buf_tx_232[2] = 0x03;
        temp.word = temp.word + 2;
        vol_tx_ppp = temp.word + 4;
      }

      temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;

      /*
          buf_tx_232[7+ln_rej_ipcp]=temp.bytes[0];
          buf_tx_232[8+ln_rej_ipcp]=temp.bytes[1];
          buf_tx_232[9+ln_rej_ipcp]=0x7e;
      */

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

  if (compress_off == 1) {
    if ((buf_rx_ppp[1] == 0xff) && (buf_rx_ppp[2] == 0x03) &&
        (buf_rx_ppp[3] == 0xc0) && (buf_rx_ppp[4] == 0x23) &&
        (layer_PPP == LAYER_PAP)) {
      rcvd_protokol = PR_PAP;
      for (i = 0; i < (count_rx_ppp - 3); i++)
        buf_rx_ppp[1 + i] = buf_rx_ppp[3 + i];
      count_rx_ppp = count_rx_ppp - 2;
    }

    if ((buf_rx_ppp[1] == 0xff) && (buf_rx_ppp[2] == 0x03) &&
        (buf_rx_ppp[3] == 0x80) && (buf_rx_ppp[4] == 0x21) &&
        ((layer_PPP == LAYER_IP) || (layer_PPP == LAYER_IPCP))) {
      rcvd_protokol = PR_IPCP;
      for (i = 0; i < (count_rx_ppp - 3); i++)
        buf_rx_ppp[1 + i] = buf_rx_ppp[3 + i];
      count_rx_ppp = count_rx_ppp - 2;
    }

    if ((buf_rx_ppp[1] == 0xff) && (buf_rx_ppp[2] == 0x03) &&
        (buf_rx_ppp[3] == 0x00) && (buf_rx_ppp[4] == 0x21) &&
        (layer_PPP == LAYER_IP)) {
      rcvd_protokol = PR_IP;
      for (i = 0; i < (count_rx_ppp - 4); i++)
        buf_rx_ppp[1 + i] = buf_rx_ppp[4 + i];
      count_rx_ppp = count_rx_ppp - 3;
    }

  } else {
    if ((buf_rx_ppp[1] == 0xc0) && (buf_rx_ppp[2] == 0x23) &&
        (layer_PPP == LAYER_PAP))
      rcvd_protokol = PR_PAP;
    if ((buf_rx_ppp[1] == 0x80) && (buf_rx_ppp[2] == 0x21) &&
        ((layer_PPP == LAYER_IP) || (layer_PPP == LAYER_IPCP)))
      rcvd_protokol = PR_IPCP;
    if ((buf_rx_ppp[1] == 0x21) && (layer_PPP == LAYER_IP))
      rcvd_protokol = PR_IP;
  }

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

      if ((buf_rx_ppp[9] == 7) && (buf_rx_ppp[10] == 2) &&
          (buf_rx_ppp[11] == 8) && (buf_rx_ppp[12] == 2))
        compress_off = 1;

      return (1);
    case CONF_REJ:
      // !!!!! подумать!!!!!!!!
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      i_scr_lcp++;  // приращение идентификатора
      LCP_event = RCN;
      if ((buf_rx_ppp[9] == 7) && (buf_rx_ppp[10] == 2) &&
          (buf_rx_ppp[11] == 8) && (buf_rx_ppp[12] == 2))
        compress_off = 1;
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

        //  {modbus_mem1[0x15f-0x150]++;}

        if (buf_rx_ppp[ind_opt + 2] != ip_self[0])
          ip_change = 1;
        if (buf_rx_ppp[ind_opt + 3] != ip_self[1])
          ip_change = 1;
        if (buf_rx_ppp[ind_opt + 4] != ip_self[2])
          ip_change = 1;
        if (buf_rx_ppp[ind_opt + 5] != ip_self[3])
          ip_change = 1;

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

    // 10.11.2010
    count_rx_ppp = buf_rx_ppp[4]; // обрезание в РРР мусора, длина складывается
                                  // из длины IP пакета и +5(начало и конец РРР)
    count_rx_ppp = (count_rx_ppp << 8) | buf_rx_ppp[5]; //
    count_rx_ppp = count_rx_ppp + 5;                    //

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
    // send_info(sizeof(gluk),gluk,1,3);

    if ((fl_pg_out.first_pg == 1) && (cnt_pg == 0)) {
      //      send_info(sizeof(gluk),gluk,1,4);
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
                         //        send_info(sizeof(gluk),gluk,1,5);
      fl_pg_out.hold_pg = 1;
      fl_pg_out.first_pg = 0;
      cnt_pg = 0;
      id_pg_kp = id_pg_appl + 1;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    }

    if (id_pg_kp == id_pg_appl) {
      //        send_info(sizeof(gluk),gluk,1,6);
      cnt_pg++;
      id_pg_kp++;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    } else {
      //       send_info(sizeof(gluk),gluk,1,7);
      fl_pg_out.cor_pg = 0;
      if (id_pg_appl == (id_pg_kp - 1))
        goto check_end_pg;
      else {
        //         send_info(sizeof(gluk),gluk,1,8);
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

    //  send_info(sizeof(gluk),gluk,1,9);
    if (fl_pg_out.first_pg == 1) {
      //         send_info(sizeof(gluk),gluk,1,10);
      fl_pg_out.hold_pg = 1;
      fl_pg_out.first_pg = 0;
      cnt_pg = 0;
      id_pg_kp = id_pg_appl + 1;
      fl_pg_out.cor_pg = 1;
      goto check_end_pg;
    } else {
      //       send_info(sizeof(gluk),gluk,1,11);
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

  //   send_info(sizeof(gluk),gluk,1,12);
  if (fl_pg_out.end_pg == 1) {
    //        send_info(sizeof(gluk),gluk,1,13);
    fl_pg_out.hold_pg = 0;
    fl_pg_out.first_pg = 0;
  }

  // send_info(sizeof(gluk),gluk,1,14);
  return (0);
}

unsigned char proc_option(unsigned char *buf_rx_ppp, unsigned int ind,
                          unsigned int count_rx_opt, unsigned char port) {

  unsigned char tt;
  // return 0 ошибка, игнорировать, весь некорректный пакет
  //  return 1 - прочитаны все опции (при поступившем запросе) дальше выход и
  //  выдача ответ на запрос return 2 - ответить сразу размером страницы return
  //  3 - ошибка время жизни истекло return 4 - ошибка операция недоступна

  unsigned char flag_life;
  unsigned int life_cl;
  unsigned char life_time;
  unsigned char life_start;
  unsigned int cnt;

  cnt = 0;
  flag_life = 0;
  life_cl = 0;
  life_time = 0;
  life_start = 0;

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
    break;

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
    else {
      fl_pg_out.end_pg = 1;
    }
    if (buf_rx_ppp[ind + 2] & NADO_KV) {
      fl_pg_out.send_kv = 1;
    } else
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

    //               send_info(sizeof(gluk),gluk,1,2);

    break;

  case KOD_OP_LIFE:
    switch (port) {
    case PORT485_1:
      life_time = Life.time_485_1;
      life_cl = Life.client_485_1;
      life_start = Life.start_485_1;
      break;
    case PORT485_2:
      life_time = Life.time_485_2;
      life_cl = Life.client_485_2;
      life_start = Life.start_485_2;
      break;
    case PORT232_2:
      life_time = Life.time_232_2;
      life_cl = Life.client_232_2;
      life_start = Life.start_232_2;
      break;
    default:
      return (0);
    }

    if (buf_rx_ppp[ind + 2] == 0) {

      if (life_cl == *(unsigned int *)&buf_rx_ppp[TR_SRC]) {
        if (life_time == 0)
          flag_life = 2;
        else {
          switch (port) {
          case PORT485_1:
            Life.time_485_1 = 0;
            Life.start_485_1 = 0;
            break;
          case PORT485_2:
            Life.time_485_2 = 0;
            Life.start_485_2 = 0;
            break;
          case PORT232_2:
            Life.time_232_2 = 0;
            Life.start_232_2 = 0;
            break;
          default:
            write_log_info(ST_ERROR, ERR6);
            lock_it(); // сбой озу
          }
          flag_life = 1;
        }
      } else
        flag_life = 3;
    } else {
      if (life_time == 0) {
        switch (port) {
        case PORT485_1:
          Life.time_485_1 = buf_rx_ppp[ind + 2];
          Life.client_485_1 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
          Life.start_485_1 = 1;
          break;
        case PORT485_2:
          Life.time_485_2 = buf_rx_ppp[ind + 2];
          Life.client_485_2 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
          Life.start_485_2 = 1;
          break;
        case PORT232_2:
          Life.time_232_2 = buf_rx_ppp[ind + 2];
          Life.client_232_2 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
          Life.start_232_2 = 1;
          break;
        default:
          write_log_info(ST_ERROR, ERR5);
          lock_it(); // сбой озу
        }
        flag_life = 1;
      } else {

        if ((life_cl == *(unsigned int *)&buf_rx_ppp[TR_SRC]) ||
            (life_start == 1)) {
          switch (port) {
          case PORT485_1:
            Life.time_485_1 = buf_rx_ppp[ind + 2];
            Life.client_485_1 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
            Life.start_485_1 = 1;
            break;
          case PORT485_2:
            Life.time_485_2 = buf_rx_ppp[ind + 2];
            Life.client_485_2 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
            Life.start_485_2 = 1;
            break;
          case PORT232_2:
            Life.time_232_2 = buf_rx_ppp[ind + 2];
            Life.client_232_2 = *(unsigned int *)&buf_rx_ppp[TR_SRC];
            Life.start_232_2 = 1;
            break;
          default:
            write_log_info(ST_ERROR, ERR5);
            lock_it(); // сбой озу
          }
          flag_life = 1;
        } else
          flag_life = 3;
      }
    }

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
  if (cnt >= count_rx_opt) {
    if (flag_life == 0) {
      return (1);
    } // все опции просчитаны
    else {
      if (flag_life == 1)
        return (1);
      if (flag_life == 2)
        return (3);
      if (flag_life == 3)
        return (4);
      write_log_info(ST_ERROR, ERR4);
      lock_it(); // сбой озу
    }
  }

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
  unsigned int length, crc, year;
  // unsigned char *p_data;
  unsigned char length_head;
  unsigned char offset, i;
  unsigned int ii, kol, k;
  unsigned int *tc_cfgPoint;
  unsigned char flag_tii, flag_time, flag_refresh;
  unsigned long int old_unix;
  unsigned char flashka[2];

  struct {
    unsigned char *point_out;
    unsigned char *point_out_save;
    unsigned char *point_in;
    unsigned char flag_elam;
    unsigned char smest_to_elam;
    unsigned int count_in_elam;
    unsigned int index_elam;
    unsigned int size_of_pac;
    unsigned int count_out_elam;
    unsigned int modbus_crc_new;
    unsigned char input_tu;
    unsigned int overall;

  } mobus;

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

    //      send_info(sizeof(prov1),prov1,0,0);

    length = count_rx_ppp - 5 - 20 - 8 - length_head;

    if (length == 0) // если длина данных 0 то контроль связи и проверка
    {

      if (length_head > MIN_TR_HEAD) {
        if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD,
                        PORT_NONE) == 2) {
          send_size_pg(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        }
        return (0);
      }

      //                  send_info(sizeof(prov2),prov2,0,0);

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
        i = proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD,
                        PORT485_1);

        if (i == 3) // время жизни истекло
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       OUT_LIFE);
          return (0);
        }

        if (i == 4) // операция недоступна
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       DES_OPERATE);
          return (0);
        }

        if (i != 1)
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
        i = proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD,
                        PORT485_2);

        if (i == 3) // время жизни истекло
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       OUT_LIFE);
          return (0);
        }

        if (i == 4) // операция недоступна
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       DES_OPERATE);
          return (0);
        }

        if (i != 1)
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
        i = proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD,
                        PORT232_2);

        if (i == 3) // время жизни истекло
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       OUT_LIFE);
          return (0);
        }

        if (i == 4) // операция недоступна
        {
          send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                       DES_OPERATE);
          return (0);
        }

        if (i != 1)
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
      // if(*(unsigned int*)&buf_rx_ppp[TR_SRC]!=num_seq_cl)return(0);

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) == KVIT) {
        if (*(unsigned int *)&buf_rx_ppp[TR_SRC] != num_seq_cl)
          return (0);
        if (buf_rx_ppp[offset + C1_DATA] != KVITOK_SEQ)
          return (0);
        if (length != 5)
          return (0);
        if (*(unsigned int *)&buf_rx_ppp[offset + C1_DATA + 1] != L_KVITOK)
          return (0);

        if (fl_appl_seq.kv_waits == 1) {
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
        } else
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

      /*
       if(fl_appl_seq.kv_waits==1)
           {
            if(((buf_rx_ppp[TR_V]>>4)&0x03)!=KVIT)return(0);
            if(Appl_seq.id!=buf_rx_ppp[TR_ID])return(0);// проверка
       идентификатора


            Appl_seq.cnt_waits=0;
            fl_appl_seq.kv_waits=0;

            cnt_no_link=vol_cnt_no_link;//dobavka
            Control.link_waits=FALSE;
            Control.cnt_link=Control.vol_link;
            Control.cnt_try=0;
            Control.link_no=FALSE;

            fl_appl_seq.en_povtor=0;
            // КВИТИРОВАНИЕ
            Appl_seq.p_out=Appl_seq.p_out_kv;
            Appl_seq.l_data=0;
            if(fl_appl_seq1.over_buf==1)
               {
               fl_appl_seq1.over_buf=0;
               fl_appl_seq1.send_state=1;
               }
            send_info(sizeof(rec_evc),rec_evc,1,buf_rx_ppp[TR_ID]);
            return(1);
            }
         */

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

      if ((fl_rewrite.ip == 1) || (fl_rewrite.udp == 1) ||
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
      if (proc_option(buf_rx_ppp, TR_OP_DATA, length_head - MIN_TR_HEAD,
                      PORT_PROG) != 1)
        return (0);

      if (length != 5 + L_COM_CODE_MEM_Z)
        return (0);
      if ((buf_rx_ppp[offset + C1_PORT + 1]) != COM_CODE_MEM)
        return (0);
      if ((*(unsigned int *)&buf_rx_ppp[offset + C1_PORT + 2]) !=
          L_COM_CODE_MEM_Z)
        return (0);

      //                     send_info(sizeof(gluk),gluk,1,20);

      if (proc_paging() == 1) {
        //                            send_info(sizeof(gluk),gluk,1,21);
        // прошивка не соответствует устройству !!  25 12 2021
      device_bad:
        send_no_sinc(buf_rx_ppp[TR_ID], *(unsigned int *)&buf_rx_ppp[TR_SRC],
                     NO_SINC_PG);
        return (0);
      }

      // if(cnt_pg>251)    //(от 0 до 251) //можно грузить не более 252 стр
      if (cnt_pg >= 505) //(от 0 до 251) //можно грузить не более 252 стр
      {
        //                             send_info(sizeof(gluk),gluk,1,22);
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

      //                           send_info(sizeof(gluk),gluk,1,23);
      reboot_byte = 0;

      if (fl_pg_out.cor_pg == 1) {
        fl_pg_out.cor_pg = 0;
        // проверка на соответствие прошивки в конце страницы должен быть код
        // 0x55aa
        //              if ( ( cnt_pg==0) &&     ( *(unsigned
        //              int*)&buf_rx_ppp[offset+C1_PORT+4 + 0xFE] != 0x55AA ) )
        //              goto device_bad;
        if ((cnt_pg == 0) &&
            (buf_rx_ppp[offset + C1_PORT + 4 + 0xFF] != VERSION_API))
          goto device_bad;

        //                              send_info(sizeof(gluk),gluk,1,24);

        // произвести запись страницы
        pagee = cnt_pg;
        pagee = pagee << 8;
        __disable_interrupt();
        __watchdog_reset();
        off_buf = offset + C1_PORT + 4;

        if (cnt_pg >= 256)
          RAMPZ = 3;
        else
          RAMPZ = 2; // 18 05 2022
        array_in_232 = &buf_rx_ppp[0];
        contr_gluk1 = C_GLUK1;
        contr_gluk2 = C_GLUK2;
        EIND = 1;
        move_to_saver();
        boot_HUGE();
        pop_from_saver();

        contr_gluk1 = 0;
        contr_gluk2 = 0;
        RAMPZ = 0;
        __watchdog_reset();
        __enable_interrupt();

        if (fl_pg_out.end_pg == 1) {
          //                                 send_info(sizeof(gluk),gluk,1,25);

          total_kol_page = cnt_pg + 1;
          fl_pg_out.end_pg = 0;
          // fl_pg_out.yes_crc=0; //обнулить флаг перезагрузки
          if (fl_pg_out.ch_crc == 1) {
            //                                       send_info(sizeof(gluk),gluk,1,26);

            // здесь проверка crc всего куска

            length = 0xffff;
            __disable_interrupt();

            i = 0;
            for (cnt_pg_i = 0; cnt_pg_i < total_kol_page;
                 cnt_pg_i++) // слово !!!
            {
              array_in_232 = &buf_rx_ppp[offset + C1_PORT + 4];
              if (cnt_pg_i >= 256)
                RAMPZ = 3;
              else
                RAMPZ = 2; // 18 05 2022

              move_to_saver();
              r_p_flash(i * 256); //
              i++;
              pop_from_saver();
              RAMPZ = 0; // 18 05 2022

              length = crc_m1(&buf_rx_ppp[offset + C1_PORT + 4], 256, length);
              __watchdog_reset();
            }
            __enable_interrupt();

            if (length == pg_crc) {
              //                                       send_info(sizeof(gluk),gluk,1,27);
              // fl_pg_out.yes_crc=1;
              // выставить флаг перезагрузки буферов
              reboot_byte = 0x55;
            } else {
              //                                       send_info(sizeof(gluk),gluk,1,28);
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
        //                              send_info(sizeof(gluk),gluk,1,29);

        if (reboot_byte != 0x55) {
          if (fl_ip.act_ip_end != 1) {
            fl_pg_out.send_crc = 0;
            reboot_byte = 0;
            return (0);
          }
        }

        //                            send_info(sizeof(gluk),gluk,1,30);

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
          //                               send_info(sizeof(gluk),gluk,1,31);
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

        crc_tii = 0;
        for (i = 0; i < 8; i++)
          cnt_tii[i] = 0;

        __disable_interrupt();
        WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);

        //  *(unsigned long int*)&buf_rx_ppp[0]=burst_ds_r();
        RdFromFleshToArr(A_TRAF, &buf_rx_ppp[0], 4);
        *(unsigned long int *)&buf_rx_ppp[4] = cnt_outcom;
        *(unsigned long int *)&buf_rx_ppp[8] = cnt_incom;
        *(unsigned int *)&buf_rx_ppp[L_TRAF - 2] =
            crc_m1(&buf_rx_ppp[0], L_TRAF - 2, 0xffff);
        WrArrayToFlesh(A_TRAF, &buf_rx_ppp[0], L_TRAF, 0, 0);

        off_buf = 0;
        SREG = 0;
        WDTCSR = 0x18;
        WDTCSR = 0x0f;
        __watchdog_reset();
        contr_gluk1 = C_GLUK1;
        contr_gluk2 = C_GLUK2;
        send_info(sizeof(evc_rld), evc_rld, 0, 0);
        EIND = 1;
        reload_apl_HUGE();
      }
      return (1);

    case PORT_MBUS:
      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) == ZAPR) {
        // это ограничение связано с модбасом для ELAM оно не актуально

        // 08 11 2019    if(length>259)return(0);

        //////////////////////каминтел

        //     if(sel_modul==1)
        {
          buf_tx_232[TR_OP_DATA + C1_DATA] =
              buf_rx_ppp[offset + C1_DATA + 8]; // функция
          buf_tx_232[TR_OP_DATA + C1_DATA + 1] =
              buf_rx_ppp[offset + C1_DATA + 9]; // адрес
          ii = proc_kamintel(&buf_rx_ppp[offset + C1_DATA + 8], length - 10);
          if (ii == 1) {

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
            Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций
            Obj_ppp_tx.l_opt = 0;              // длина буфера опций
            Obj_ppp_tx.kol_opt = 0;            // количество опций
            buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
            buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_MBUS;
            Obj_ppp_tx.l_data = Obj_ppp_tx.l_data + 2;
            goto metka_send_mbus;
          }
        }
        ///////////////////////// конец каминтел

        //     if((length<18)||(length>259))return(0);

        if (length < 18)
          return (0);
        /////07 11 2019 процедура проверки пакета на модбас не имеет смысла
        /*
        ii=proc_modbus(&buf_rx_ppp[offset+C1_DATA+8],length-10);
        if(ii==0)return(0);
        */

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
        Obj_ppp_tx.kol_opt = 0;                   // количество опций
        buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
        buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_MBUS;

        // buf_tx_232[TR_OP_DATA+C1_DATA]=buf_rx_ppp[offset+C1_DATA+8];//адрес
        // !!! оставляем
        // buf_tx_232[TR_OP_DATA+C1_DATA+1]=buf_rx_ppp[offset+C1_DATA+9];//функция
        // !!! оставляем

        // crc=swap(*(unsigned int*)&buf_rx_ppp[offset+C1_DATA+10]);//здесь
        // временно старт адрес kol=swap(*(unsigned
        // int*)&buf_rx_ppp[offset+C1_DATA+12]); //здесь временно количество
        // регистров

        // Инициализация указателей
        mobus.point_out = &buf_tx_232[TR_OP_DATA + C1_DATA]; // выходной буфер
        mobus.point_in =
            &buf_rx_ppp[offset + C1_DATA + 8]; // входной буфер с модбас пакетом
        mobus.index_elam =
            0; // индекс очередного инкапсулированного елам пакета
        mobus.flag_elam = 0;
        mobus.count_in_elam = length - 10;
        mobus.count_out_elam = 0;
        mobus.overall = TR_OP_DATA + C1_DATA + 100;

        //+=3+(kol_word*2)+2+(smest_to_elam*2);

      //
      //
      //
      //
      next_elam:
        if ((*(mobus.point_in + mobus.index_elam) & 0xf8) != 0xf8)
          goto ne_elam;
        mobus.flag_elam = 1;
        switch (*(mobus.point_in + mobus.index_elam + 2)) {
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
          mobus.size_of_pac = 7;
          break;
        case 16:
          mobus.size_of_pac = 8 + *(mobus.point_in + mobus.index_elam + 7);
          break;
        default: {
          return (0);
        }
        }
        if (mobus.count_in_elam < (mobus.index_elam + mobus.size_of_pac + 2)) {
          goto FinitaPerformence;
        }

        // проверка фрагмента на CRC
        mobus.modbus_crc_new = crc_m1(mobus.point_in + mobus.index_elam,
                                      mobus.size_of_pac, 0xffff);
        if (mobus.modbus_crc_new !=
            (((unsigned int)(*(mobus.point_in + mobus.index_elam +
                               mobus.size_of_pac))
              << 8) +
             (unsigned int)(*(mobus.point_in + mobus.index_elam +
                              mobus.size_of_pac + 1)))) {
          return (0);
        }

        // zapoln_otvet_elam();
        goto Performence;
      ReEnter:

        mobus.index_elam = mobus.index_elam + mobus.size_of_pac + 2;
        if (mobus.count_in_elam > mobus.index_elam)
          goto next_elam;

        goto FinitaPerformence;

      ne_elam:
        if (mobus.flag_elam == 1) {
          return (0);
        } // выход с ошибкой
          // это одиночный пакет

        mobus.modbus_crc_new =
            crc_m1(mobus.point_in, mobus.count_in_elam - 2, 0xffff);

        if (mobus.modbus_crc_new !=
            (((unsigned int)(*(mobus.point_in + mobus.count_in_elam - 2))
              << 8) +
             (unsigned int)(*(mobus.point_in + mobus.count_in_elam - 1))))
          return (0);

        //    zapoln_otvet_elam();
        mobus.flag_elam = 0;
        goto Performence;

        // switch обработчик модбас фрагмента                         Начало
        // **************************************

      Performence:
        //          crc=swap(*(unsigned
        //          int*)&buf_rx_ppp[offset+C1_DATA+10]);//здесь временно старт
        //          адрес kol=swap(*(unsigned
        //          int*)&buf_rx_ppp[offset+C1_DATA+12]); //здесь временно
        //          количество регистров
        if (mobus.flag_elam == 1)
          mobus.smest_to_elam = 1;
        else
          mobus.smest_to_elam = 0;
        ii = *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam +
               1); // номер команды
        crc = ((unsigned int)*(mobus.point_in + mobus.index_elam +
                               mobus.smest_to_elam + 2)
               << 8) +
              (unsigned int)*(mobus.point_in + mobus.smest_to_elam +
                              mobus.index_elam + 3);
        kol = ((unsigned int)*(mobus.point_in + mobus.smest_to_elam +
                               mobus.index_elam + 4)
               << 8) +
              (unsigned int)*(mobus.point_in + mobus.smest_to_elam +
                              mobus.index_elam + 5);
        mobus.point_out_save = mobus.point_out; // сохранить указатель

        //**************************??????
        /*
                                       {
                                          send_err485(NUM_RS485_1,RS_DATA_ERR,buf_rx_ppp[TR_ID],*(unsigned
           int*)&buf_rx_ppp[TR_SRC]); return(0);  // выход по ошибке
                                        }
                                        */
        //**************************??????

        switch (ii) {
        case 1:

          if (mobus.flag_elam == 1)
            mobus.smest_to_elam = 1;
          else
            mobus.smest_to_elam = 0;

          // ожидаемое количество байт в выходной буфер
          mobus.overall += 4 + mobus.smest_to_elam + 2;
          if (mobus.overall > VOL_TX_PPP)
            return (0); // выход по ошибке
          // ожидаемое количество байт в выходной буфер

          *mobus.point_out++ = *(mobus.point_in + mobus.index_elam);
          if (mobus.flag_elam == 1)
            *mobus.point_out++ =
                *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam);
          *mobus.point_out++ = 1;
          *mobus.point_out++ = 1;
          mobus.input_tu = 0;
          if (crc == MB_TU1) {
            if (PORTA & IO11)
              mobus.input_tu = mobus.input_tu | 1;
            if (PORTA & IO12)
              mobus.input_tu = mobus.input_tu | 2;
          }

          if (crc == MB_TU2) {
            if (PORTA & IO12)
              mobus.input_tu = mobus.input_tu | 1;
          }

          *mobus.point_out++ = mobus.input_tu;
          Obj_ppp_tx.l_data = 4 + mobus.smest_to_elam;

          //  *(unsigned
          //  int*)&buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data]=crc_m1(&buf_tx_232[TR_OP_DATA+C1_DATA],Obj_ppp_tx.l_data,0xffff);
          mobus.modbus_crc_new = crc_m1(mobus.point_out - Obj_ppp_tx.l_data,
                                        Obj_ppp_tx.l_data, 0xffff);
          *mobus.point_out++ = mobus.modbus_crc_new >> 8;
          *mobus.point_out++ = mobus.modbus_crc_new;
          mobus.count_out_elam +=
              Obj_ppp_tx.l_data + 2; // плюс контрольная сумма

          break;

          /*  старый обработчик
             buf_tx_232[TR_OP_DATA+C1_DATA+2]=1;
             buf_tx_232[TR_OP_DATA+C1_DATA+3]=0;

             if (crc==MB_TU1)
             {
               if(PORTA & IO11)
             buf_tx_232[TR_OP_DATA+C1_DATA+3]=buf_tx_232[TR_OP_DATA+C1_DATA+3] |
             0x01; if(PORTA & IO12)
             buf_tx_232[TR_OP_DATA+C1_DATA+3]=buf_tx_232[TR_OP_DATA+C1_DATA+3] |
             0x02;
             }

             if (crc==MB_TU2)
             {
               if(PORTA & IO12)
             buf_tx_232[TR_OP_DATA+C1_DATA+3]=buf_tx_232[TR_OP_DATA+C1_DATA+3] |
             0x01;
             }

             Obj_ppp_tx.l_data=4;
             break;
            */

        case 3:
        case 4:
          if (mobus.flag_elam == 1)
            mobus.smest_to_elam = 1;
          else
            mobus.smest_to_elam = 0;
          // ожидаемое количество байт в выходной буфер
          mobus.overall += 3 + (kol * 2) + (mobus.smest_to_elam * 2) + 2;

          if (mobus.overall > VOL_TX_PPP) {
            send_err485(NUM_RS485_1, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                        *(unsigned int *)&buf_rx_ppp[TR_SRC]);
            return (0); // выход по ошибке
          }

          // ожидаемое количество байт в выходной буфер

          *mobus.point_out++ = *(mobus.point_in + mobus.index_elam);
          if (mobus.flag_elam == 1)
            *mobus.point_out++ =
                *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam);
          *mobus.point_out++ = ii;
          if (mobus.flag_elam == 1)
            *mobus.point_out++ = (kol * 2) >> 8;
          *mobus.point_out++ = (kol * 2);

          /////buf_tx_232[TR_OP_DATA+C1_DATA+2]=buf_rx_ppp[offset+C1_DATA+13] <<
          ///1;//byte_count

          *(unsigned long int *)&modbus_mem1[AD_TIME] = burst_ds_r();
          length = modbus_mem1[AD_TIME + 1];
          modbus_mem1[AD_TIME + 1] = modbus_mem1[AD_TIME];
          modbus_mem1[AD_TIME] = length;

          /////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          /////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          for (ii = 0; ii < kol; ii++) {
            // *(unsigned int*)&buf_tx_232[TR_OP_DATA+C1_DATA+3+(ii<<1)]=0;

            *(unsigned int *)(mobus.point_out + (ii << 1)) = 0;
            if ((crc + ii) == 0) // нулевой адрес там свои тс, для совместимости
            {
              *mobus.point_out++ = 0; //  buf_tx_232[TR_OP_DATA+C1_DATA+3]=0;
              *mobus.point_out++ =
                  (sv1.tc & 0x01) |
                  ((sv2.tc & 0x01) << 1); //  buf_tx_232[TR_OP_DATA+C1_DATA+4]=(sv1.tc
                                          //  & 0x01) | ((sv2.tc & 0x01)<<1);
            }

            if (((crc + ii) >= BEGIN_SEG7) && ((crc + ii) <= END_SEG7)) {

              old_unix = hash_tabletka2();
              for (i = 0; i < 12; i++)
                buf_tx_232[570 + i] = new_version[i];
              buf_tx_232[570 + i++] = old_unix;
              buf_tx_232[570 + i++] = old_unix >> 8;
              buf_tx_232[570 + i++] = old_unix >> 16;
              buf_tx_232[570 + i++] = old_unix >> 24;
              WrArrayToFlesh(A_SEG7, &buf_tx_232[570], 16, 0, 0);

              if ((crc + kol - 1) <= END_SEG7) {

                RdFromFleshToArr(A_SEG7 + (crc + ii - BEGIN_SEG7) * 2,
                                 &buf_tx_232[600], (kol - ii) * 2);
                // for(k=0;k<(kol-ii);k++) *(unsigned
                // int*)&buf_tx_232[TR_OP_DATA+C1_DATA+3+(ii<<1)+(k<<1)]=swap(*(unsigned
                // int*)&buf_tx_232[600+(k<<1)]);
                ///// for(k=0;k<(kol-ii);k++) *(unsigned
                ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+3+(ii<<1)+(k<<1)]=*(unsigned
                ///int*)&buf_tx_232[600+(k<<1)];

                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      *(unsigned int *)&buf_tx_232[600 + (k << 1)];
                ii = kol;
              }

              else {

                RdFromFleshToArr(A_SEG7 + (crc + ii - BEGIN_SEG7) * 2,
                                 &buf_tx_232[600],
                                 (END_SEG7 - (crc + ii) + 1) * 2);
                //  for(k=0;k<(END_SEG7-(crc+ii)+1);k++) *(unsigned
                //  int*)&buf_tx_232[TR_OP_DATA+C1_DATA+3+(ii<<1)+(k<<1)]=swap(*(unsigned
                //  int*)&buf_tx_232[600+(k<<1)]);
                /////  for(k=0;k<(END_SEG7-(crc+ii)+1);k++) *(unsigned
                ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+3+(ii<<1)+(k<<1)]=*(unsigned
                ///int*)&buf_tx_232[600+(k<<1)];

                for (k = 0; k < (END_SEG7 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      *(unsigned int *)&buf_tx_232[600 + (k << 1)];
                ii = END_SEG7 - crc;
              }
            }

            if (((crc + ii) >= BEGIN_SEG9) && ((crc + ii) <= END_SEG9)) {

              if ((crc + kol - 1) <= END_SEG9) {

                RdFromFleshToArr(A_SEG9 + (crc + ii - BEGIN_SEG9) * 2,
                                 &buf_tx_232[1000], (kol - ii) * 2);
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = kol;
              }

              else {

                RdFromFleshToArr(A_SEG9 + (crc + ii - BEGIN_SEG9) * 2,
                                 &buf_tx_232[1000],
                                 (END_SEG9 - (crc + ii) + 1) * 2);
                for (k = 0; k < (END_SEG9 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = END_SEG9 - crc;
              }
            }
            /// вставка ГЗУ 9/03/16
            if (((crc + ii) >= BEGIN_SEG99) && ((crc + ii) <= END_SEG99)) {

              if ((crc + kol - 1) <= END_SEG99) {

                RdFromFleshToArr(A_SEG99 + (crc + ii - BEGIN_SEG99) * 2,
                                 &buf_tx_232[1000], (kol - ii) * 2);
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = kol;
              }

              else {

                RdFromFleshToArr(A_SEG99 + (crc + ii - BEGIN_SEG99) * 2,
                                 &buf_tx_232[1000],
                                 (END_SEG99 - (crc + ii) + 1) * 2);
                for (k = 0; k < (END_SEG99 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = END_SEG99 - crc;
              }
            }
            ///

            if (((crc + ii) >= BEGIN_SEG1) && ((crc + ii) <= END_SEG1)) {
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(modbus_mem1[crc + ii - BEGIN_SEG1]);
              if ((crc + ii) == MB_IND_DNS) {
                if (modbus_mem1[AD_IND_DNS] == 0)
                  *(unsigned int *)(mobus.point_out + (ii << 1)) = swap(99);
                else
                  *(unsigned int *)(mobus.point_out + (ii << 1)) =
                      swap(modbus_mem1[AD_IND_DNS] - 1);
              }
              ////
              if ((crc + ii) == MB_IND_GZU) {
                if (modbus_mem1[AD_IND_GZU] == 0)
                  *(unsigned int *)(mobus.point_out + (ii << 1)) = swap(54);
                else
                  *(unsigned int *)(mobus.point_out + (ii << 1)) =
                      swap(modbus_mem1[AD_IND_GZU] - 1);
              }
            }

            if ((crc + ii) == MB_SEC)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_sec);
            if ((crc + ii) == MB_MIN)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_min);
            if ((crc + ii) == MB_HR)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_hor);
            if ((crc + ii) == MB_DAY)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_date);
            if ((crc + ii) == MB_MONTH)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_month);
            if ((crc + ii) == MB_YEAR)
              *(unsigned int *)(mobus.point_out + (ii << 1)) =
                  swap(real_time.r_year);

            if (((crc + ii) >= BEGIN_SEG3) && ((crc + ii) <= END_SEG3)) {

              if ((crc + kol - 1) <= END_SEG3) {

                RdFromFleshToArr(A_SEG3 + (crc + ii - BEGIN_SEG3) * 2,
                                 &buf_tx_232[1000], (kol - ii) * 2);
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = kol;
              }

              else {

                RdFromFleshToArr(A_SEG3 + (crc + ii - BEGIN_SEG3) * 2,
                                 &buf_tx_232[1000],
                                 (END_SEG3 - (crc + ii) + 1) * 2);
                for (k = 0; k < (END_SEG3 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = END_SEG3 - crc;
              }
            }

            if (((crc + ii) >= BEGIN_SEG8) && ((crc + ii) <= END_SEG8)) {

              if ((crc + kol - 1) <= END_SEG8) {

                // 28 12 2019
                // RdFromFleshToArr(A_SEG8+(crc+ii-BEGIN_SEG8)*2,&buf_tx_232[1000],(kol-ii)*2);
                tc_cfgPoint = (unsigned int *)&buf_tx_232[1000];
                for (k = 0; k < (kol - ii); k++)
                  *tc_cfgPoint++ = c_config_tc[crc + ii - BEGIN_SEG8 + k];
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = kol;
              }

              else {

                // 28 12 2019
                // RdFromFleshToArr(A_SEG8+(crc+ii-BEGIN_SEG8)*2,&buf_tx_232[1000],(END_SEG8-(crc+ii)+1)*2);
                tc_cfgPoint = (unsigned int *)&buf_tx_232[1000];
                for (k = 0; k < (END_SEG8 - (crc + ii) + 1); k++)
                  *tc_cfgPoint++ = c_config_tc[crc + ii - BEGIN_SEG8 + k];
                for (k = 0; k < (END_SEG8 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&buf_tx_232[1000 + (k << 1)]);
                ii = END_SEG8 - crc;
              }
            }

            if (((crc + ii) >= BEGIN_SEG6) && ((crc + ii) <= END_SEG6)) {

              if ((crc + kol - 1) <= END_SEG6) {

                // RdFromFleshToArr(A_SEG6+(crc+ii-BEGIN_SEG6)*2,&buf_tx_232[600],(kol-ii)*2);
                // читаю по одному слову и упаковую в выходной массив
                // иначе происходит переполнение массива !!!! 4 10 2021

                for (k = 0; k < (kol - ii); k++) {
                  RdFromFleshToArr(A_SEG6 + (k + crc + ii - BEGIN_SEG6) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                  //  *(unsigned
                  //  int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                  //  int*)&buf_tx_232[600+(k<<1)]);
                }

                ii = kol;
              }

              else {

                // RdFromFleshToArr(A_SEG6+(crc+ii-BEGIN_SEG6)*2,&buf_tx_232[600],(END_SEG6-(crc+ii)+1)*2);
                for (k = 0; k < (END_SEG6 - (crc + ii) + 1); k++) {
                  RdFromFleshToArr(A_SEG6 + (k + crc + ii - BEGIN_SEG6) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                  //*(unsigned
                  //int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                  //int*)&buf_tx_232[600+(k<<1)]);
                }
                ii = END_SEG6 - crc;
              }
            }

            if (((crc + ii) >= BEGIN_SEG5) && ((crc + ii) <= END_SEG5)) {

              if ((crc + kol - 1) <= END_SEG5) {
                // RdFromFleshToArr(A_SEG5+(crc+ii-BEGIN_SEG5)*2,&buf_tx_232[700],(kol-ii)*2);
                // читаю по одному слову и упаковую в выходной массив
                // иначе происходит переполнение массива !!!! 4 10 2021
                for (k = 0; k < (kol - ii); k++) {

                  RdFromFleshToArr(A_SEG5 + (k + crc + ii - BEGIN_SEG5) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                  // *(unsigned
                  // int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                  // int*)&buf_tx_232[700+(k<<1)]);
                }
                ii = kol;
              } else {
                //  RdFromFleshToArr(A_SEG5+(crc+ii-BEGIN_SEG5)*2,&buf_tx_232[700],(END_SEG5-(crc+ii)+1)*2);
                for (k = 0; k < (END_SEG5 - (crc + ii) + 1); k++) {
                  RdFromFleshToArr(A_SEG5 + (k + crc + ii - BEGIN_SEG5) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                  // *(unsigned
                  // int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                  // int*)&buf_tx_232[700+(k<<1)]);
                }
                ii = END_SEG5 - crc;
              }
            }

            ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! чтение
            ///архивов ГЗУ
            if (((crc + ii) >= BEGIN_SEG55) && ((crc + ii) <= END_SEG55)) {

              if ((crc + kol - 1) <= END_SEG55) {
                // RdFromFleshToArr(STRUCT_US
                // +(crc+ii-BEGIN_SEG55)*2,&buf_tx_232[700],(kol-ii)*2); читаю
                // по одному слову и упаковую в выходной массив иначе происходит
                // переполнение массива !!!! 4 10 2021
                for (k = 0; k < (kol - ii); k++) {
                  RdFromFleshToArr(STRUCT_US + (k + crc + ii - BEGIN_SEG55) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                  //*(unsigned
                  //int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                  //int*)&buf_tx_232[700+(k<<1)]);
                }
                ii = kol;
              } else {
                // RdFromFleshToArr(STRUCT_US
                // +(crc+ii-BEGIN_SEG55)*2,&buf_tx_232[700],(END_SEG55-(crc+ii)+1)*2);
                for (k = 0; k < (END_SEG55 - (crc + ii) + 1); k++) {
                  RdFromFleshToArr(STRUCT_US + (k + crc + ii - BEGIN_SEG55) * 2,
                                   &flashka[0], 2);
                  *(unsigned int *)(mobus.point_out + (ii << 1) + (k << 1)) =
                      swap(*(unsigned int *)&flashka[0]);
                }
                //*(unsigned
                //int*)(mobus.point_out+(ii<<1)+(k<<1))=swap(*(unsigned
                //int*)&buf_tx_232[700+(k<<1)]);
                ii = END_SEG55 - crc;
              }
            }
            ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

          } // end for
          ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

          Obj_ppp_tx.l_data = 3 + (kol * 2) + (mobus.smest_to_elam * 2);
          mobus.point_out = mobus.point_out_save + Obj_ppp_tx.l_data;
          //  *(unsigned
          //  int*)&buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data]=crc_m1(&buf_tx_232[TR_OP_DATA+C1_DATA],Obj_ppp_tx.l_data,0xffff);
          mobus.modbus_crc_new = crc_m1(mobus.point_out - Obj_ppp_tx.l_data,
                                        Obj_ppp_tx.l_data, 0xffff);
          *mobus.point_out++ = mobus.modbus_crc_new >> 8;
          *mobus.point_out++ = mobus.modbus_crc_new;
          mobus.count_out_elam +=
              Obj_ppp_tx.l_data + 2; // плюс контрольная сумма

          //    Obj_ppp_tx.l_data=3+kol*2; //без crc

          break;
        case 5:
          /////  *(unsigned
          ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+2]=swap(crc);//start
          /////  *(unsigned int*)&buf_tx_232[TR_OP_DATA+C1_DATA+4]=0;

          // ожидаемое количество байт в выходной буфер
          mobus.overall += 6 + mobus.smest_to_elam + 2;
          if (mobus.overall > VOL_TX_PPP)
            return (0); // выход по ошибке
                        // ожидаемое количество байт в выходной буфер

          *mobus.point_out++ = *(mobus.point_in + mobus.index_elam);
          if (mobus.flag_elam == 1)
            *mobus.point_out++ =
                *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 1);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 2);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 3);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 4);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 5);
          Obj_ppp_tx.l_data = 6 + mobus.smest_to_elam;
          mobus.modbus_crc_new = crc_m1(mobus.point_out - Obj_ppp_tx.l_data,
                                        Obj_ppp_tx.l_data, 0xffff);
          *mobus.point_out++ = mobus.modbus_crc_new >> 8;
          *mobus.point_out++ = mobus.modbus_crc_new;
          mobus.count_out_elam +=
              Obj_ppp_tx.l_data + 2; // плюс контрольная сумма

          if (sel_modul != 1) {
            switch (crc) {
            case MB_TU1:
              if (c1_byte.mb[0] == 0x55)
                break; // не давать включать ту безконтрольно
              if (kol == 0xff00) {
                TU1_ON;
              }
              if (kol == 0x0000) {
                TU1_OFF;
              }
              break;
            case MB_TU2:
              if (kol == 0xff00) {
                TU2_ON;
              }
              if (kol == 0x0000) {
                TU2_OFF;
              }
              break;
            case MB_TU1_IMP:
              if (c1_byte.mb[0] == 0x55)
                break; // не давать включать ту безконтрольно

              if (kol == 0xff00) {
                //  TU1_ON;cnt_tu1=TM_TU_IMP;

                TU1_ON;
                cnt_tu1 = c2_byte.mb[0x1f * 2];
                if (cnt_tu1 == 0 || cnt_tu1 == 0xff)
                  cnt_tu1 = TM_TU_IMP;
              }
              break;
            case MB_TU2_IMP:

              if (kol == 0xff00) {
                //  TU2_ON;cnt_tu2=TM_TU_IMP;

                TU2_ON;
                cnt_tu2 = c2_byte.mb[0x1f * 2 + 1];
                if (cnt_tu2 == 0 || cnt_tu2 == 0xff)
                  cnt_tu2 = TM_TU_IMP;
              }
              break;

            //////////////////////////////////
            case MB_BPA:
              if (kol == 0xff00) {
                bpa.sost = 1;
                break;
              }
              if (kol == 0x0000) {
                bpa.sost = 3;
                break;
              }

              break;
            }

          } else {
            if (crc == MB_COM_DISP)
              com_disp_mbus();
          }

          ////// *(unsigned int*)&buf_tx_232[TR_OP_DATA+C1_DATA+4]=swap(kol);

          // Сформировать registr value и конечное число отправляемых байт
          // выдать команду

          ///// Obj_ppp_tx.l_data=6;
          break;
        case 6:

          //////////07 11 2019 *(unsigned
          ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+2]=swap(crc);//start
          //////////07 11 2019 *(unsigned
          ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+4]=swap(kol);

          // ожидаемое количество байт в выходной буфер
          mobus.overall += 6 + mobus.smest_to_elam + 2;
          if (mobus.overall > VOL_TX_PPP)
            return (0); // выход по ошибке
                        // ожидаемое количество байт в выходной буфер

          *mobus.point_out++ = *(mobus.point_in + mobus.index_elam);
          if (mobus.flag_elam == 1)
            *mobus.point_out++ =
                *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 1);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 2);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 3);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 4);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 5);
          Obj_ppp_tx.l_data = 6 + mobus.smest_to_elam;
          mobus.modbus_crc_new = crc_m1(mobus.point_out - Obj_ppp_tx.l_data,
                                        Obj_ppp_tx.l_data, 0xffff);
          *mobus.point_out++ = mobus.modbus_crc_new >> 8;
          *mobus.point_out++ = mobus.modbus_crc_new;
          mobus.count_out_elam +=
              Obj_ppp_tx.l_data + 2; // плюс контрольная сумма

          *(unsigned int *)&buf_tx_232[1000] = kol;

          if ((crc >= BEGIN_SEGWR19) && (crc <= END_SEGWR19)) {

            zadan_ys = buf_tx_232[1000];
          }

          if ((crc >= BEGIN_SEGWR9) && (crc <= END_SEGWR9)) {
            WrArrayToFlesh(A_SEG9 + (crc - BEGIN_SEGWR9) * 2, &buf_tx_232[1000],
                           2, 0, 0);
            // RdFromFleshToArr(A_SEG9,(unsigned char *)&ust_vhod,SEG9*2);
            RdFromFleshToArrInt(A_SEG9, &ust_vhod, 1);
          }

          if ((crc >= BEGIN_SEGWR) && (crc <= END_SEGWR)) {
            modbus_mem1[crc - BEGIN_SEGWR] = kol;
            cnt_tii[crc - BEGIN_SEGWR] = kol;
            // for(i=0;i<8;i++)cnt_tii[i]=modbus_mem1[AD_TII1+i];
            crc_tii = func_crc_tii();
          }

          if ((crc >= BEGIN_SEGWR3) && (crc <= END_SEGWR3)) {
            RdFromFleshToArr(A_SEG3 + (crc - BEGIN_SEGWR3) * 2,
                             &buf_tx_232[1050], 2);
            WrArrayToFlesh(A_SEG3 + (crc - BEGIN_SEGWR3) * 2, &buf_tx_232[1000],
                           2, 0, 0);
            RdFromFleshToArr(A_SEG3, (unsigned char *)&c2_byte.mb[0], SEG3 * 2);
            if (*(unsigned int *)&buf_tx_232[1050] !=
                *(unsigned int *)&buf_tx_232[1000])
              refresh_a(crc - BEGIN_SEG3);
          }

          if ((crc >= BEGIN_SEGWR8) && (crc <= END_SEGWR8)) {
            WrArrayToFlesh(A_SEG8 + (crc - BEGIN_SEGWR8) * 2, &buf_tx_232[1000],
                           2, 0, 0);
            //       RdFromFleshToArr(A_SEG8,(unsigned char
            //       *)&c_config_tc[0],SEG8*2);
            if ((crc - BEGIN_SEGWR8) == 0x17) {
              k_a_tit[0] = 0;
              k_a_tit[1] = 0;
            }
            RdFromFleshToArrInt(A_SEG8, &c_config_tc[0], SEG8);
          }

          // записать конфигурацию ГЗУ
          if ((crc >= BEGIN_SEGWR99) && (crc <= END_SEGWR99)) {
            WrArrayToFlesh(A_SEG99 + (crc - BEGIN_SEGWR99) * 2,
                           &buf_tx_232[1000], 2, 0, 0);

            RdFromFleshToArr(A_SEG99, (unsigned char *)&c1_byte.mb[0],
                             SEG99 * 2);
          }

          if ((crc >= BEGIN_SEGWR6) && (crc <= END_SEGWR6)) {
            WrArrayToFlesh(A_SEG6 + (crc - BEGIN_SEGWR6) * 2, &buf_tx_232[1000],
                           2, 0, 0);
          }

          if ((crc >= BEGIN_SEGWR4) && (crc <= END_SEGWR4)) {
            flag_time = 0;
            burst_ds_r();

            buf_tx_232[1000] = real_time.r_sec;
            buf_tx_232[1001] = real_time.r_min;
            buf_tx_232[1002] = real_time.r_hor;
            buf_tx_232[1003] = real_time.r_date;
            buf_tx_232[1004] = real_time.r_month;
            buf_tx_232[1005] = real_time.r_year;

            if (crc == MB_SEC)
              if (kol < 60) {
                buf_tx_232[1000] = kol;
                flag_time = 1;
              }
            if (crc == MB_MIN)
              if (kol < 60) {
                buf_tx_232[1001] = kol;
                flag_time = 1;
              }
            if (crc == MB_HR)
              if (kol < 24) {
                buf_tx_232[1002] = kol;
                flag_time = 1;
              }
            if (crc == MB_DAY)
              if (kol < 32) {
                buf_tx_232[1003] = kol;
                flag_time = 1;
              }
            if (crc == MB_MONTH)
              if (kol < 13)
                if (kol != 0) {
                  buf_tx_232[1004] = kol;
                  flag_time = 1;
                }
            if (crc == MB_YEAR) {
              // year=kol-2000;
              // if(year<100){buf_tx_232[1005]=year;flag_time=1;}
              if (kol < 100) {
                buf_tx_232[1005] = kol;
                flag_time = 1;
              }
            }
            if (flag_time != 0) {
              old_unix = unix;
              set_rlt(2, &buf_tx_232[1000]);
              burst_ds_r();
              // if ((( old_unix - unix) >= 600 )||(( unix - old_unix) >= 600 ))
              // bit_level=1;
              if (unix > old_unix) {
                if ((unix - old_unix) >= 600)
                  bit_level = 1;
              } else {
                if ((old_unix - unix) >= 600)
                  bit_level = 1;
              }

              if ((sost_psm == 4) && (bit_level == 1))
                bit_level_psm = 1;
            }
          }

          /////07 11 2019   Obj_ppp_tx.l_data=6;

          break;
        case 16:

          // ожидаемое количество байт в выходной буфер
          mobus.overall += 6 + mobus.smest_to_elam + 2;
          if (mobus.overall > VOL_TX_PPP)
            return (0); // выход по ошибке
                        // ожидаемое количество байт в выходной буфер

          *mobus.point_out++ = *(mobus.point_in + mobus.index_elam);
          if (mobus.flag_elam == 1)
            *mobus.point_out++ =
                *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 1);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 2);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 3);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 4);
          *mobus.point_out++ =
              *(mobus.point_in + mobus.index_elam + mobus.smest_to_elam + 5);
          Obj_ppp_tx.l_data = 6 + mobus.smest_to_elam;
          mobus.modbus_crc_new = crc_m1(mobus.point_out - Obj_ppp_tx.l_data,
                                        Obj_ppp_tx.l_data, 0xffff);
          *mobus.point_out++ = mobus.modbus_crc_new >> 8;
          *mobus.point_out++ = mobus.modbus_crc_new;
          mobus.count_out_elam +=
              Obj_ppp_tx.l_data + 2; // плюс контрольная сумма

          flag_tii = 0;
          flag_time = 0;
          flag_refresh = 0;

          burst_ds_r();

          buf_tx_232[1000] = real_time.r_sec;
          buf_tx_232[1001] = real_time.r_min;
          buf_tx_232[1002] = real_time.r_hor;
          buf_tx_232[1003] = real_time.r_date;
          buf_tx_232[1004] = real_time.r_month;
          buf_tx_232[1005] = real_time.r_year;

          RdFromFleshToArr(A_SEG3, &buf_tx_232[1050], 16);

          /////07 11 2019 *(unsigned
          ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+2]=swap(crc);//start
          /////07 11 2019 *(unsigned
          ///int*)&buf_tx_232[TR_OP_DATA+C1_DATA+4]=swap(kol);//start
          /////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          /////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!

          for (ii = 0; ii < kol; ii++) {

            if (((crc + ii) >= BEGIN_SEGWR9) && ((crc + ii) <= END_SEGWR9)) {
              if ((crc + kol - 1) <= END_SEGWR9) {
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG9 + (crc + ii - BEGIN_SEGWR9) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = kol;
              }

              else {
                for (k = 0; k < (END_SEG9 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG9 + (crc + ii - BEGIN_SEGWR9) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = END_SEGWR9 - crc;
              }

              //        RdFromFleshToArr(A_SEG9,(unsigned char
              //        *)&ust_vhod,SEG9*2);
              RdFromFleshToArrInt(A_SEG9, &ust_vhod, 1);
            }

            if ((crc + ii) == MB_TII1) {
              modbus_mem1[AD_TII1] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[0] = modbus_mem1[AD_TII1];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII2) {
              modbus_mem1[AD_TII2] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[1] = modbus_mem1[AD_TII2];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII3) {
              modbus_mem1[AD_TII3] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[2] = modbus_mem1[AD_TII3];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII4) {
              modbus_mem1[AD_TII4] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[3] = modbus_mem1[AD_TII4];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII5) {
              modbus_mem1[AD_TII5] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[4] = modbus_mem1[AD_TII5];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII6) {
              modbus_mem1[AD_TII6] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[5] = modbus_mem1[AD_TII6];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII7) {
              modbus_mem1[AD_TII7] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[6] = modbus_mem1[AD_TII7];
              flag_tii = 1;
            }
            if ((crc + ii) == MB_TII8) {
              modbus_mem1[AD_TII8] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              cnt_tii[7] = modbus_mem1[AD_TII8];
              flag_tii = 1;
            }

            if (((crc + ii) >= (BEGIN_SEGWR + 8)) && ((crc + ii) <= END_SEGWR))
              modbus_mem1[crc + ii - BEGIN_SEGWR] =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));

            if (((crc + ii) >= BEGIN_SEGWR3) && ((crc + ii) <= END_SEGWR3)) {
              flag_refresh = 1;
              bit_level = 1;
              if ((crc + kol - 1) <= END_SEGWR3) {
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG3 + (crc + ii - BEGIN_SEGWR3) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = kol;
              }

              else {
                for (k = 0; k < (END_SEG3 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG3 + (crc + ii - BEGIN_SEGWR3) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = END_SEGWR3 - crc;
              }

              RdFromFleshToArr(A_SEG3, (unsigned char *)&c2_byte.mb[0],
                               SEG3 * 2);
            }

            if (((crc + ii) >= BEGIN_SEGWR8) && ((crc + ii) <= END_SEGWR8)) {
              if ((crc + kol - 1) <= END_SEGWR8) {
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG8 + (crc + ii - BEGIN_SEGWR8) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                if ((crc + kol - 1) == END_SEGWR8) {
                  k_a_tit[0] = 0;
                  k_a_tit[1] = 0;
                }
                ii = kol;
              }

              else {
                for (k = 0; k < (END_SEG8 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG8 + (crc + ii - BEGIN_SEGWR8) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = END_SEGWR8 - crc;
              }

              // RdFromFleshToArr(A_SEG8,(unsigned char
              // *)&c_config_tc[0],SEG8*2);
              RdFromFleshToArrInt(A_SEG8, &c_config_tc[0], SEG8);
            }

            /// ГЗУ
            if (((crc + ii) >= BEGIN_SEGWR99) && ((crc + ii) <= END_SEGWR99)) {
              if ((crc + kol - 1) <= END_SEGWR99) {
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG99 + (crc + ii - BEGIN_SEGWR99) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = kol;
              }

              else {
                for (k = 0; k < (END_SEG99 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG99 + (crc + ii - BEGIN_SEGWR99) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = END_SEGWR99 - crc;
              }

              RdFromFleshToArr(A_SEG99, (unsigned char *)&c1_byte.mb[0],
                               SEG99 * 2);
            }

            if (((crc + ii) >= BEGIN_SEGWR6) && ((crc + ii) <= END_SEGWR6)) {

              if ((crc + kol - 1) <= END_SEGWR6) {
                for (k = 0; k < (kol - ii); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG6 + (crc + ii - BEGIN_SEGWR6) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = kol;
              }

              else {
                for (k = 0; k < (END_SEG6 - (crc + ii) + 1); k++)
                  *(unsigned int *)(mobus.point_in + mobus.index_elam +
                                    mobus.smest_to_elam + 7 + (ii << 1) +
                                    (k << 1)) =
                      swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                             mobus.smest_to_elam + 7 +
                                             (ii << 1) + (k << 1)));
                WrArrayToFlesh(A_SEG6 + (crc + ii - BEGIN_SEGWR6) * 2,
                               (mobus.point_in + mobus.index_elam +
                                mobus.smest_to_elam + 7 + (ii << 1)),
                               (kol - ii) * 2, 0, 0);
                ii = END_SEGWR6 - crc;
              }
            }

            if ((crc + ii) == MB_SEC)
              if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1))) <
                  60) {
                buf_tx_232[1000] =
                    buf_rx_ppp[offset + C1_DATA + 16 + (ii << 1)];
                flag_time = 1;
              }
            if ((crc + ii) == MB_MIN)
              if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1))) <
                  60) {
                buf_tx_232[1001] =
                    buf_rx_ppp[offset + C1_DATA + 16 + (ii << 1)];
                flag_time = 1;
              }

            if ((crc + ii) == MB_HR)
              if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1))) <
                  24) {
                buf_tx_232[1002] =
                    buf_rx_ppp[offset + C1_DATA + 16 + (ii << 1)];
                flag_time = 1;
              }

            if ((crc + ii) == MB_DAY)
              if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1))) <
                  32) {
                buf_tx_232[1003] =
                    buf_rx_ppp[offset + C1_DATA + 16 + (ii << 1)];
                flag_time = 1;
              }

            if ((crc + ii) == MB_MONTH)
              if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1))) <
                  13)
                if (swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                           mobus.smest_to_elam + 7 +
                                           (ii << 1))) != 0) {
                  buf_tx_232[1004] =
                      buf_rx_ppp[offset + C1_DATA + 16 + (ii << 1)];
                  flag_time = 1;
                }

            if ((crc + ii) == MB_YEAR) {
              // year=swap(*(unsigned
              // int*)&buf_rx_ppp[offset+C1_DATA+15+(ii<<1)])-2000;
              year =
                  swap(*(unsigned int *)(mobus.point_in + mobus.index_elam +
                                         mobus.smest_to_elam + 7 + (ii << 1)));
              if (year < 100) {
                buf_tx_232[1005] = year;
                flag_time = 1;
              }
            }
          }
          ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          ///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

          /*
          if(flag_refresh==1)
          {
            RdFromFleshToArr(A_SEG3,(unsigned char *)&c2_byte.mb[0],SEG3*2);
            RdFromFleshToArr(A_SEG3,&buf_tx_232[900],16);
            for(k=0;k<8;k++)
            {
             if(*(unsigned int*)&buf_tx_232[900+k*2]!=*(unsigned
          int*)&buf_tx_232[1050+k*2])refresh_a(k);

            }


           }

         */

          if (flag_time != 0) {
            old_unix = unix;
            set_rlt(2, &buf_tx_232[1000]);
            burst_ds_r();
            // if ((( old_unix - unix) >= 600 )||(( unix - old_unix) >= 600 ))
            // bit_level=1;
            if (unix > old_unix)

            {
              if ((unix - old_unix) >= 600)
                bit_level = 1;
            } else {
              if ((old_unix - unix) >= 600)
                bit_level = 1;
            }
          }

          if (flag_tii != 0)
            crc_tii = func_crc_tii();
          /////07 11 2019 Obj_ppp_tx.l_data=6;

          break;
          /* таких функций не будет !!
  case 0x8302:
  case 0x8303:
  case 0x8402:
  case 0x8403:
  case 0x8503:
  case 0x9002:
  case 0x9003:
             buf_tx_232[TR_OP_DATA+C1_DATA+1]=ii>>8;
             buf_tx_232[TR_OP_DATA+C1_DATA+2]=ii;
             Obj_ppp_tx.l_data=3;
             break;
          */
        default:
          return (0); // выход по ошибке модбас пакета
        }

        if (mobus.flag_elam == 1)
          goto ReEnter;

      FinitaPerformence:
        // switch обработчик модбас фрагмента                         закончен
        // **************************************

        // этот кусок не нужен срс считается в каждом фрагменте
        //  *(unsigned
        //  int*)&buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data]=crc_m1(&buf_tx_232[TR_OP_DATA+C1_DATA],Obj_ppp_tx.l_data,0xffff);
        //  //crc
        // i=buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data];
        // buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data]=buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data+1];
        // buf_tx_232[TR_OP_DATA+C1_DATA+Obj_ppp_tx.l_data+1]=i;
        // этот кусок не нужен срс считается в каждом фрагменте

        Obj_ppp_tx.l_data = mobus.count_out_elam + 2;

        //      Obj_ppp_tx.l_data=Obj_ppp_tx.l_data+4;   // длина данных плюс 2
        //      байта срс плюс 2 байта

      metka_send_mbus:
        Obj_ppp_tx.p_data =
            &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

        send_info(sizeof(ans_out_mbus), ans_out_mbus, 1, Obj_ppp_tx.id_pac);

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
      break;
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

unsigned char null_tii_485(unsigned char *pnt_buf) {
  if ((sel_modul == 1) && (pnt_buf[2] > 4))
    return (0); // мкд
  if ((sel_modul != 1) && (pnt_buf[2] >= 8))
    return (0); // мтс
  // if (pnt_buf[2]>4) return(0);
  modbus_mem1[AD_TII1 + pnt_buf[2]] = 0;
  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = 0;
  Obj_ppp_tx.l_data = 4;
  return (1);
}

unsigned char extract_tit_485(unsigned char *pnt_buf) {
  if (pnt_buf[2] > 1)
    return (0);
  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = pnt_buf[2];
  *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 3] =
      modbus_mem1[AD_TIT1 + pnt_buf[2]];
  Obj_ppp_tx.l_data = 6;
  return (1);
}

unsigned char extract_tii_485(unsigned char *pnt_buf) {
  // if (pnt_buf[2]>4)return(0);
  if ((sel_modul == 1) && (pnt_buf[2] > 4))
    return (0); // мкд
  if ((sel_modul != 1) && (pnt_buf[2] >= 8))
    return (0); // мтс

  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = pnt_buf[2];
  *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 3] =
      modbus_mem1[AD_TII1 + pnt_buf[2]];
  Obj_ppp_tx.l_data = 6;
  return (1);
}

void tc_485(void) {

  *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 2] = modbus_mem1[AD_TS];
  read_ohr();
  buf_tx_232[TR_OP_DATA + C1_DATA + 4] = bufer[0];
  buf_tx_232[TR_OP_DATA + C1_DATA + 5] = bufer[1];
  // buf_tx_232[TR_OP_DATA+C1_DATA+6]=crc_485(6,&buf_tx_232[TR_OP_DATA+C1_DATA]);
  Obj_ppp_tx.l_data = 7;
}

void null_all_tii_485(void) {
  char i, k, r;
  k = 0;
  if (buf_tx_232[TR_OP_DATA + C1_DATA] == 6)
    k = 1;
  if (sel_modul == 1)
    r = 5;
  else
    r = 8;

  for (i = k; i < r; i++)
    modbus_mem1[AD_TII1 + i] = 0;
  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = 0;
  Obj_ppp_tx.l_data = 4;
}

void extract_all_tii_485(void) {
  char i;
  for (i = 0; i < 16; i++)
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 2 + (i << 1)] =
        modbus_mem1[AD_TII1 + i];
  Obj_ppp_tx.l_data = 35;
}

void extract_all_tit_485(void) {
  char i;
  for (i = 0; i < 8; i++)
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 2 + (i << 1)] =
        modbus_mem1[AD_TIT1 + i];
  Obj_ppp_tx.l_data = 19;
}

void com_disp(unsigned char *pnt_buf) {
  unsigned int pred_sost;

  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = pnt_buf[2];

  bit_registr1 = bit_registr1 | TABLETKA;
  nomer_tab = 0;
  pred_sost = sost;
  switch (sost) {
  case 0:
    test_vzat();
    break;
  case 1:
    test_snat();
    break;
  case 2:
    test_alarm();
    break;
  case 3:
    test_t_vhod();
    break;
  case 4:
    test_t_vihod();
    break;
  default:
    break;
  }

  if (pred_sost == sost)
    buf_tx_232[TR_OP_DATA + C1_DATA] = 192;
  Obj_ppp_tx.l_data = 4;
}

void w_in_out_485(unsigned char *pnt_buf) {
  //   char i;

  //    for(i=0;i<4;i++)Rs485_buf_rx_tx[TX_BUF_RS485+i]=Rs485_buf_rx_tx[i];
  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = pnt_buf[2];
  //          buf_tx_232[TR_OP_DATA+C1_DATA+3]=pnt_buf[3];
  Obj_ppp_tx.l_data = 4;

  /*
  c_config_kd[AD_TM_VHOD]=Rs485_buf_rx_tx[2] & 0x1f;
  WrArrayToFleshInt(A_KD,&c_config_kd[AD_TM_VHOD],1,0,0);
  ust_vhod=c_config_kd[AD_TM_VHOD]*1000;
  */

  WrArrayToFlesh(A_SEG9, &buf_tx_232[TR_OP_DATA + C1_DATA + 2], 2, 0, 0);
  // RdFromFleshToArr(A_SEG9,(unsigned char *)&ust_vhod,SEG9*2);
  RdFromFleshToArrInt(A_SEG9, &ust_vhod, 1);
}

void r_in_out_485(void) {
  RdFromFleshToArrInt(A_SEG9, &ust_vhod, 1);
  buf_tx_232[TR_OP_DATA + C1_DATA + 2] = ust_vhod;
  Obj_ppp_tx.l_data = 4;
}

unsigned char proc_kamintel(unsigned char *pnt_buf, unsigned char length) {
  if (pnt_buf[length - 1] != crc_485(length - 1, &pnt_buf[0]))
    return (0);

  switch (pnt_buf[0])

  {

  case 0:
    if (null_tii_485(&pnt_buf[0]) == 0)
      return (0);
    else
      break;
  case 1:
    if (extract_tit_485(&pnt_buf[0]) == 0)
      return (0);
    else
      break;
  case 3:
    if (extract_tii_485(&pnt_buf[0]) == 0)
      return (0);
    else
      break;
  case 4:
    tc_485();
    break;
  case 5:
  case 6:
    null_all_tii_485();
    break;
  case 10:
    extract_all_tii_485();
    break;
  case 11:
    extract_all_tit_485();
    break;
  case 20:
    obmen_full = 0;
    return (0);

  case 128:
    com_disp(&pnt_buf[0]);
    break; // exo_485();break;

  case 201:
    w_in_out_485(&pnt_buf[0]);
    break; // задержка на вход выход записать
  case 200:
    r_in_out_485();
    break; // задержка на вход выход прочитать

  default:
    return (0);
  }

  buf_tx_232[TR_OP_DATA + C1_DATA + Obj_ppp_tx.l_data - 1] =
      crc_485(Obj_ppp_tx.l_data - 1, &buf_tx_232[TR_OP_DATA + C1_DATA]);
  return (1);
}

unsigned int proc_modbus(unsigned char *pnt_buf, unsigned char length) {
  unsigned int temp;
  unsigned char i;

  i = pnt_buf[length - 2];
  pnt_buf[length - 2] = pnt_buf[length - 1];
  pnt_buf[length - 1] = i;
  if (*(unsigned int *)&pnt_buf[length - 2] !=
      crc_m1(&pnt_buf[0], length - 2, 0xffff))
    return (0); // crc

  switch (pnt_buf[1]) {
  case 1:
    if (length != 8)
      return (0);
    if (swap(*(unsigned int *)&pnt_buf[4]) == 0 ||
        swap(*(unsigned int *)&pnt_buf[4]) > 2000) {
      temp = pnt_buf[1];
      temp = temp << 8;
      return (temp | 0x8003);
    }
    if (swap(*(unsigned int *)&pnt_buf[4]) > 8)
      return (0);
    return (1);
  case 3:
  case 4:
    if (length != 8)
      return (0);
    if (swap(*(unsigned int *)&pnt_buf[4]) == 0 ||
        swap(*(unsigned int *)&pnt_buf[4]) > 125) {
      temp = pnt_buf[1];
      temp = temp << 8;
      return (temp | 0x8003);
    }
    if ((0xffff - swap(*(unsigned int *)&pnt_buf[2])) <
        (swap(*(unsigned int *)&pnt_buf[4]) - 1)) {
      temp = pnt_buf[1];
      temp = temp << 8;
      return (temp | 0x8002);
    }
    break;
  case 5:
    if (length != 8)
      return (0);
    if (swap(*(unsigned int *)&pnt_buf[4]) != 0 &&
        swap(*(unsigned int *)&pnt_buf[4]) != 0xff00) {
      temp = pnt_buf[1];
      temp = temp << 8;
      return (0x8503);
    }
    return (5);
  case 6:
    if (length != 8)
      return (0);
    return (6);
  case 16:
    if (length < 11)
      return (0);
    if (length > 249)
      return (0);
    if (swap(*(unsigned int *)&pnt_buf[4]) == 0 ||
        swap(*(unsigned int *)&pnt_buf[4]) > 120)
      return (0x9003);
    temp = pnt_buf[5];
    if (temp != swap(*(unsigned int *)&pnt_buf[4]))
      return (0x9003);
    if ((0xffff - swap(*(unsigned int *)&pnt_buf[2])) <
        (swap(*(unsigned int *)&pnt_buf[4]) - 1))
      return (0x9002);
    return (16);
  default:
    return (0);
  }

  return (pnt_buf[1]);
}

/*

void razbor_input_packet(char smest)   //
{
char size_of_pac;

struct {
        unsigned char *point_out;
        unsigned char *point_in;
        unsigned char flag_elam;
        unsigned int count_in_elam;
        unsigned int index_elam;
        unsigned int size_of_pac;

}mobus;

   mobus.point_out=& buf_tx_232[TR_OP_DATA+C1_DATA];        // выходной буфер
   mobus.point_in = & buf_rx_ppp[offset+C1_DATA+8];         // входной буфер с
модбас пакетом mobus.index_elam =0;                                     //
индекс очередного инкапсулированного елам пакета mobus.flag_elam=0;
   mobus.count_in_elam=length-10;

//
//
//
//
next_elam:
            if ( (  * (  mobus.point_in +  mobus.index_elam) & 0xf8) !=0xf8)
goto ne_elam; mobus.flag_elam=1; switch (* ( mobus.point_in +  mobus.index_elam
+ 2))
                {
                case 1:
                case 3:
                case 4:
                case 5:
                case 6:  mobus.size_of_pac=7;break;
                case 16:  mobus.size_of_pac=8 + *( mobus.point_in +
mobus.index_elam + 7);break; default : {respons();return;}
                }
                if ( mobus.count_in_elam < ( mobus.index_elam +
mobus.size_of_pac + 2) )  {respons();return;}


// проверка фрагмента на CRC
 mobus.modbus_crc_new=crc_m1(mobus.point_in + mobus.index_elam, size_of_pac ,
0xffff); if(mobus.modbus_crc_new!=(  (  (unsigned int)( * (mobus.point_in +
mobus.index_elam + mobus.size_of_pac) )<<8)
                             + (   unsigned int)( * (mobus.point_in +
mobus.index_elam+size_of_pac + 1) ) )  )     {respons();return;}



               //zapoln_otvet_elam();
               goto Performance;
FinitaPerformance:
               mobus.index_elam=mobus.index_elam + mobus.size_of_pac + 2;
               if (mobus.count_in_elam > mobus.index_elam ) goto next_elam;
               respons();
               return;

ne_elam:
              if ( mobus.flag_elam==1)  {respons();return;}
              // это одиночный пакет

 mobus.modbus_crc_new=crc_m1(mobus.point_in, mobus.count_in_elam-2,0xffff);





             if(mobus.modbus_crc_new!=(  (  (unsigned int)( * (mobus.point_in +
mobus.count_in_elam-2) )<<8)
                             + (   unsigned int)( * (mobus.point_in +
mobus.count_in_elam-1) ) ) ) {

                             respons();return;}



         //    zapoln_otvet_elam();
             mobus.flag_elam=0;
             goto Performance;
             respons();
             return;


}

*/
