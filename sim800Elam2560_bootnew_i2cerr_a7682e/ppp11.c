#include "def_at.h"
#include "def_conf.h"
#include "def_link.h"
#include "def_log.h"
#include "def_pin.h"
#include "def_prot.h"
#include "map_ad.h"
#include "map_ef.h"
#include "ozu_map.h"
#include "sec.h"
#include <inavr.h>
#include <iom128.h>
#include <stdlib.h>
#include <string.h>

#define C_GLUK1 0x55
#define C_GLUK2 0xaa

// 23.03.07 попытка адоптации Сименса 35 путем соглашения на все запрашиваемые
// опции в PPP

// CTS от модема  на  физический DSR контроллера
// реально отправилось не болеее 1474 (Wavecom) и 1472 (SonyErricson) данных UDP
// при MRU=1500 ( заголовки: 20 IP+8 UDP) два буфера на прием, прием по концу
// 0х7е

//__flash char com_AT[] = {'A','T'};
//__flash char com_ATE0V1[] = {'A','T','E','0','V','1'};
//__flash char com_AT_IRP[] =
//{'A','T','+','I','P','R','=','1','1','5','2','0','0'};
//__flash char com_AT_CGCLASS[] =
//{'A','T','+','C','G','C','L','A','S','S','=','"','C','G','"'};
//__flash char com_AT_CGATT[] = {'A','T','+','C','G','A','T','T','=','1'};
//__flash char com_AT_CGDCONT[] =
//{'A','T','+','C','G','D','C','O','N','T','=','1',',','"','I','P','"',','};
//__flash char com_AT_CGATT_R[ ] = {'A','T','+','C','G','A','T','T','?'};
//__flash char com_ATZ[] = {'A','T','Z'};
//__flash char com_ATH[] = {'A','T','H'};
//__flash char com_ATD[] = {'A','T','D','*','9','9','*','*','*','1','#'};
//__flash char peer_id[] = {'b','e','e','l','i','n','e'};
//__flash char password[] = {'b','e','e','l','i','n','e'};

//__flash char ip_serv[] =    {'r','0',' ','I','P','
//','s','e','r','v','e','r','-','-','>' };
//__flash char portik_udp[] = {'r','1',' ','U','D','P','
//','p','o','r','t','-','-','-','>' };
//__flash char in_group[] =   {'r','2',' ','G','R','
//','N','U','M','-','-','-','-','-','>' };
//__flash char in_self[] =    {'r','3','
//','N','U','M','B','E','R','-','-','-','-','-','>' };
//__flash char user_name[] =  {'r','4',' ','U','s','e','r','
//','n','a','m','e','-','-','>' };
//__flash char passord[] =    {'r','5','
//','P','a','s','s','w','o','r','d','-','-','-','>' };
//__flash char init_line[] =  {'r','6',' ','I','n','i','t','
//','l','i','n','e','-','-','>' };
//__flash char ctrl_time[] = {'r','7',' ','C','t','r','l','
//','t','i','m','e','-','-','>' };
//__flash char rs_485_par[] = {'r','8','
//','R','S','-','4','8','5','-','-','-','-','-','>' };
//__flash char rx_out_tm[] =  {'r','9','
//','R','X','-','O','U','T','-','-','-','-','-','>' };
//__flash char tx_out_tm[] =  {'r','a','
//','T','X','-','O','U','T','-','-','-','-','-','>' };
//__flash char rcv_out_tm[] = {'r','b','
//','R','C','V','-','O','U','T','-','-','-','-','>' };

//_flash char err[] = {0xd,0xa,'E','R','R','O','R',0xd,0xa};
//__flash char ok[] = {0xd,0xa,'O','K',0xd,0xa};
//__flash char mks[] = {' ','m','k','s'};
//__flash char ms[] = {' ','m','s'};

__flash unsigned char tab_crc[] = {
    0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,
    65,  157, 195, 33,  127, 252, 162, 64,  30,  95,  1,   227, 189, 62,  96,
    130, 220, 35,  125, 159, 193, 66,  28,  254, 160, 225, 191, 93,  3,   128,
    222, 60,  98,  190, 224, 2,   92,  223, 129, 99,  61,  124, 34,  192, 158,
    29,  67,  161, 255, 70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,
    102, 229, 187, 89,  7,   219, 133, 103, 57,  186, 228, 6,   88,  25,  71,
    165, 251, 120, 38,  196, 154, 101, 59,  217, 135, 4,   90,  184, 230, 167,
    249, 27,  69,  198, 152, 122, 36,  248, 166, 68,  26,  153, 199, 37,  123,
    58,  100, 134, 216, 91,  5,   231, 185, 140, 210, 48,  110, 237, 179, 81,
    15,  78,  16,  242, 172, 47,  113, 147, 205, 17,  79,  173, 243, 112, 46,
    204, 146, 211, 141, 111, 49,  178, 236, 14,  80,  175, 241, 19,  77,  206,
    144, 114, 44,  109, 51,  209, 143, 12,  82,  176, 238, 50,  108, 142, 208,
    83,  13,  239, 177, 240, 174, 76,  18,  145, 207, 45,  115, 202, 148, 118,
    40,  171, 245, 23,  73,  8,   86,  180, 234, 105, 55,  213, 139, 87,  9,
    235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,  233,
    183, 85,  11,  136, 214, 52,  106, 43,  117, 151, 201, 74,  20,  246, 168,
    116, 42,  200, 150, 21,  75,  169, 247, 182, 232, 10,  84,  215, 137, 107,
    53};

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

__flash unsigned char auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40};

__flash unsigned char auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40};

__flash unsigned char keys[] = {124, 134, 184, 128, 226, 42, 100, 185};

__flash unsigned char s1[] = {0, 15, 1, 14, 2, 13, 3, 12,
                              4, 11, 5, 10, 6, 9,  7, 8};

__flash unsigned char s2[] = {1, 9,  2, 10, 3, 11, 4, 12,
                              5, 13, 6, 14, 7, 15, 8, 0};

#pragma location = FIRST_ON
__eeprom __no_init unsigned char e_first_on;

#pragma location = A_RW_PDP
__eeprom __no_init unsigned char e_rw_pdp;

// #pragma location=ADR_NUM_SELF
//__eeprom __no_init unsigned int e_num_self;
// #pragma location=ADR_NUM_DST_SEQ
//__eeprom __no_init unsigned int e_num_dst_seq;

#pragma location = ADR_CNTR_LINK
__eeprom __no_init unsigned int e_cntr_link;
#pragma location = ADR_CNTR_LINK_WAITS
__eeprom __no_init unsigned int e_cntr_link_waits;
#pragma location = ADR_CNTR_VOL_TRY
__eeprom __no_init unsigned char e_cntr_vol_try;
#pragma location = ADR_CNTR_NAT
__eeprom __no_init unsigned int e_cntr_nat;

#pragma location = ADR_VOL_TM_VZAT
__eeprom __no_init unsigned int e_vol_tm_vzat;

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

unsigned char count_block_net, byte_state_net;

unsigned char kamintel;

struct {
  unsigned char ip : 1;
  unsigned char udp : 1;
  unsigned char num_self : 1;
  unsigned char cnt_reset;
} fl_rewrite;

struct {
  unsigned char clr_wdt : 1;
  unsigned char fl_out : 1;
  unsigned char from_timer0 : 1;
  unsigned char fl_main : 1;
  unsigned char from_timer3 : 1;
  unsigned char from_timer2 : 1;
} fl_wdt;

unsigned char glutch;

unsigned char cnt_term;

unsigned char cnt_clr_wdt, cnt_flag_out;

unsigned int prov_ozu;

extern unsigned char point_log_buf;

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

unsigned int num_self, num_seq_cl, port_udp;

union {
  unsigned char bytes[4];
  unsigned long int word;
} temp1;

// unsigned int proverka;

unsigned char buf_rx_232[30];

unsigned char buf_tx_232[VOL_TX_PPP];

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

unsigned char temp;

unsigned int kol_rx_232, count_tx_ppp, vol_tx_ppp;
unsigned char count_rx_232;

unsigned char layer_PPP;

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

unsigned char ip_self[4], ip_pri_dns[4], ip_sec_dns[4], ip_serv_gprs[4],
    ip_ls[4];

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
  unsigned char add_byte : 1;
} fl_reg3;

struct {
  unsigned char on : 1;
} fl_cts_232;

enum bool fl_cts_232_ignor;
unsigned int cnt_cts_off;

void monitor_beg_state_seq(unsigned char zad);

extern void boot(void);
extern void r_p_flash(unsigned int adress);
extern void reload_apl(void);

extern unsigned char ret_version(unsigned char unit, unsigned char *ptr);
unsigned int crc_ozu(void);

extern void set_rlt(unsigned char address, unsigned char *data);

extern void write_log_info(unsigned char sost, unsigned char mesto);
extern void serch_point_log(void);

extern void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                           unsigned int num, unsigned char flag,
                           unsigned char znach);
extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                             unsigned int num);

extern void vosstan_memory(void);
extern unsigned char check_memory_map(void);
extern void monitor_terminal(void);
extern void sending_at_pac(void);
// extern unsigned char modem_com_init(void);
// extern void delay_modem_on(void);
// extern void delay_modem_off(void);
extern void init_scen_stm_si(void);
extern void init_scen_stm_sc(void);
extern void init_scen_stm_dc(void);
extern void at_com_rx(unsigned char cnt);
extern void at_com_tx(unsigned char cnt);
extern unsigned char at_com_scen_init(unsigned char *cnt, unsigned char *rp);
extern unsigned char at_com_scen_stm_sc(unsigned char *cnt, unsigned char *rp);
extern unsigned char at_com_scen_stm_dc(unsigned char *cnt, unsigned char *rp);

extern void init_scen_stm_dc_1(void);
extern unsigned char at_com_scen_stm_dc_1(unsigned char *cnt,
                                          unsigned char *rp);

extern unsigned long int burst_ds_r(void);
extern void funct_ds(char address, char num);
extern void delay(unsigned int period);

unsigned int calc_crc_udp_2(unsigned char *buf_rx_ppp, unsigned int count);
unsigned char read_eeprom(unsigned int adresok);
unsigned int calc_crc_ip(unsigned char *p, unsigned int count);
unsigned int pppfcs16(unsigned int fcs, unsigned char *cp, unsigned int len);
unsigned char check_cts(void);
void send_err485(unsigned char port, unsigned char err, unsigned char id,
                 unsigned int dst);
/// описание!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

enum t_version { VER1 = 1, VER2, VER3, VER4 };
enum t_type { ZAPR, OTV, SOOB, KVIT };
enum bool { FALSE, TRUE };

enum t_state_modem {
  STM_NONE = 0,
  STM_ON,
  STM_SS,
  STM_SI,
  STM_SC,
  STM_SCP,
  STM_PPP,
  STM_RDC,
  STM_DC,
  STM_OFF
};

enum t_state_modem state_modem;
enum t_event_modem event_modem;

unsigned char cnt_stm_tm1;

extern unsigned char fl_at_mom_232;

// unsigned char res_return;
extern unsigned char cnt_com;
extern unsigned char rep;

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
  unsigned char ok : 1;     // требуемый ответ
  unsigned char err : 1;    // ошибочный, нетребуемый ответ
  unsigned char tm_out : 1; // отсутствие ответа
  unsigned char tx_en : 1;  // послать комманду
  unsigned char rx_en : 1;  // принимать ответы
  unsigned char rx_rec : 1; // принят ответ
  // unsigned char tm_out_en:1;//разрешение анализа по превышению времени
  // ожидания ответа
} fl_at_com;

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

struct // структура, описывающая объект передачи по PPP
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
} Control;

unsigned char buf_opt_tr[20];

unsigned char Appl_RS485_1_data_buf[LN_BUF_485_1]; // сам буфер
unsigned char Appl_RS485_2_data_buf[LN_BUF_485_2]; // сам буфер

struct // структура описывающая работу приложения "RS485_1"
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
} Appl_RS485_1, Appl_RS485_2;

unsigned int delay_pre_tx;

struct {
  unsigned char busy : 1;
  unsigned char rec : 1;
  unsigned char tm_out : 1;
  unsigned char tx : 1;
  unsigned char over : 1;
  unsigned char buffed : 1;
} fl_485_1, fl_485_2, fl_232_2;

struct {
  unsigned char mon1 : 1;
  unsigned char mon2 : 1;
} fl_tx485;

unsigned char Rs485_1_buf_rx_tx[MAX_BUF_RS485_1];
unsigned char Rs485_2_buf_rx_tx[MAX_BUF_RS485_2];
unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];

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
} Rs485_1, Rs485_2;

struct // структура описывающая работу эмулированного порта "RS232_1"
{
  unsigned int cnt_bt_rx_tx;  // счетчик байтов на прием-передачу
  unsigned int cnt_tm_rx_out; //  счетчик времени на определение конца приема
  unsigned int vol_tm_rx_out; //  предел времени на определение конца приема
  unsigned char *p_data232;   // указатель на буфер передачи
} Rs232_2;

// структуры буферов на прием
struct {
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

// эмуляторы портов
struct {
  unsigned int bod;       // задатчик скорости
  unsigned int shift_reg; // сдвиговый регистр битов
  enum bool dir_tx;       // передача
  unsigned char cnt_bit;  // счетчик битов
  unsigned char fl_bit_chet;
  unsigned char bit_par;
} Emul_485, Emul_232;

unsigned char Emul_232_cnt_post_tx; // защитный интервал на переключения на
                                    // прием, чтоб не всасывались куски

struct {
  unsigned char sinc : 1;   // флаг начала посылки байта
  unsigned char was_tx : 1; // флаг того что была передача
} fl_232_2_add;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

struct {
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

unsigned char Appl_seq_des;

unsigned char state_seq;

unsigned char Appl_seq_buf[MAX_BUF_SEQ];

struct {
  unsigned char en_tx : 1;
  unsigned char kv_waits : 1;
  unsigned char en_povtor : 1;
} fl_appl_seq;

struct {
  unsigned char over_buf : 1;
  unsigned char send_state : 1;
} fl_appl_seq1;

struct {
  char scanb[2];
} ru;

unsigned char g2[16], count;

union {
  char arr_tc[4];
  struct {
    char b0 : 1;
    char b1 : 1;
    char b2 : 1;
    char b3 : 1;
    char b41 : 1;
    char b51 : 1;
    char b61 : 1;
    char b71 : 1;

    char b4 : 1;
    char b5 : 1;
    char b6 : 1;
    char b7 : 1;
    char b42 : 1;
    char b52 : 1;
    char b62 : 1;
    char b72 : 1;

    char b8 : 1;
    char b9 : 1;
    char b10 : 1;
    char b11 : 1;
    char b43 : 1;
    char b53 : 1;
    char b63 : 1;
    char b73 : 1;

    char b12 : 1;
    char b13 : 1;
    char b14 : 1;
    char b15 : 1;
    char b44 : 1;
    char b54 : 1;
    char b64 : 1;
    char b74 : 1;
  } bit;
} tek_tc;

union {
  char arr_tc[4];
  struct {
    char b0 : 1;
    char b1 : 1;
    char b2 : 1;
    char b3 : 1;
    char b41 : 1;
    char b51 : 1;
    char b61 : 1;
    char b71 : 1;

    char b4 : 1;
    char b5 : 1;
    char b6 : 1;
    char b7 : 1;
    char b42 : 1;
    char b52 : 1;
    char b62 : 1;
    char b72 : 1;

    char b8 : 1;
    char b9 : 1;
    char b10 : 1;
    char b11 : 1;
    char b43 : 1;
    char b53 : 1;
    char b63 : 1;
    char b73 : 1;

    char b12 : 1;
    char b13 : 1;
    char b14 : 1;
    char b15 : 1;
    char b44 : 1;
    char b54 : 1;
    char b64 : 1;
    char b74 : 1;

  } bit;
} real_tc;

union {
  unsigned char bytes[2];
  unsigned int word;
} sv_tc, sv_tc_old;

enum bool command_AT;

void st_gprs_no(void) {
  SVD1_1_OFF; // зеленый ближе к процессору
  SVD1_2_OFF;
  SVD2_1_OFF;
  SVD2_2_OFF;
}

void st_gprs_atcom(void) {
  SVD1_1_OFF; // зеленый ближе к процессору
  SVD1_2_ON;
  SVD2_1_OFF;
  SVD2_2_OFF;
}

void st_gprs_att(void) {
  SVD1_1_ON; // зеленый ближе к процессору
  SVD1_2_ON;
}

void st_ppp_connect(void) {
  SVD1_1_ON; // зеленый ближе к процессору
  SVD1_2_OFF;
  SVD2_1_OFF;
  SVD2_2_OFF;
}

unsigned int crc_m1(unsigned char *ka, unsigned int num, unsigned int crc)

{
  unsigned char uchCRCHi, uIndex;
  unsigned char uchCRCLo;
  unsigned int ii;

  uchCRCLo = crc;
  uchCRCHi = crc >> 8;

  for (ii = 0; ii < num; ii++) {

    uIndex = uchCRCHi ^ *ka++;

    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];

    uchCRCLo = auchCRCLo[uIndex];
  }

  return (uchCRCHi << 8 | uchCRCLo);
}

// подготовка буфера к отправке по PPP

void form_buf_tx_ppp(void) {
  unsigned int i;
  unsigned char nopp, j;
  unsigned char *pointer;
  union {
    unsigned char bytes[2];
    unsigned int word;
  } temp;

  buf_tx_232[PPP_BEG] = 0x7e;
  buf_tx_232[PPP_PROT] = 0x21;

  // заголовок IP
  buf_tx_232[IP_VLEN] = 0x45;
  buf_tx_232[IP_TYPE] = 0;

  // общая длина IP
  // 20 -IP
  // 8 -UDP
  // 12 - Заголовок Данных
  //+ Длина буфера опций
  //+ Заполнитель
  //+ Длина данных
  nopp = 0;

  // Расчет заполнителя
  if (Obj_ppp_tx.prozr == FALSE) {
    if ((Obj_ppp_tx.l_opt <= 4) && (Obj_ppp_tx.l_opt > 0))
      nopp = 4 - Obj_ppp_tx.l_opt;
    if (Obj_ppp_tx.l_opt % 4 != 0)
      nopp = 4 - Obj_ppp_tx.l_opt % 4;
    temp.word = 20 + 8 + 12 + Obj_ppp_tx.l_opt + nopp + Obj_ppp_tx.l_data;
  } else
    temp.word = 20 + 8 + Obj_ppp_tx.l_data;

  buf_tx_232[IP_LN] =
      temp.bytes[1]; // длина IP пакета         ///////////////!!!!!!!!!!!!!!!!!
  buf_tx_232[IP_LN + 1] = temp.bytes[0]; ///////////////!!!!!!!!!!!!!!!!!

  buf_tx_232[IP_ID] = 0;
  buf_tx_232[IP_ID + 1] = 0;

  buf_tx_232[IP_FUK] = 0;
  buf_tx_232[IP_FUK + 1] = 0;
  buf_tx_232[IP_TIME] = 0x80;
  buf_tx_232[IP_PROT] = 0x11; // UDP

  // buf_tx_232[IP_CRC]=0;
  // buf_tx_232[IP_CRC+1]=0; //IP - CRC
  *(unsigned int *)&buf_tx_232[IP_CRC] = 0;

  buf_tx_232[IP_SRC] = ip_self[0];
  buf_tx_232[IP_SRC + 1] = ip_self[1];
  buf_tx_232[IP_SRC + 2] = ip_self[2];
  buf_tx_232[IP_SRC + 3] = ip_self[3];

  buf_tx_232[IP_DST] = ip_ls[0];
  buf_tx_232[IP_DST + 1] = ip_ls[1];
  buf_tx_232[IP_DST + 2] = ip_ls[2];
  buf_tx_232[IP_DST + 3] = ip_ls[3];

  // buf_tx_232[18]=read_eeprom(ADR_IP_SRV);
  // buf_tx_232[19]=read_eeprom(ADR_IP_SRV+1);
  // buf_tx_232[20]=read_eeprom(ADR_IP_SRV+2);
  // buf_tx_232[21]=read_eeprom(ADR_IP_SRV+3);

  // заголовок UDP
  // buf_tx_232[22]=UDP_PORT_H;
  // buf_tx_232[23]=UDP_PORT_L;
  // buf_tx_232[24]=UDP_PORT_H;
  // buf_tx_232[25]=UDP_PORT_L;
  /*
  buf_tx_232[UDP_PSRC]=read_eeprom(ADR_UDP_PORT_HI);
  buf_tx_232[UDP_PSRC+1]=read_eeprom(ADR_UDP_PORT_LO);
  buf_tx_232[UDP_PDST]=read_eeprom(ADR_UDP_PORT_HI);
  buf_tx_232[UDP_PDST+1]=read_eeprom(ADR_UDP_PORT_LO);
  */
  buf_tx_232[UDP_PSRC] = port_udp >> 8;
  buf_tx_232[UDP_PSRC + 1] = port_udp;
  buf_tx_232[UDP_PDST] = port_udp >> 8;
  buf_tx_232[UDP_PDST + 1] = port_udp;

  // общая длина UDP

  // 8 -UDP
  // 12 - Заголовок Данных
  // + Длина буфера опций
  // + Заполнитель
  // + Длина данных

  temp.word = temp.word - 20;

  buf_tx_232[UDP_LN] = temp.bytes[1];     ///////////////длина UDP
  buf_tx_232[UDP_LN + 1] = temp.bytes[0]; ////////////

  // buf_tx_232[UDP_CRC]=0;        //СRС UDP пакета
  // buf_tx_232[UDP_CRC+1]=0;
  *(unsigned int *)&buf_tx_232[UDP_CRC] = 0;

  if (Obj_ppp_tx.prozr == FALSE) {
    // Транспортный заголовок
    // buf_tx_232[30]=0;        //CRC
    // buf_tx_232[31]=0;
    //*(unsigned int*)&buf_tx_232[TR_CRC]=0;

    temp.word = temp.word - 8;
    // buf_tx_232[32]=temp.bytes[1];        //полная длина
    // buf_tx_232[33]=temp.bytes[0];
    *(unsigned int *)&buf_tx_232[TR_LEN] = temp.word;

    // buf_tx_232[34]=0;        //шифр
    // buf_tx_232[35]=0;

    *(unsigned int *)&buf_tx_232[TR_KOD] = 0;

    buf_tx_232[TR_V] = Obj_ppp_tx.version;                            // версия
    buf_tx_232[TR_V] = (buf_tx_232[TR_V] << 2) | Obj_ppp_tx.type_pac; // тип
    buf_tx_232[TR_V] =
        (buf_tx_232[TR_V] << 4) |
        ((12 + Obj_ppp_tx.l_opt + nopp) / 4); // длина траспортного заголовка

    buf_tx_232[TR_ID] = Obj_ppp_tx.id_pac;

    *(unsigned int *)&buf_tx_232[TR_SRC] =
        Obj_ppp_tx.num_src; // номер отправителя
    *(unsigned int *)&buf_tx_232[TR_DST] =
        Obj_ppp_tx.num_dst; // номер получателя

    pointer = &buf_tx_232[TR_OP_DATA];

    // Забивка опции и заполнителя
    if (Obj_ppp_tx.l_opt != 0) {
      for (j = 0; j < Obj_ppp_tx.l_opt; j++) {
        *pointer = *Obj_ppp_tx.p_opt;
        pointer++;
        Obj_ppp_tx.p_opt++;
      }
      if (nopp != 0)
        for (j = 0; j < nopp; j++) {
          *pointer = NOPPER;
          pointer++;
        }
    }

    // Забивка данных
    for (i = 0; i < Obj_ppp_tx.l_data; i++) {
      *pointer = *Obj_ppp_tx.p_data;
      pointer++;
      Obj_ppp_tx.p_data++;
    }

    // шифровка
    //  i=rand();
    //  transform_buf(&buf_tx_232[36],temp.word-6;,i);
    //  buf_tx_232[34]=temp.bytes[1];
    //  buf_tx_232[35]=temp.bytes[0];
    // шифровка

    // crc
    *(unsigned int *)&buf_tx_232[TR_CRC] = crc_m1(
        &buf_tx_232[TR_LEN], *(unsigned int *)&buf_tx_232[TR_LEN] - 2, 0xffff);
  } else {

    pointer = &buf_tx_232[TR_CRC];
    for (i = 0; i < Obj_ppp_tx.l_data; i++) {
      *pointer = *Obj_ppp_tx.p_data;
      pointer++;
      Obj_ppp_tx.p_data++;
    }
  }

  // temp.word=calc_crc_ip(&buf_tx_232[2],20);
  // buf_tx_232[12]=temp.bytes[0];
  // buf_tx_232[13]=temp.bytes[1];

  *(unsigned int *)&buf_tx_232[IP_CRC] = calc_crc_ip(&buf_tx_232[IP_VLEN], 20);

  temp.bytes[1] = buf_tx_232[UDP_LN]; ///////////////длина UDP
  temp.bytes[0] = buf_tx_232[UDP_LN + 1];

  // *(unsigned
  // int*)&buf_tx_232[UDP_CRC]=calc_crc_udp(&buf_tx_232[UDP_PSRC],temp.word);
  *(unsigned int *)&buf_tx_232[UDP_CRC] =
      calc_crc_udp_2(&buf_tx_232[0], temp.word);

  i = buf_tx_232[IP_LN];
  i = (i << 8) +
      buf_tx_232[IP_LN + 1]; // длина IP пакета ///////////////!!!!!!!!!!!!!!!!!
  i = i + 5; // длина PPP пакета         ///////////////!!!!!!!!!!!!!!!!!

  temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[PPP_PROT], i - 4) ^
              0xffff; // //длина IP+1 т.е
  buf_tx_232[i - 3] = temp.bytes[0];
  buf_tx_232[i - 2] = temp.bytes[1];
  buf_tx_232[i - 1] = 0x7e;

  vol_tx_ppp = i;
  count_tx_ppp = 0;
}

void clr_cntr_link_2(void) {
  Control.link_waits = FALSE;
  Control.link = FALSE;
  Control.cnt_link = Control.vol_link;
  Control.cnt_try = 0;
  Control.link_no = FALSE;
}

void clr_cntr_link(void) {
  if (Control.link_waits == FALSE) {
    Control.link = FALSE;
    Control.cnt_link = Control.vol_link;
  }
}

void clr_cntr_nat(void) {
  Control.nat = FALSE;
  Control.cnt_nat = Control.vol_nat;
}

void monitor_cntr_ch(void) {

  if ((fl_lcp.act_lcp_end != 1) && (fl_pap.act_pap_end != 1) ||
      (fl_ipcp.act_ipcp_end != 1) || (fl_ip.act_ip_end != 1))
    return;

  if (Control.link_waits == TRUE)
    return;
  if (Control.link == FALSE)
    return;
  Control.link = FALSE;

  clr_cntr_nat();

  fl_ip.act_ip_end = 0;

  Obj_ppp_tx.prozr = FALSE;          // версия
  Obj_ppp_tx.version = VER1;         // версия
  Obj_ppp_tx.type_pac = SOOB;        // тип пакета
  Obj_ppp_tx.num_src = num_self;     // номер отправителя
  Obj_ppp_tx.num_dst = NUM_SRV_LINK; // номер получателя
  Obj_ppp_tx.id_pac = ID_CC;         // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций

  Obj_ppp_tx.l_opt = 0; // длина буфера опций
  buf_opt_tr[0] = 1;
  buf_opt_tr[1] = 2;
  buf_opt_tr[2] = 3;
  buf_opt_tr[3] = 4;
  buf_opt_tr[4] = 5;
  buf_opt_tr[5] = 6;
  buf_opt_tr[6] = 7;
  buf_opt_tr[7] = 8;
  buf_opt_tr[8] = 9;
  buf_opt_tr[9] = 10;
  buf_opt_tr[10] = 11;

  Obj_ppp_tx.kol_opt = 0; // количество опций

  Obj_ppp_tx.l_data = 0;                       // длина данных
  Obj_ppp_tx.p_data = &buf_tx_232[TR_OP_DATA]; // указатель буфера данных

  form_buf_tx_ppp();

  Control.cnt_link = Control.vol_waits;
  Control.link_waits = TRUE;
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void monitor_cntr_nat(void) {
  unsigned char temp = NOPPER;
  if ((fl_lcp.act_lcp_end != 1) && (fl_pap.act_pap_end != 1) ||
      (fl_ipcp.act_ipcp_end != 1) || (fl_ip.act_ip_end != 1))
    return;

  if (Control.nat == FALSE)
    return;
  clr_cntr_nat();
  fl_ip.act_ip_end = 0;

  Obj_ppp_tx.prozr = TRUE;
  Obj_ppp_tx.l_data = 1;     // длина данных
  Obj_ppp_tx.p_data = &temp; // указатель буфера данных

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void monitor_cntr_cl_seq(void) {

  if ((fl_lcp.act_lcp_end != 1) && (fl_pap.act_pap_end != 1) ||
      (fl_ipcp.act_ipcp_end != 1) || (fl_ip.act_ip_end != 1))
    return;
  if (Appl_seq.cntr_cl == FALSE)
    return;
  if (Appl_seq.en_cntr_cl == FALSE)
    return;
  Appl_seq.cnt_cntr_cl = Appl_seq.vol_cntr_cl;
  Appl_seq.en_cntr_cl = FALSE;

  if (Appl_seq_des == 1)
    return; // блокировка

  clr_cntr_nat();
  fl_ip.act_ip_end = 0;
  Obj_ppp_tx.prozr = FALSE;          // версия
  Obj_ppp_tx.version = VER1;         // версия
  Obj_ppp_tx.type_pac = ZAPR;        // тип пакета
  Obj_ppp_tx.num_src = num_self;     // номер отправителя
  Obj_ppp_tx.num_dst = NUM_SRV_LINK; // номер получателя
  Obj_ppp_tx.id_pac = 0;             // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций
  Obj_ppp_tx.l_opt = 0;              // длина буфера опций
  buf_opt_tr[0] = PROT_SL_LS;
  buf_opt_tr[1] = COM_CNTR_CL;
  *(unsigned int *)&buf_opt_tr[2] = L_COM_CNTR_CL;
  *(unsigned int *)&buf_opt_tr[4] = num_seq_cl;
  Obj_ppp_tx.kol_opt = 0;             // количество опций
  Obj_ppp_tx.l_data = 6;              // длина данных
  Obj_ppp_tx.p_data = &buf_opt_tr[0]; // указатель буфера данных
  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void sending_ppp_pac(void);

/*
void delay(unsigned int period)     //6 тактов - 0,375 мкс
{
bad:
period--;
if(period !=0) goto bad;
}
*/

void lock_it(void) {
  do {
    SREG = 0;     // пока
    WDTCR = 0x18; // пока
    WDTCR = 0x0f; // пока
  } while (1);
}

void write_eeprom(unsigned int adresok, unsigned char datok) {

  union {
    unsigned char b_adres_ee[2];
    unsigned int adres_ee;
  } st_eeprom;

  st_eeprom.adres_ee = adresok;
  while (EECR & 2)
    ;
  EEARH = st_eeprom.b_adres_ee[1];
  EEARL = st_eeprom.b_adres_ee[0];
  EEDR = datok;
  __disable_interrupt();
  EECR = EECR | 4;
  __no_operation();
  EECR = EECR | 2;
  __enable_interrupt();
}

unsigned char check_cont_485_1(unsigned char *pointer) {

  switch (*pointer) {
  case B2400:
  case B4800:
  case B9600:
  case B19200:
    break;
  default:
    return (1);
  }

  switch ((*(pointer + 1)) & 0x07) {
  case NON:
  case ODD:
  case EVEN:
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 3) & 0x03) {
  case STOP1:
  case STOP2:
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 5) & 0x03) {
  case INF7:
  case INF8:
    break;
  default:
    return (1);
  }

  if ((*(unsigned int *)(pointer + 2)) > 10000)
    return (1);

  if ((*(unsigned int *)(pointer + 4)) == 0)
    return (1);
  if ((*(unsigned int *)(pointer + 4)) > 10000)
    return (1);

  if ((*(unsigned int *)(pointer + 6)) < 3)
    return (1);

  return (0);
}

unsigned char run_cont_485_1(unsigned char *pointer) {

  switch ((*(pointer + 1)) & 0x07) {
  case NON:
    UCSR1C = UCSR1C & ~0x30;
    break;
  case ODD:
    UCSR1C = UCSR1C | 0x30;
    break;
  case EVEN:
    UCSR1C = UCSR1C & ~0x30;
    UCSR1C = UCSR1C | 0x20;
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 3) & 0x03) {
  case STOP1:
    UCSR1C = UCSR1C & ~0x08;
    break;
  case STOP2:
    UCSR1C = UCSR1C | 0x08;
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 5) & 0x03) {
  case INF7:
    UCSR1C = UCSR1C & ~0x06;
    UCSR1C = UCSR1C | 0x04;
    break;
  case INF8:
    UCSR1C = UCSR1C | 0x06;
    break;
  default:
    return (1);
  }

  if ((*(unsigned int *)(pointer + 2)) > 10000)
    return (1);
  Rs485_1.cnt_tm_tx_out = 0;
  Rs485_1.vol_tm_tx_out = *(unsigned int *)(pointer + 2);

  if ((*(unsigned int *)(pointer + 4)) == 0)
    return (1);
  if ((*(unsigned int *)(pointer + 4)) > 10000)
    return (1);
  Rs485_1.cnt_tm_rx_out = 0;
  Rs485_1.vol_tm_rx_out = *(unsigned int *)(pointer + 4);

  if ((*(unsigned int *)(pointer + 6)) < 3)
    return (1);
  Rs485_1.cnt_tm_out = 0;
  Rs485_1.vol_tm_out = *(unsigned int *)(pointer + 6); // 1000 миллисекунд

  switch (*pointer) {
  case B2400:
    UBRR1H = R2400_H;
    UBRR1L = R2400_L;

    if (Appl_RS485_1.pre_tx != 0) {
      // перехват порта для лафкина

      __disable_interrupt();
      lufkin_port_on;
      __enable_interrupt();

      Rs485_1.cnt_tm_pre_tx = Appl_RS485_1.pre_tx; // задержка перед передачей
    } else
      Rs485_1.cnt_tm_pre_tx = 17; // 17 миллисекунд

    Rs485_1.vol_tm_tx_out = Rs485_1.vol_tm_tx_out + 5;
    break;
  case B4800:
    UBRR1H = R4800_H;
    UBRR1L = R4800_L;

    if (Appl_RS485_1.pre_tx != 0) {
      // перехват порта для лафкина
      __disable_interrupt();
      lufkin_port_on;
      __enable_interrupt();

      Rs485_1.cnt_tm_pre_tx = Appl_RS485_1.pre_tx; // задержка перед передачей
    } else
      Rs485_1.cnt_tm_pre_tx = 9; // 9 миллисекунд

    Rs485_1.vol_tm_tx_out = Rs485_1.vol_tm_tx_out + 3;
    break;
  case B9600:
    UBRR1H = R9600_H;
    UBRR1L = R9600_L;

    if (Appl_RS485_1.pre_tx != 0) {
      // перехват порта для лафкина
      __disable_interrupt();
      lufkin_port_on;
      __enable_interrupt();

      Rs485_1.cnt_tm_pre_tx = Appl_RS485_1.pre_tx; // задержка перед передачей
    } else
      Rs485_1.cnt_tm_pre_tx = 4; // 4 миллисекунд

    Rs485_1.vol_tm_tx_out = Rs485_1.vol_tm_tx_out + 2;
    break;
  case B19200:
    UBRR1H = R19200_H;
    UBRR1L = R19200_L;

    if (Appl_RS485_1.pre_tx != 0) {
      // перехват порта для лафкина

      __disable_interrupt();
      lufkin_port_on;
      __enable_interrupt();

      Rs485_1.cnt_tm_pre_tx = Appl_RS485_1.pre_tx; // задержка перед передачей
    } else
      Rs485_1.cnt_tm_pre_tx = 2; // 2 миллисекунд

    Rs485_1.vol_tm_tx_out = Rs485_1.vol_tm_tx_out + 1;
    break;
  default:
    return (1);
  }

  return (0);
}

unsigned char check_cont_485_2(unsigned char *pointer) {

  switch (*pointer) {
  case B2400:
  case B4800:
  case B9600:
    break;
  default:
    return (1);
  }

  switch ((*(pointer + 1)) & 0x07) {
  case NON:
  case ODD:
  case EVEN:
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 3) & 0x03) {
  case STOP1:
    break;
  default:
    return (1);
  }

  switch (((*(pointer + 1)) >> 5) & 0x03) {
  case INF8:
    break;
  default:
    return (1);
  }

  if ((*(unsigned int *)(pointer + 2)) > 10000)
    return (1);

  if ((*(unsigned int *)(pointer + 4)) == 0)
    return (1);
  if ((*(unsigned int *)(pointer + 4)) > 10000)
    return (1);

  if ((*(unsigned int *)(pointer + 6)) < 3)
    return (1);

  return (0);
}

unsigned char run_cont_485_2(unsigned char *pointer) {

  switch ((*(pointer + 1)) & 0x07) {
  case NON:
    Emul_485.fl_bit_chet = 0;
    break;
  case ODD:
    Emul_485.fl_bit_chet = 1;
    break;
  case EVEN:
    Emul_485.fl_bit_chet = 2;
    break;
  default:
    return (1);
  }

  if ((*(unsigned int *)(pointer + 2)) > 10000)
    return (1);
  Rs485_2.cnt_tm_tx_out = 0;
  Rs485_2.vol_tm_tx_out = *(unsigned int *)(pointer + 2);

  if ((*(unsigned int *)(pointer + 4)) == 0)
    return (1);
  if ((*(unsigned int *)(pointer + 4)) > 10000)
    return (1);
  Rs485_2.cnt_tm_rx_out = 0;
  Rs485_2.vol_tm_rx_out = *(unsigned int *)(pointer + 4);

  if ((*(unsigned int *)(pointer + 6)) < 3)
    return (1);
  Rs485_2.cnt_tm_out = 0;
  Rs485_2.vol_tm_out = *(unsigned int *)(pointer + 6);

  switch (*pointer) {
  case B2400:
    Emul_485.bod = 6667;
    Rs485_2.cnt_tm_pre_tx = 17; // 17 миллисекунд
    break;
  case B4800:
    Emul_485.bod = 3333;
    Rs485_2.cnt_tm_pre_tx = 9; // 9 миллисекунд
    break;
  case B9600:
    // Emul_485.bod=1666;
    Emul_485.bod = 1600;
    Rs485_2.cnt_tm_pre_tx = 4; // 4 миллисекунд
    break;

  default:
    return (1);
  }

  return (0);
}

unsigned char check_ln_conf(unsigned char *buf_rx_ppp, unsigned char offset,
                            unsigned int count_rx_ppp) {
  unsigned int ind;

  ind = offset + C1_DATA;
next_check_ln:
  ind = ind + 3 + *(unsigned int *)&buf_rx_ppp[ind + 1];
  if (ind > count_rx_ppp - 3)
    return (1);
  if (ind == count_rx_ppp - 3)
    return (0);
  goto next_check_ln;
}

unsigned int proc_config(unsigned char *buf_rx_ppp, unsigned char offset,
                         unsigned int count_rx_ppp) {

  unsigned int ind, temp, ofs, i;
  unsigned int cnt_bt;
  unsigned char buf[20];
  cnt_bt = 0;
  ind = offset + C1_DATA;
next_parametr:

  switch (buf_rx_ppp[ind] & 0x7f) {
  case CONF_VERS:
    if ((buf_rx_ppp[ind] & 0x80) == 0x80)
      goto bad_com;
    else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_RD_CONF_VERS)
        goto bad_com;
    }
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    buf[0] = ret_version(0, &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt]);
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        buf[0] + 1;
    cnt_bt = cnt_bt + buf[0] + 1 + 3;
    break;

  case CONF_MAP:
    if ((buf_rx_ppp[ind] & 0x80) == 0x80)
      goto bad_com;
    else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_RD_CONF_MAP)
        goto bad_com;
    }
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    buf[0] = ret_version(1, &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt]);
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        buf[0] + 1;
    cnt_bt = cnt_bt + buf[0] + 1 + 3;
    break;

  case CONF_OZU:
    if ((buf_rx_ppp[ind] & 0x80) == 0x80)
      goto bad_com;
    else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_RD_CONF_OZU)
        goto bad_com;
    }
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    buf[0] = ret_version(2, &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt]);
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        buf[0] + 1;
    cnt_bt = cnt_bt + buf[0] + 1 + 3;
    break;

  case CONF_TM_NAT:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_TM_NAT)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 10) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Control.vol_nat = temp;
      RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
      *(unsigned int *)&buf[OFS_NAT] = temp;
      *(unsigned int *)&buf[OFS_C_PAR_CRC] =
          crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
      WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_TM_NAT - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_TM_NAT + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_C_PAR + OFS_NAT,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_TM_NAT + 1 + 3;
    break;

  case CONF_TM_CC:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_TM_CC)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 10) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Control.vol_link = temp;
      RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
      *(unsigned int *)&buf[OFS_CCH] = temp;
      *(unsigned int *)&buf[OFS_C_PAR_CRC] =
          crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
      WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_TM_CC - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_TM_CC + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_C_PAR + OFS_CCH,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_TM_CC + 1 + 3;
    break;

  case CONF_TM_WT:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_TM_WT)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 5) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Control.vol_try = temp / Control.vol_waits;

      RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
      *(unsigned int *)&buf[OFS_TM_CH] = temp;
      *(unsigned int *)&buf[OFS_C_PAR_CRC] =
          crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
      WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_TM_WT - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_TM_WT + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_C_PAR + OFS_TM_CH,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_TM_WT + 1 + 3;
    break;

  case CONF_NUM_CL:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_NUM_CL)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 3) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      num_seq_cl = temp;

      RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
      *(unsigned int *)&buf[OFS_NUM_CL] = temp;
      *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
          crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
      WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_NUM_CL - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_NUM_CL + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_SEQ_PAR + OFS_NUM_CL,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_NUM_CL + 1 + 3;
    break;
  case CONF_TM_VZ:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_TM_VZ)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 1) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Appl_seq.vol_tm_vzat = temp;

      RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
      *(unsigned int *)&buf[OFS_TM_VZ] = temp;
      *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
          crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
      WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_TM_VZ - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_TM_VZ + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_SEQ_PAR + OFS_TM_VZ,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_TM_VZ + 1 + 3;
    break;

  case CONF_TM_CL:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_TM_CL)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 5) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Appl_seq.vol_cntr_cl = temp;

      RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
      *(unsigned int *)&buf[OFS_TM_CL] = temp;
      *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
          crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
      WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);
      prov_ozu = crc_ozu();
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_TM_CL - 2))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_TM_CL + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_SEQ_PAR + OFS_TM_CL,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);

    cnt_bt = cnt_bt + L_CONF_TM_CL + 1 + 3;
    break;

  case CONF_DES_OHR:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_DES_OHR)
        goto bad_com;
      // запись
      temp = buf_rx_ppp[ind + 3];

      if (temp > 2) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      Appl_seq_des = temp;

      RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
      buf[OFS_DES_SEQ] = temp;
      *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
          crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
      WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);
      prov_ozu = crc_ozu();

      if (Appl_seq_des != 1)
        monitor_beg_state_seq(1);

    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_DES_OHR - 1))
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_DES_OHR + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    RdFromFleshToArr(A_SEQ_PAR + OFS_DES_SEQ,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 1);

    cnt_bt = cnt_bt + L_CONF_DES_OHR + 1 + 3;
    break;

  case CONF_NUM_SELF:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_NUM_SELF)
        goto bad_com;
      // запись
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];

      if (temp < 3) {
        buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x01;
        goto nocorr_com;
      }

      fl_rewrite.num_self = 1;

      // WrArrayToFlesh(ABUF_NUM,&buf_rx_ppp[ind+3],L_CONF_NUM_SELF,0,0);
      // RdFromFleshToArr(ABUF_NUM,&buf_tx_232[TR_OP_DATA+C1_DATA+4+cnt_bt],2);

      RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
      *(unsigned int *)&buf[OFS_NUM] = temp;
      *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
          crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
      WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);
      //  prov_ozu=crc_ozu();

    } else if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_NUM_SELF - 2))
      goto bad_com;

    RdFromFleshToArr(A_IP_PAR + OFS_NUM,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_NUM_SELF + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK

    cnt_bt = cnt_bt + L_CONF_NUM_SELF + 1 + 3;
    break;

  case CONF_UDP:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_UDP)
        goto bad_com;

      // запись
      fl_rewrite.udp = 1;
      temp = *(unsigned int *)&buf_rx_ppp[ind + 3];
      // WrArrayToFlesh(ABUF_PORT, &buf_rx_ppp[ind+3],L_CONF_UDP,0,0);
      // RdFromFleshToArr(ABUF_PORT,&buf_tx_232[TR_OP_DATA+C1_DATA+4+cnt_bt],2);

      RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
      *(unsigned int *)&buf[OFS_PORT] = temp;
      *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
          crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
      WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);

    } else if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_UDP - 2))
      goto bad_com;

    RdFromFleshToArr(A_IP_PAR + OFS_PORT,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 2);
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_UDP + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK

    cnt_bt = cnt_bt + L_CONF_UDP + 1 + 3;
    break;

  case CONF_IP:

    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_CONF_IP)
        goto bad_com;

      fl_rewrite.ip = 1;

      // запись
      // WrArrayToFlesh(ABUF_IP, &buf_rx_ppp[ind+3],L_CONF_IP,0,0);
      // RdFromFleshToArr(ABUF_IP,&buf_tx_232[TR_OP_DATA+C1_DATA+4+cnt_bt],4);

      RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
      buf[OFS_IP] = buf_rx_ppp[ind + 3];
      buf[OFS_IP + 1] = buf_rx_ppp[ind + 4];
      buf[OFS_IP + 2] = buf_rx_ppp[ind + 5];
      buf[OFS_IP + 3] = buf_rx_ppp[ind + 6];
      *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
          crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
      WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);
      // prov_ozu=crc_ozu();

    } else if (*(unsigned int *)&buf_rx_ppp[ind + 1] != (L_CONF_IP - 4))
      goto bad_com;

    RdFromFleshToArr(A_IP_PAR + OFS_IP,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], 4);
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_CONF_IP + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK

    cnt_bt = cnt_bt + L_CONF_IP + 1 + 3;
    break;

  case CONF_PDP:
    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      goto bad_com;
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != 0)
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind]; // чтение
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00;        // OK

    RdFromFleshToArr(A_C_GPRS, &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt],
                     L_C_GPRS_MAX);

    ofs = TR_OP_DATA + C1_DATA + 4 + cnt_bt;
    temp = buf_tx_232[ofs];
    for (i = 0; i < temp; i++)
      buf_tx_232[ofs + i] = buf_tx_232[ofs + i + 1];
    buf_tx_232[ofs + i] = 0;

    ofs = ofs + temp + 1;
    temp = buf_tx_232[ofs];
    for (i = 0; i < temp; i++)
      buf_tx_232[ofs + i] = buf_tx_232[ofs + i + 1];
    buf_tx_232[ofs + i] = 0;

    ofs = ofs + temp + 1;
    temp = buf_tx_232[ofs];
    for (i = 0; i < temp; i++)
      buf_tx_232[ofs + i] = buf_tx_232[ofs + i + 1];
    buf_tx_232[ofs + i] = 0;

    ofs = ofs + temp + 1;
    ofs = ofs - (TR_OP_DATA + C1_DATA + 4 + cnt_bt);

    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] = ofs + 1;
    cnt_bt = cnt_bt + ofs + 1 + 3;

    break;

  case CONF_RESET:
    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != 0)
        goto bad_com;
      buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
      *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] = 1;
      buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
      cnt_bt = cnt_bt + 1 + 3;
      fl_rewrite.cnt_reset = 4;
      break;
    } else
      goto bad_com;

  case CONF_PDP_DOP:
    if ((buf_rx_ppp[offset + C1_DATA] & 0x80) == 0x80) {
      temp = *(unsigned int *)&buf_rx_ppp[ind + 1];

      if ((temp == 1) || (temp == 2))
        goto bad_com;
      if (temp > L_CONF_PDP)
        goto bad_com;

      if (temp == 0) {
        e_rw_pdp = 0xff;
        temp = L_CONF_PDP;
        WrArrayToFlesh(ABUF_C_GPRS, 0, temp, 0x01, 0xff);
        // записать во флешь признак перезаписи контекста
      }

      else {
        // запись
        WrArrayToFlesh(ABUF_C_GPRS, &buf_rx_ppp[ind + 3], temp, 0, 0);
        // записать во флешь признак перезаписи контекста
        e_rw_pdp = 0;
      }
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != 0)
        goto bad_com;
      temp = L_CONF_PDP;
    }

    RdFromFleshToArr(ABUF_C_GPRS,
                     &buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt], temp);
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] = temp + 1;
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK

    cnt_bt = cnt_bt + temp + 1 + 3;

    break;

  case CONF_TIME:
    if ((buf_rx_ppp[ind] & 0x80) == 0x80) {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_WR_CONF_TIME)
        goto bad_com;
      // запись времени
      set_rlt(0, &buf_rx_ppp[ind + 3]);
    } else {
      if (*(unsigned int *)&buf_rx_ppp[ind + 1] != L_RD_CONF_TIME)
        goto bad_com;
    }

    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] =
        L_WR_CONF_TIME + 1;

    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x00; // OK
    burst_ds_r();
    buf_tx_232[TR_OP_DATA + C1_DATA + 4 + cnt_bt] = real_time.r_sec;
    buf_tx_232[TR_OP_DATA + C1_DATA + 5 + cnt_bt] = real_time.r_min;
    buf_tx_232[TR_OP_DATA + C1_DATA + 6 + cnt_bt] = real_time.r_hor;
    buf_tx_232[TR_OP_DATA + C1_DATA + 7 + cnt_bt] = real_time.r_date;
    buf_tx_232[TR_OP_DATA + C1_DATA + 8 + cnt_bt] = real_time.r_month;
    buf_tx_232[TR_OP_DATA + C1_DATA + 9 + cnt_bt] = real_time.r_year;
    cnt_bt = cnt_bt + L_WR_CONF_TIME + 1 + 3;

    break;

  default:
  bad_com:
    buf_tx_232[TR_OP_DATA + C1_DATA + 3 + cnt_bt] = 0x02; // не поддерживается
  nocorr_com:
    buf_tx_232[TR_OP_DATA + C1_DATA + cnt_bt] = buf_rx_ppp[ind];
    *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1 + cnt_bt] = 1;
    cnt_bt = cnt_bt + 4;
    break;
  }

  ind = ind + 3 + *(unsigned int *)&buf_rx_ppp[ind + 1];
  if (ind >= count_rx_ppp - 3)
    return (cnt_bt);
  goto next_parametr;
}

void send_no_sinc(unsigned char id, unsigned int dst, unsigned char error) {
  if (fl_ip.act_ip_end != 1)
    return;
  fl_ip.act_ip_end = 0;
  clr_cntr_nat();
  clr_cntr_link();
  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER1;     // версия
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
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
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
  Obj_ppp_tx.version = VER1;     // версия
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
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

unsigned char proc_udp_data(unsigned char *buf_rx_ppp,
                            unsigned int count_rx_ppp) {
  unsigned int length;
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

  // проверка crc
  if (*(unsigned int *)&buf_rx_ppp[TR_CRC] !=
      crc_m1(&buf_rx_ppp[TR_LEN], *(unsigned int *)&buf_rx_ppp[TR_LEN] - 2,
             0xffff))
    return (0);

  // Crc_out.bytes[1]=buf_rx_ppp[30];    //раскодировка
  // Crc_out.bytes[0]=buf_rx_ppp[31];
  // temp.bytes[1]=buf_rx_ppp[26];
  // temp.bytes[0]=buf_rx_ppp[27];
  // temp.word=temp.word-8;
  // transform_buf(&buf_rx_ppp[32],temp.word,Crc_out.word);

  if ((buf_rx_ppp[TR_V] >> 6) == VER1) // версия протокола VER1
  {

    if (*(unsigned int *)&buf_rx_ppp[TR_DST] != num_self)
      return (0);

    length_head = (buf_rx_ppp[TR_V] & 0x0f) * 4;

    if (length_head < MIN_TR_HEAD)
      return (0);
    if (length_head > 50)
      return (0); // пока ограничил

    /*
     p_data=&buf_rx_ppp[TR_OP_DATA];
     length_head=MIN_TR_HEAD;
     if((buf_rx_ppp[TR_V]&0x0f)*4>MIN_TR_HEAD)

        {
        //здесь обработка возможных опций транспортного заголовка
          // пока обработка одной опции - отсутствие указанного адресата в
     информационном поле


            if((buf_rx_ppp[TR_V]&0x0f)*4!=MIN_TR_HEAD+8)return(0);
            if(((buf_rx_ppp[TR_V]>>4)&0x03)!=KVIT)return(0);
            if(buf_rx_ppp[TR_OP_DATA]!=KOD_OP_ERROR)return(0);
            if(buf_rx_ppp[TR_OP_DATA+1]!=L_ERR_NO_INTABL)return(0);
            if(buf_rx_ppp[TR_OP_DATA+2]!=ERR_NO_INTABL)return(0);

            if(*(unsigned int*)&buf_rx_ppp[TR_OP_DATA+3]==num_seq_cl)
                {

                  Appl_seq.cnt_cntr_cl=0;
                  Appl_seq.cntr_cl=TRUE;
                  Appl_seq.en_cntr_cl=TRUE;
                  Appl_seq.cnt_try=0;
                  fl_appl_seq.en_povtor=0;
                  Appl_seq.l_data=0;
                  Appl_seq.cnt_waits=0;
                  fl_appl_seq.kv_waits=0;
                  }

                  Control.link_waits=FALSE;
                  Control.cnt_link=Control.vol_link;
                  Control.link=FALSE;

                  Control.cnt_try=0;
                  Control.link_no=FALSE;



            length_head=MIN_TR_HEAD+4;
            return(1);
        // пока заглушено

        }

     */

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

      if (*(unsigned int *)&buf_rx_ppp[TR_SRC] != NUM_SRV_LINK)
        return (0);
      //                  if(buf_rx_ppp[TR_ID]!=ID_CC)return(0);

      if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != KVIT)
        return (0);

      if (Control.link_waits == TRUE) {
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

      Control.link_waits = FALSE;
      Control.cnt_link = Control.vol_link;
      Control.link = FALSE;
      Control.cnt_try = 0;
      Control.link_no = FALSE;

      return (1);
    }

    if (buf_rx_ppp[offset + C1_PROT] != PROT_C1)
      return (0);

    switch (buf_rx_ppp[offset + C1_PORT]) {
    case PORT485_1:

      if (length < 11) {
        // можно отправиь сообщение об ошибке "некорректные данные"
        send_err485(NUM_RS485_1, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (length > LN_BUF_485_1 + 10) {
        // можно отправиь сообщение об ошибке "слишком длинные данные для
        // приложения"
        send_err485(NUM_RS485_1, RS_OVER_BUF_TX, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      // здесь должна быть проверка контекста, если все нормально то дальше
      // забит контекст то можно послать служебное сообщение

      if (check_cont_485_1(&buf_rx_ppp[offset + C1_CONT]) == 1) {
        send_err485(NUM_RS485_1, RS_NO_CONT, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (Appl_RS485_1.fl_data_buf == TRUE) {
        // то можно послать служебное сообщение что буфер переполнен
        send_err485(NUM_RS485_1, RS_BUSY, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        break;
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
        send_err485(NUM_RS485_2, RS_DATA_ERR, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (length > LN_BUF_485_2 + 10) {
        // можно отправиь сообщение об ошибке "слишком длинные данные для
        // приложения"
        send_err485(NUM_RS485_2, RS_OVER_BUF_TX, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      // здесь должна быть проверка контекста, если все нормально то дальше
      // забит контекст то можно послать служебное сообщение
      if (check_cont_485_2(&buf_rx_ppp[offset + C1_CONT]) == 1) {
        send_err485(NUM_RS485_2, RS_NO_CONT, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        return (0);
      }

      if (Appl_RS485_2.fl_data_buf == TRUE) {
        // то можно послать служебное сообщение что буфер переполнен
        send_err485(NUM_RS485_2, RS_BUSY, buf_rx_ppp[TR_ID],
                    *(unsigned int *)&buf_rx_ppp[TR_SRC]);
        break;
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
    case PORT_TM:
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
        Obj_ppp_tx.version = VER1;     // версия
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
        buf_tx_232[TR_OP_DATA + C1_DATA + 8] = 0;
        buf_tx_232[TR_OP_DATA + C1_DATA + 9] = 0;

        if (Appl_seq_des == 0) {
          buf_tx_232[TR_OP_DATA + C1_DATA + 7] = state_seq;
          buf_tx_232[TR_OP_DATA + C1_DATA + 8] = sv_tc.bytes[0] & 0x01;
          buf_tx_232[TR_OP_DATA + C1_DATA + 9] = byte_state_net & 0x01;
        } else
          buf_tx_232[TR_OP_DATA + C1_DATA + 7] = DES_SEQ;

        Obj_ppp_tx.l_data = 5 + L_STATE; // длина данных
        Obj_ppp_tx.p_data =
            &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

        form_buf_tx_ppp();
        UCSR0A = UCSR0A | TXC;
        UCSR0B = UCSR0B | TXEN;
        UCSR0B = UCSR0B | TXCIE;
        if (check_cts() == 1)
          return (1);
        SVD2_1_ON;
        UDR0 = buf_tx_232[0];
        return (1);
      }

      if (fl_appl_seq.kv_waits == 1) {
        if (((buf_rx_ppp[TR_V] >> 4) & 0x03) != KVIT)
          return (0);
        if (Appl_seq.id != buf_rx_ppp[TR_ID])
          return (0); // проверка идентификатора

        Appl_seq.cnt_waits = 0;
        fl_appl_seq.kv_waits = 0;

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
      Obj_ppp_tx.version = VER1;     // версия
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
      SVD2_1_ON;
      UDR0 = buf_tx_232[0];

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
        Obj_ppp_tx.version = VER1;     // версия
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

        form_buf_tx_ppp();
        UCSR0A = UCSR0A | TXC;
        UCSR0B = UCSR0B | TXEN;
        UCSR0B = UCSR0B | TXCIE;

        if (reboot_byte != 0x55) {
          if (check_cts() == 1)
            return (0);
        }

        SVD2_1_ON;
        UDR0 = buf_tx_232[0];
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
        WDTCR = 0x18;
        WDTCR = 0x0f;
        __watchdog_reset();
        contr_gluk1 = C_GLUK1;
        contr_gluk2 = C_GLUK2;
        reload_apl();
      }

      return (1);
    }

  } // для версии VER1

  return (0);
}

unsigned char read_eeprom(unsigned int adresok) {
  union {
    unsigned char b_adres_ee[2];
    unsigned int adres_ee;
  } st_eeprom;

  st_eeprom.adres_ee = adresok;

  while (EECR & 2)
    ;

  EEARH = st_eeprom.b_adres_ee[1];
  EEARL = st_eeprom.b_adres_ee[0];

  EECR = EECR | 1;
  return (EEDR);
}

unsigned int pppfcs16(unsigned int fcs, unsigned char *cp, unsigned int len)

{

  while (len--)
    fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
  return (fcs);
}

void s_port(unsigned char ch) {
wawa:
  if ((UCSR0A & 0x20) == 0)
    goto wawa;
  UDR0 = ch;
}

void mov_s(char size, char __flash *p) {
  while (size--)
    s_port(*p++);
}

void mov_buf(char size, unsigned char *p) {
  while (size--)
    s_port(*p++);
}

void mov_lf(void) {
  s_port(0xd);
  s_port(0xa);
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

/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void test_alert(void) {

  // 1 scan
  if (((ru.scanb[0] & 1) == 1) & (g2[0] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~1;
    // sstii.arr_tii[0]++;
    real_tc.bit.b0 = tek_tc.bit.b0;
  }

  // 2 scan
  if (((ru.scanb[0] & 2) == 2) & (g2[1] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~2;
    // sstii.arr_tii[1]++;
    real_tc.bit.b1 = tek_tc.bit.b1;
  }

  // 3 scan
  if (((ru.scanb[0] & 4) == 4) & (g2[2] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~4;
    // sstii.arr_tii[2]++;
    real_tc.bit.b2 = tek_tc.bit.b2;
  }

  // 4 scan
  if (((ru.scanb[0] & 8) == 8) & (g2[3] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~8;
    // sstii.arr_tii[3]++;
    real_tc.bit.b3 = tek_tc.bit.b3;
  }

  // 5 scan
  if (((ru.scanb[0] & 0x10) == 0x10) & (g2[4] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~0x10;
    // sstii.arr_tii[4]++;
    real_tc.bit.b4 = tek_tc.bit.b4;
  }

  // 6 scan
  if (((ru.scanb[0] & 0x20) == 0x20) & (g2[5] > PREDEL_OUT_TC)) {

    ru.scanb[0] = ru.scanb[0] & ~0x20;
    // sstii.arr_tii[5]++;
    real_tc.bit.b5 = tek_tc.bit.b5;
  }

  // 7 scan
  if (((ru.scanb[0] & 0x40) == 0x40) & (g2[6] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~0x40;
    // sstii.arr_tii[6]++;
    real_tc.bit.b6 = tek_tc.bit.b6;
  }

  // 8 scan
  if (((ru.scanb[0] & 0x80) == 0x80) & (g2[7] > PREDEL_OUT_TC)) {
    ru.scanb[0] = ru.scanb[0] & ~0x80;
    //   sstii.arr_tii[7]++;
    //        datchik_ob++;
    real_tc.bit.b7 = tek_tc.bit.b7;
  }

  //////////////////////////

  // 1 scan
  if (((ru.scanb[1] & 1) == 1) & (g2[0 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~1;
    // sstii.arr_tii[0+8]++;
    real_tc.bit.b8 = tek_tc.bit.b8;
  }

  // 2 scan
  if (((ru.scanb[1] & 2) == 2) & (g2[1 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~2;
    //  sstii.arr_tii[1+8]++;
    real_tc.bit.b9 = tek_tc.bit.b9;
  }

  // 3 scan
  if (((ru.scanb[1] & 4) == 4) & (g2[2 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~4;
    //  sstii.arr_tii[2+8]++;
    real_tc.bit.b10 = tek_tc.bit.b10;
  }

  // 4 scan
  if (((ru.scanb[1] & 8) == 8) & (g2[3 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~8;
    //  sstii.arr_tii[3+8]++;
    real_tc.bit.b11 = tek_tc.bit.b11;
  }

  // 5 scan
  if (((ru.scanb[1] & 0x10) == 0x10) & (g2[4 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~0x10;
    //  sstii.arr_tii[4+8]++;
    real_tc.bit.b12 = tek_tc.bit.b12;
  }

  // 6 scan
  if (((ru.scanb[1] & 0x20) == 0x20) & (g2[5 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~0x20;
    //  sstii.arr_tii[5+8]++;
    real_tc.bit.b13 = tek_tc.bit.b13;
  }

  // 7 scan
  if (((ru.scanb[1] & 0x40) == 0x40) & (g2[6 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~0x40;
    //  sstii.arr_tii[6+8]++;

    real_tc.bit.b14 = tek_tc.bit.b14;
  }

  // 8 scan
  if (((ru.scanb[1] & 0x80) == 0x80) & (g2[7 + 8] > PREDEL_OUT_TC)) {
    ru.scanb[1] = ru.scanb[1] & ~0x80;
    //   sstii.arr_tii[7+8]++;
    real_tc.bit.b15 = tek_tc.bit.b15;
  }

  //////////////////
}

#pragma vector = TIMER0_COMP_vect
__interrupt void TIMER0_COMP_interrupt(void)

{

  char c, cccc;

  fl_wdt.from_timer0 = 1;

  __enable_interrupt();

  // опрос реальных ТС
  // опрос реальных ТС
  ////////////////////////////////////
  test_alert();
  g2[0]++;
  g2[1]++;
  g2[2]++;
  g2[3]++;
  g2[4]++;
  g2[5]++;
  g2[6]++;
  g2[7]++;
  g2[8]++;
  g2[9]++;
  g2[10]++;
  g2[11]++;
  g2[12]++;
  g2[13]++;
  g2[14]++;
  g2[15]++;
  //     __disable_interrupt();
  cccc = 0;

  if (PINC & 8)
    cccc |= 1;
  if (PING & 2)
    cccc |= 2;
  if (PINC & 2)
    cccc |= 4;
  if (PINC & 4)
    cccc |= 8;
  c = tek_tc.arr_tc[count] ^ cccc;
  tek_tc.arr_tc[count] = cccc;
  c = c & 0xf;

  switch (count) {
    //
  case 0: {

    if (c != 0)

    {

      if (c & 1) {
        ru.scanb[0] |= 1;
        g2[0] = 0;
      }

      if (c & 2) {

        ru.scanb[0] |= 2;
        g2[1] = 0;
      }

      if (c & 4) {

        ru.scanb[0] |= 4;
        g2[2] = 0;
      }

      if (c & 8) {

        ru.scanb[0] |= 8;
        g2[3] = 0;
      }
    }
    break;
  }

    //

    //
  case 1: {

    if (c != 0)

    {

      if (c & 1) {

        ru.scanb[0] |= 0x10;
        g2[4] = 0;
      }

      if (c & 2) {

        ru.scanb[0] |= 0x20;
        g2[5] = 0;
      }

      if (c & 4) {

        ru.scanb[0] |= 0x40;
        g2[6] = 0;
      }

      if (c & 8) {

        ru.scanb[0] |= 0x80;
        g2[7] = 0;
      }
    }
    break;
  }

    //
    //    2 byte
  case 2: {

    if (c != 0)

    {

      if (c & 1) {

        ru.scanb[1] |= 0x1;
        g2[8] = 0;
      }

      if (c & 2) {

        ru.scanb[1] |= 0x2;
        g2[9] = 0;
      }

      if (c & 4) {

        ru.scanb[1] |= 0x4;
        g2[10] = 0;
      }

      if (c & 8) {

        ru.scanb[1] |= 0x8;
        g2[11] = 0;
      }
    }
    break;
  }

    //

  case 3: {

    if (c != 0)

    {

      if (c & 1) {

        ru.scanb[1] |= 0x10;
        g2[12] = 0;
      }

      if (c & 2) {

        ru.scanb[1] |= 0x20;
        g2[13] = 0;
      }

      if (c & 4) {

        ru.scanb[1] |= 0x40;
        g2[14] = 0;
      }

      if (c & 8) {

        ru.scanb[1] |= 0x80;
        g2[15] = 0;
      }
    }
    break;
  }

    //

  } // end switch

  count++;
  if (count == 4) {
    count = 0;
    //              return;
  }

  switch (count)

  {
  case 0: {
    PORTD = PORTD & ~0x80;
    PORTG = PORTG | 1;
    PORTC = PORTC | 1;
    PORTC = PORTC | 0x10;
    break;
  }
  case 1: {
    PORTD = PORTD | 0x80;
    PORTG = PORTG & ~1;
    PORTC = PORTC | 1;
    PORTC = PORTC | 0x10;
    break;
  }
  case 2: {
    PORTD = PORTD | 0x80;
    PORTG = PORTG | 1;
    PORTC = PORTC & ~1;
    PORTC = PORTC | 0x10;
    break;
  }
  case 3: {
    PORTD = PORTD | 0x80;
    PORTG = PORTG | 1;
    PORTC = PORTC | 1;
    PORTC = PORTC & ~0x10;
    break;
  }

  default:
    break;
  }

  ////////////////////////////////////

  // конец
  // опрос реальных ТС
  // опрос реальных ТС
}

/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// 888888888888888888888888888888888888888888888888888
// 888888888888888888888888888888888888888888888888888
/////эмуляторы портов

void reset_em_232_new(void) {
  fl_232_2.over = 0;
  Emul_232.cnt_bit = 0;
  Rs232_2.cnt_bt_rx_tx = 0;
  Emul_232.dir_tx = FALSE;
  TIFR = TIFR | 0x10; //??
  TIMSK = TIMSK & ~0x10;
  EIFR = EIFR | 0x20;
  EIMSK = EIMSK | 0x20; // enable start find
  Rs232_2.p_data232 = &Rs232_2_buf_rx_tx[0];
}

void send_em_232_2(void) {

  Emul_232.shift_reg = 0xff00 | (*Rs232_2.p_data232);
  Emul_232.shift_reg = Emul_232.shift_reg << 1;
  Emul_232.cnt_bit = 0;
  Emul_232.dir_tx = TRUE;
  OCR1A = TCNT1 + Emul_232.bod;
  EIMSK = EIMSK & ~0x20;
  TIMSK = TIMSK | 0x10;
}

void reset_em_485_err(void) {

  clr_rts_485_2;
  TIFR = TIFR | 0x08; //??
  TIMSK = TIMSK & ~0x08;
  EIFR = EIFR | 0x02;
  EIMSK = EIMSK & ~0x02;
}

void send_em_485(void) {
  Emul_485.dir_tx = TRUE;
  Emul_485.cnt_bit = 0;
  Emul_485.bit_par = 0;
  Emul_485.shift_reg = 0xff00 | (*Rs485_2.p_data485);
  Emul_485.shift_reg = Emul_485.shift_reg << 1;
  EIMSK = EIMSK & ~0x02;
  set_rts_485_2;
}

void reset_em_485_new(void) {
  Emul_485.cnt_bit = 0;
  Emul_485.bit_par = 0;
  Rs485_2.cnt_bt_rx_tx = 0;
  Rs485_2.p_data485 = &Rs485_2_buf_rx_tx[0];
  TIFR = TIFR | 0x08; //??
  TIMSK = TIMSK & ~0x08;
  EIFR = EIFR | 0x02;
  EIMSK = EIMSK | 0x02; // enable start find
}

void set_em_485_rx(void) {
  Emul_485.cnt_bit = 0;
  Emul_485.bit_par = 0;
  Rs485_2.cnt_bt_rx_tx = 0;
  Rs485_2.p_data485 = &Rs485_2_buf_rx_tx[0];
  Rs485_2.cnt_tm_out = Rs485_2.vol_tm_out;
  clr_rts_485_2;
  // PORTD=PORTD & ~1; // togle to receiv mode
  TIFR = TIFR | 0x08; //??
  TIMSK = TIMSK & ~0x08;
  EIFR = EIFR | 0x02;
  EIMSK = EIMSK | 0x02; // enable start find
  Emul_485.dir_tx = FALSE;
}

#pragma vector = INT1_vect // по эмулированному 485
__interrupt void INT1_interrupt(void)

//    start bit
{
  TIFR = TIFR | 0x08;
  OCR1B = TCNT1 + Emul_485.bod / 3;
  EIMSK = EIMSK & ~0x02; // disable myself
  TIMSK = TIMSK | 0x08;
}

#pragma vector = TIMER1_COMPB_vect
__interrupt void TIMER1_COMPB_interrupt(void)

{
  OCR1B = TCNT1 + Emul_485.bod;

  if (Emul_485.dir_tx == TRUE) {

    Emul_485.cnt_bit++;

    if (Emul_485.cnt_bit == 10) // вставка паритета если нужно
    {
      switch (Emul_485.fl_bit_chet) {
      case 2:
        if (Emul_485.bit_par) {
          SETB485_2;
        } else {
          CLR485_2;
        } // int-registr
        return;
      case 1:
        if (Emul_485.bit_par) {
          CLR485_2;
        } else {
          SETB485_2;
        } // int-registr
        return;
      }
    }

    //  if((Emul_485.shift_reg & 1)==1) SETB485_2;  // int-registr
    //  if((Emul_485.shift_reg & 1)==0) CLR485_2;

    if (Emul_485.shift_reg & 1) {
      SETB485_2;
    } else {
      CLR485_2;
    } // int-registr
    Emul_485.shift_reg = Emul_485.shift_reg >> 1;

    Emul_485.bit_par =
        Emul_485.bit_par ^ (Emul_485.shift_reg & 0x01); // подсчет паритета

    if (Emul_485.cnt_bit >= 10) {
      Emul_485.cnt_bit = 0;
      Emul_485.bit_par = 0;
      if (Rs485_2.cnt_bt_rx_tx != 0) {

        Rs485_2.cnt_bt_rx_tx--;
        Rs485_2.p_data485++;
        Emul_485.shift_reg = 0xff00 | (*Rs485_2.p_data485);
        Emul_485.shift_reg = Emul_485.shift_reg << 1;
      }
      if (Rs485_2.cnt_bt_rx_tx == 0)

      {
        TIMSK = TIMSK & ~0x08;
        if (Rs485_2.vol_tm_tx_out == 0)
          set_em_485_rx();
        else
          Rs485_2.cnt_tm_tx_out = Rs485_2.vol_tm_tx_out;
      }
    }

  }

  ///

  else {
    Emul_485.cnt_bit++;

    if (Emul_485.cnt_bit == 10) {

      switch (Emul_485.fl_bit_chet) {
      case 2:
      case 1:
        return;
      }
    }

    // if ( ((Emul_485.cnt_bit==1)&&((PIND & IN_BIT485_2)==IN_BIT485_2))||
    //      ((Emul_485.cnt_bit==10)&&((PIND & IN_BIT485_2)==0)) )

    //  if  ((Emul_485.cnt_bit==1)&&((PIND & IN_BIT485_2)==IN_BIT485_2))

    if (((Emul_485.cnt_bit == 1) && ((PIND & IN_BIT485_2) == IN_BIT485_2)) ||
        ((Emul_485.cnt_bit >= 10) && ((PIND & IN_BIT485_2) == 0)))

    {
      reset_em_485_new();
      return;
    }

    if ((PIND & IN_BIT485_2) == IN_BIT485_2)
      Emul_485.shift_reg = Emul_485.shift_reg | 0x8000;
    if ((PIND & IN_BIT485_2) == 0)
      Emul_485.shift_reg = Emul_485.shift_reg & ~0x8000;

    if (Emul_485.cnt_bit >= 10) {

      Emul_485.cnt_bit = 0;
      // Rs485_2.cnt_bt_rx_tx++;
      Emul_485.shift_reg = Emul_485.shift_reg << 1;
      Rs485_2.cnt_tm_out = 0;
      Rs485_2.cnt_tm_rx_out = Rs485_2.vol_tm_rx_out;
      TIMSK = TIMSK & ~0x08;
      EIFR = EIFR | 0x02;
      EIMSK = EIMSK | 0x02;
      /*
      if(Rs485_2.cnt_tm_rx_out<=MAX_BUF_RS485_2) *Rs485_2.p_data485++=(unsigned
      char)(Emul_485.shift_reg>>8);//переполнение else {
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Rs485_2.cnt_bt_rx_tx--;
            }
      */

      if (Rs485_2.cnt_bt_rx_tx < MAX_BUF_RS485_2) {
        Rs485_2_buf_rx_tx[Rs485_2.cnt_bt_rx_tx] =
            (unsigned char)(Emul_485.shift_reg >> 8);
        Rs485_2.cnt_bt_rx_tx++;
      } else
        fl_485_2.over = 1;
    }

    Emul_485.shift_reg = Emul_485.shift_reg >> 1;
  }
}

#pragma vector = INT5_vect // по эмулированному 232
__interrupt void INT5_interrupt(void)

//    start bit
{
  TIFR = TIFR | 0x10;
  OCR1A = TCNT1 + Emul_232.bod / 3;
  EIMSK = EIMSK & ~0x20; // disable myself
  TIMSK = TIMSK | 0x10;
}

#pragma vector = TIMER1_COMPA_vect
__interrupt void TIMER1_COMPA_interrupt(void)

{
  OCR1A = TCNT1 + Emul_232.bod;

  if (Emul_232.dir_tx == TRUE) {

    Emul_232.cnt_bit++;
    if ((Emul_232.shift_reg & 1) == 1)
      SETB232_2; // int-registr
    if ((Emul_232.shift_reg & 1) == 0)
      CLR232_2;
    Emul_232.shift_reg = Emul_232.shift_reg >> 1;

    if (Emul_232.cnt_bit == 10) {
      Emul_232.cnt_bit = 0;
      if (Rs232_2.cnt_bt_rx_tx != 0) {
        Rs232_2.cnt_bt_rx_tx--;
        Rs232_2.p_data232++;
        Emul_232.shift_reg = 0xff00 | (*Rs232_2.p_data232);
        Emul_232.shift_reg = Emul_232.shift_reg << 1;
      }
      if (Rs232_2.cnt_bt_rx_tx == 0)

      {
        fl_232_2_add.was_tx = 1; // флаг того что была передача
        fl_232_2_add.sinc = 0;   // очистка того что принимались биты
        Emul_232_cnt_post_tx =
            DEF_EM232_POST_TX; // установка защитного интервала на прием
        fl_232_2.busy = 0;
        reset_em_232_new(); // переключение на прием с поиском нового синхро
      }
    }

  }

  ///

  else {
    Emul_232.cnt_bit++;

    fl_232_2_add.sinc = 1;

    if (((Emul_232.cnt_bit == 1) && ((PINE & IN_BIT232_2) == IN_BIT232_2)) ||
        ((Emul_232.cnt_bit == 10) && ((PINE & IN_BIT232_2) == 0)))

    {
      reset_em_232_new(); // разрешить поиск синхро
      return;
    }

    if ((PINE & IN_BIT232_2) == IN_BIT232_2)
      Emul_232.shift_reg = Emul_232.shift_reg | 0x8000;
    if ((PINE & IN_BIT232_2) == 0)
      Emul_232.shift_reg = Emul_232.shift_reg & ~0x8000;

    if (Emul_232.cnt_bit >= 10) {

      Emul_232.cnt_bit = 0;
      Emul_232.shift_reg = Emul_232.shift_reg << 1;
      Rs232_2.cnt_tm_rx_out = Rs232_2.vol_tm_rx_out;
      TIMSK = TIMSK & ~0x10;
      EIFR = EIFR | 0x20;
      EIMSK = EIMSK | 0x20;

      if (Rs232_2.cnt_bt_rx_tx < MAX_BUF_RS232_2) {
        Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx] =
            (unsigned char)(Emul_232.shift_reg >> 8);
        Rs232_2.cnt_bt_rx_tx++;
      } else
        fl_232_2.over = 1;
    }

    Emul_232.shift_reg = Emul_232.shift_reg >> 1;
  }
}

// 888888888888888888888888888888888888888888888888888
// 888888888888888888888888888888888888888888888888888
/////эмуляторы портов

#pragma vector = TIMER3_COMPA_vect // 1 сек
__interrupt void TIMER3_COMPA_interrupt(void)
//////////////////////////////////////////
{

  fl_wdt.from_timer3 = 1;

  if ((PINB & CTS_232) == CTS_232) {
    cnt_term++;
    if (cnt_term >= 4)
      lock_it();
  } else
    cnt_term = 0;

  if (count_block_net != 0)
    count_block_net--;

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

  if (fl_rewrite.cnt_reset == 0)
    goto next_tm01;
  fl_rewrite.cnt_reset--;
  if (fl_rewrite.cnt_reset == 0) {
    lock_it();
  }
next_tm01:

  glutch++;
  if (glutch >= 4)
    glutch = 0;

  /*
   if(Cnt_modem_on_off==0)goto next_tm00;
   Cnt_modem_on_off--;
   if(Cnt_modem_on_off==0) flag=1;
next_tm00:
 */

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
      Control.cnt_nat = Control.vol_nat;
      Control.nat = TRUE;
    }
  }

  //////////////////////////////////////////
}

#pragma vector = TIMER2_COMP_vect // 1,024 мсек
__interrupt void TIMER2_COMP_interrupt(void) {

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

  if (fl_cts_232.on == 1) {
    if (fl_cts_232_ignor == TRUE) {
      fl_cts_232.on = 0;
      if (count_tx_ppp == 0) {
        SVD2_1_ON;
        UDR0 = buf_tx_232[0];
      } else
        sending_ppp_pac();
      return;
    }

    // if (PINB & CTS_232)
    if ((PINA & DSR) == 0) {
      cnt_cts_off = 0;
      fl_cts_232.on = 0;
      if (count_tx_ppp == 0) {
        SVD2_1_ON;
        UDR0 = buf_tx_232[0];
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
    SVD2_2_OFF;
  } // счетчик межбайтовый промежуток
next_step1:

  if (Rs485_1.cnt_tm_pre_tx == 0)
    goto next_step2;
  Rs485_1.cnt_tm_pre_tx--;
  if (Rs485_1.cnt_tm_pre_tx == 0)
    UCSR1B = UCSR1B | UDRIE; // enable transmit 485_1

next_step2:

  if (Rs485_1.cnt_tm_out == 0)
    goto next_step3;
  Rs485_1.cnt_tm_out--;
  if (Rs485_1.cnt_tm_out == 0) {
    UCSR1B = UCSR1B & ~(RXEN | RXCIE);
    Rs485_1.cnt_bt_rx_tx = 0;
    Rs485_1.cnt_tm_rx_out = 0;
    fl_485_1.rec = 0;
    fl_485_1.tm_out = 1;
  }

next_step3:

  if (Rs485_1.cnt_tm_rx_out == 0)
    goto next_step4;
  Rs485_1.cnt_tm_rx_out--;
  if (Rs485_1.cnt_tm_rx_out == 0) {
    fl_485_1.rec = 1;
    Rs485_1.cnt_tm_out = 0;
    fl_485_1.tm_out = 0;
    UCSR1B = UCSR1B & ~(RXEN | RXCIE);
  }

next_step4:
  if (Rs485_1.cnt_tm_tx_out == 0)
    goto next_step5;
  Rs485_1.cnt_tm_tx_out--;
  if (Rs485_1.cnt_tm_tx_out == 1)
    clr_rts_485_1; // togle to receiv mode
  if (Rs485_1.cnt_tm_tx_out == 0) {
    Rs485_1.cnt_tm_out = Rs485_1.vol_tm_out;
    UCSR1B = UCSR1B | RXEN | RXCIE;
  } // togle to receiv mode ;

next_step5:

  if (Rs485_2.cnt_tm_pre_tx == 0)
    goto next_step6;
  Rs485_2.cnt_tm_pre_tx--;
  if (Rs485_2.cnt_tm_pre_tx == 0) {
    OCR1B = TCNT1 + Emul_485.bod;
    TIMSK = TIMSK | 0x08; // enable transmit 485
  }
next_step6:

  if (Rs485_2.cnt_tm_out == 0)
    goto next_step7;
  Rs485_2.cnt_tm_out--;
  if (Rs485_2.cnt_tm_out == 0) {
    Emul_485.dir_tx = FALSE;
    Emul_485.cnt_bit = 0;
    Emul_485.bit_par = 0;
    Rs485_2.cnt_bt_rx_tx = 0;
    Rs485_2.cnt_tm_rx_out = 0;
    fl_485_2.rec = 0;
    fl_485_2.tm_out = 1;
    reset_em_485_err(); // запрет приема
  }
next_step7:

  if (Rs485_2.cnt_tm_rx_out == 0)
    goto next_step8;
  Rs485_2.cnt_tm_rx_out--;
  if (Rs485_2.cnt_tm_rx_out == 0) {

    fl_485_2.rec = 1;
    Rs485_2.cnt_tm_out = 0;
    fl_485_2.tm_out = 0;
    Emul_485.dir_tx = FALSE;
    Emul_485.cnt_bit = 0;
    Emul_485.bit_par = 0;
    reset_em_485_err();
  }

next_step8:

  if (Rs485_2.cnt_tm_tx_out == 0)
    goto next_step9;
  Rs485_2.cnt_tm_tx_out--;
  if (Rs485_2.cnt_tm_tx_out == 0)
    set_em_485_rx(); // установка на прием

next_step9:

  if (Rs232_2.cnt_tm_rx_out == 0)
    goto next_step10;
  Rs232_2.cnt_tm_rx_out--;
  if (Rs232_2.cnt_tm_rx_out == 0) {

    if (((fl_232_2_add.sinc == 1) && (fl_232_2_add.was_tx == 1)) ||
        (Emul_232_cnt_post_tx != 0)) {
      fl_232_2_add.was_tx = 0;
      Emul_232_cnt_post_tx = 0;
      reset_em_232_new();
      goto next_step11;
    }

    fl_232_2.rec = 1;
    TIMSK = TIMSK & ~0x10;
    EIMSK = EIMSK & ~0x20;
    goto next_step11;
  }
next_step10:

  if (Emul_232_cnt_post_tx == 0)
    goto next_step11;
  Emul_232_cnt_post_tx--;
  if (Emul_232_cnt_post_tx == 0)
    fl_232_2_add.was_tx = 0;
  ;
next_step11:

  /// таймер рестарта LCP
  if (fl_lcp.lcp_tm_out_en == 1) {
    cnt_lcp_tm_out--;
    if (cnt_lcp_tm_out == 0) {
      cnt_lcp_tm_out = VOL_LCP_TM_OUT;
      if (cnt_rst_lcp == 0) {
        cnt_lcp_tm_out = vol_cnt_rst_lcp;
        fl_lcp.t0_mi = 1;
      } else {
        cnt_rst_lcp--;
        fl_lcp.t0_pl = 1;
      }
    }
  }
  //////////////////////////////////////////

  /// таймер рестарта PAP
  if (fl_pap.pap_tm_out_en == 1) {
    cnt_pap_tm_out--;
    if (cnt_pap_tm_out == 0) {
      cnt_pap_tm_out = VOL_PAP_TM_OUT;
      if (cnt_rst_pap == 0) {
        cnt_pap_tm_out = vol_cnt_rst_pap;
        fl_pap.t0_mi = 1;
      } else {
        cnt_rst_pap--;
        fl_pap.t0_pl = 1;
      }
    }
  }
  //////////////////////////////////////////

  /// таймер рестарта IPCP
  if (fl_ipcp.ipcp_tm_out_en == 1) {
    cnt_ipcp_tm_out--;
    if (cnt_ipcp_tm_out == 0) {
      cnt_ipcp_tm_out = VOL_IPCP_TM_OUT;
      if (cnt_rst_ipcp == 0) {
        cnt_ipcp_tm_out = vol_cnt_rst_ipcp;
        fl_ipcp.t0_mi = 1;
      } else {
        cnt_rst_ipcp--;
        fl_ipcp.t0_pl = 1;
      }
    }
  }
}

unsigned char recive_buf1(unsigned char temp) {

  if (fl_rx_ppp.fl_7e == 1) {

    if (Buf1_rx_ppp.ln_data >=
        VOL_RX_PPP) // проверка на переполнение первого буфера
    {
      Buf1_rx_ppp.ln_data = 0;
      fl_rx_ppp.fl_7e = 0;
      fl_rx_ppp.fl_7d = 0;
      SVD2_2_OFF;
      return (0);
    }

    // //////////укладка
    if (temp == 0x7d) {
      fl_rx_ppp.fl_7d = 1;
      return (0);
    } else {
      if (fl_rx_ppp.fl_7d == 1) {
        if ((temp >= 0x20) && (temp < 0x40)) {
          Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = temp - 0x20;
          Buf1_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        if (temp == 0x5e) {
          Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = 0x7e;
          Buf1_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        if (temp == 0x5d) {
          Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = 0x7d;
          Buf1_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        Buf1_rx_ppp.ln_data = 0;
        fl_rx_ppp.fl_7e = 0;
        fl_rx_ppp.fl_7d = 0;
        SVD2_2_OFF;
        return (0);
      } else {
        Buf1_rx_ppp.data[Buf1_rx_ppp.ln_data] = temp;
        Buf1_rx_ppp.ln_data++;

        if (temp == 0x7e) // нет переполнения
        {
          if (Buf1_rx_ppp.ln_data <= 2) // если подряд два 0x7e
          {
            //  if(ppp_packet.rcvd==1)???????????????
            fl_rx_ppp.fl_7e = 1;
            Buf1_rx_ppp.data[0] = temp;
            Buf1_rx_ppp.ln_data = 1;
            return (0);
          }

          SVD2_2_OFF;
          fl_rx_ppp.fl_7e = 0;
          fl_rx_ppp.switcher = 1;
          Buf1_rx_ppp.rec = TRUE;
          Buf1_rx_ppp.busy = TRUE;
          if (Buf2_rx_ppp.busy == TRUE) {
            Buf2_rx_ppp.check_busy = TRUE;
            UCSR0B = UCSR0B & ~RXEN;
            UCSR0B = UCSR0B & ~RXCIE;
          }
          return (0);
        }
      }
    }

    return (0);
  } // к пекет бегин
  else {
    if (temp == 0x7e) {
      //  if(ppp_packet.rcvd==1)???????????????
      fl_rx_ppp.fl_7e = 1;
      Buf1_rx_ppp.data[0] = temp;
      Buf1_rx_ppp.ln_data = 1;
      SVD2_2_ON;
      return (0);
    }
    return (1);
  }
}

unsigned char recive_buf2(unsigned char temp) {

  if (fl_rx_ppp.fl_7e == 1) {

    if (Buf2_rx_ppp.ln_data >=
        VOL_RX_PPP) // проверка на переполнение первого буфера
    {
      Buf2_rx_ppp.ln_data = 0;
      fl_rx_ppp.fl_7e = 0;
      fl_rx_ppp.fl_7d = 0;
      SVD2_2_OFF;
      return (0);
    }

    // //////////укладка
    if (temp == 0x7d) {
      fl_rx_ppp.fl_7d = 1;
      return (0);
    } else {
      if (fl_rx_ppp.fl_7d == 1) {

        if ((temp >= 0x20) && (temp < 0x40)) {
          Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = temp - 0x20;
          Buf2_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        if (temp == 0x5e) {
          Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = 0x7e;
          Buf2_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        if (temp == 0x5d) {
          Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = 0x7d;
          Buf2_rx_ppp.ln_data++;
          fl_rx_ppp.fl_7d = 0;
          return (0);
        }
        Buf2_rx_ppp.ln_data = 0;
        fl_rx_ppp.fl_7e = 0;
        fl_rx_ppp.fl_7d = 0;
        SVD2_2_OFF;
        return (0);
      } else {
        Buf2_rx_ppp.data[Buf2_rx_ppp.ln_data] = temp;
        Buf2_rx_ppp.ln_data++;

        if (temp == 0x7e) // нет переполнения
        {
          if (Buf2_rx_ppp.ln_data <= 2) // если подряд два 0x7e
          {
            //  if(ppp_packet.rcvd==1)???????????????
            fl_rx_ppp.fl_7e = 1;
            Buf2_rx_ppp.data[0] = temp;
            Buf2_rx_ppp.ln_data = 1;
            return (0);
          }

          SVD2_2_OFF;
          fl_rx_ppp.fl_7e = 0;
          fl_rx_ppp.switcher = 0;
          Buf2_rx_ppp.rec = TRUE;
          Buf2_rx_ppp.busy = TRUE;
          if (Buf1_rx_ppp.busy == TRUE) {
            Buf1_rx_ppp.check_busy = TRUE;
            UCSR0B = UCSR0B & ~RXEN;
            UCSR0B = UCSR0B & ~RXCIE;
          }
          return (0);
        }
      }
    }

    return (0);
  } // к пекет бегин
  else {
    if (temp == 0x7e) {
      //  if(ppp_packet.rcvd==1)???????????????
      fl_rx_ppp.fl_7e = 1;
      Buf2_rx_ppp.data[0] = temp;
      Buf2_rx_ppp.ln_data = 1;
      SVD2_2_ON;
      return (0);
    }
    return (1);
  }
}

#pragma vector = USART0_RXC_vect

__interrupt void UART_RX_interrupt(void) {
  unsigned char temp;
  temp = UDR0;

  if (fl_at_mom_232 == 1) {
    if (vol_tx_ppp >= VOL_TX_PPP)
      vol_tx_ppp = 0;
    buf_tx_232[vol_tx_ppp] = temp;
    vol_tx_ppp++;
    return;
  }

  if (command_AT == TRUE) {
    SVD2_2_ON;
    At_com.cnt_rx_out = At_com.vol_rx_out;
    At_com.cnt_tm_out = 0;
    if (At_com.cnt_rx < LN_BUF_AT) {
      At_com.buf[At_com.cnt_rx] = temp;
      At_com.cnt_rx++;
    }
    return;
  }

  if (fl_rx_ppp.switcher == 0) {
    if (recive_buf1(temp) == 1) {
      if (count_rx_232 >= kol_rx_232)
        return;
      buf_rx_232[count_rx_232] = temp;
      count_rx_232++;
      return;
    }
  } else {

    if (recive_buf2(temp) == 1) {
      if (count_rx_232 >= kol_rx_232)
        return;
      buf_rx_232[count_rx_232] = temp;
      count_rx_232++;
      return;
    }
  }
}

#pragma vector = USART1_RXC_vect

__interrupt void UART1_RX_interrupt(void)

{
  unsigned char data;

  data = UDR1;
  Rs485_1.cnt_tm_rx_out = Rs485_1.vol_tm_rx_out;
  Rs485_1.cnt_tm_out = 0;
  if (Rs485_1.cnt_bt_rx_tx < MAX_BUF_RS485_1) {
    Rs485_1_buf_rx_tx[Rs485_1.cnt_bt_rx_tx] = data;
    Rs485_1.cnt_bt_rx_tx++;
  } else
    fl_485_1.over = 1;
}

#pragma vector = USART1_UDRE_vect

__interrupt void UART1_UDRE_interrupt(void)

{

  if (Rs485_1.cnt_bt_rx_tx == 0)
    goto end_tx1;

  UDR1 = *Rs485_1.p_data485++;
  Rs485_1.cnt_bt_rx_tx--;
  return;
end_tx1:
  UCSR1B = UCSR1B & ~UDRIE;
  Rs485_1.cnt_bt_rx_tx = 0;
  Rs485_1.cnt_tm_tx_out = Rs485_1.vol_tm_tx_out;
  if (Rs485_1.cnt_tm_tx_out == 0) {
    Rs485_1.cnt_tm_out = Rs485_1.vol_tm_out;
    clr_rts_485_1;                  // togle to receiv mode
    UCSR1B = UCSR1B | RXEN | RXCIE; // togle to receiv mode ;
  }
}

void sending_ppp_pac(void) {
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
    UCSR0B = UCSR0B & ~TXEN;
    UCSR0B = UCSR0B & ~TXCIE;
    SVD2_1_OFF;
    return;
  }

  if (fl_reg3.add_byte == 1) {
    fl_reg3.add_byte = 0;
    if (buf_tx_232[count_tx_ppp] == 0x7e) {
      UDR0 = 0x5e;
      return;
    }
    if (buf_tx_232[count_tx_ppp] == 0x7d) {
      UDR0 = 0x5d;
      return;
    }
    UDR0 = buf_tx_232[count_tx_ppp] + 0x20;
    return;
  }

  count_tx_ppp++;

  if (count_tx_ppp == (vol_tx_ppp - 1))
    goto exit_tx;

  if ((buf_tx_232[count_tx_ppp] == 0x7e) ||
      (buf_tx_232[count_tx_ppp] == 0x7d)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    return;
  }

  if ((buf_tx_232[count_tx_ppp] == 17) || (buf_tx_232[count_tx_ppp] == 19)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    return;
  }

  if ((buf_tx_232[count_tx_ppp] < 0x20) && (fl_ip.act_ip_end == 1)) {
    fl_reg3.add_byte = 1;
    UDR0 = 0x7d;
    return;
  }

exit_tx:
  UDR0 = buf_tx_232[count_tx_ppp];
}

unsigned char check_cts(void) {
  // if ((PINB & CTS_232)==0)
  if (PINA & DSR) {
    fl_cts_232.on = 1;
    return (1);
  } // proverka CTS
  return (0);
}

#pragma vector = USART0_TXC_vect
__interrupt void USART0_TXC_interrupt(void)

{

  if (command_AT == TRUE)
    sending_at_pac();
  else {

    if (fl_cts_232_ignor == TRUE) {
      sending_ppp_pac();
      return;
    }

    if (check_cts() == 1)
      return;
    sending_ppp_pac();
  }
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
  case TLS:
    fl_lcp.act_lcp_end = 0;
    // write_log_info(ST_ERROR,ERR5);
    // lock_it();
    event_modem = EVM_PPP_ERR;
    break;
  case TLF:
    fl_lcp.act_lcp_end = 0;
    // write_log_info(ST_ERROR,ERR6);
    // lock_it();
    event_modem = EVM_PPP_ERR;
    break;
  case IRC:
    fl_lcp.act_lcp_end = 0;
    cnt_lcp_tm_out = VOL_LCP_TM_OUT;
    cnt_rst_lcp = vol_cnt_rst_lcp;
    fl_lcp.lcp_tm_out_en = 1;
    fl_lcp.act_lcp_end = 1;

    break;
  case ZRC:
    fl_lcp.act_lcp_end = 0;
    cnt_lcp_tm_out = VOL_LCP_TM_OUT;
    cnt_rst_lcp = 0;
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
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

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
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

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
      SVD2_1_ON;
      UDR0 = buf_tx_232[0];

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
      SVD2_1_ON;
      UDR0 = buf_tx_232[0];
    }

    break;
  case STR:
    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;
    buf_tx_232[5] = TERM_REQ;
    buf_tx_232[6] = i_str_lcp;

    buf_tx_232[7] = 0;
    buf_tx_232[8] = 0x4;

    vol_tx_ppp = buf_tx_232[8] + 8;

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
    buf_tx_232[9] = temp.bytes[0];
    buf_tx_232[10] = temp.bytes[1];

    buf_tx_232[11] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

    break;
  case STA:
    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;
    buf_tx_232[5] = TERM_ACK;

    buf_tx_232[6] = i_sta_lcp;

    buf_tx_232[7] = 0;
    buf_tx_232[8] = 0x4;

    vol_tx_ppp = buf_tx_232[8] + 8;

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[8] + 4) ^ 0xffff;
    buf_tx_232[9] = temp.bytes[0];
    buf_tx_232[10] = temp.bytes[1];

    buf_tx_232[11] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

    break;

  case SCJ:
    fl_lcp.act_lcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0xff;
    buf_tx_232[2] = 0x03;
    buf_tx_232[3] = 0xc0;
    buf_tx_232[4] = 0x21;
    buf_tx_232[5] = CODE_REJ;

    buf_tx_232[6] = i_scj_lcp;
    i_scj_lcp++;

    temp.word = ln_rejc_lcp + 4;
    buf_tx_232[7] = temp.bytes[1];
    buf_tx_232[8] = temp.bytes[0];

    temp.word = temp.word + 4;
    vol_tx_ppp = temp.word + 4;

    for (i = 0; i < ln_rejc_lcp; i++)
      buf_tx_232[9 + i] = buf_rejc_opt_lcp[i];
    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;

    buf_tx_232[9 + ln_rejc_lcp] = temp.bytes[0];
    buf_tx_232[10 + ln_rejc_lcp] = temp.bytes[1];
    buf_tx_232[11 + ln_rejc_lcp] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];
    break;
    // case SER:
    //      break;
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

    RdFromFleshToArr(A_C_GPRS, &temp1, 1); // количество байт APN
    RdFromFleshToArr(A_C_GPRS + temp1 + 1, &temp.bytes[0],
                     1); // количество логина
    RdFromFleshToArr(A_C_GPRS + temp1 + temp.bytes[0] + 2, &temp.bytes[1],
                     1); // количество пароля

    /*
      if((temp.bytes[0]==0)||(temp.bytes[0]>MAX_VOL_US_NAME)) buf_tx_232[7]=0;
         else RdFromFleshToArr(A_C_GPRS+temp1+2,&buf_tx_232[8],temp.bytes[0]);

      if((temp.bytes[1]==0)||(temp.bytes[1]>MAX_VOL_PSW))
      buf_tx_232[8+buf_tx_232[7]]=0; else
      RdFromFleshToArr(A_C_GPRS+temp1+temp.bytes[0]+3,&buf_tx_232[9+buf_tx_232[7]],temp.bytes[1]);
   */

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

    /*
       //buf_tx_232[7]=sizeof(peer_id);
       //for(i=0;i<sizeof(peer_id);i++)buf_tx_232[8+i]=peer_id[i];
         i=0;
         while(i<MAX_VOL_US_NAME)
              {
               temp1=read_eeprom(ADR_US_NAME+i);
               if (temp1==0x0d)goto ex_name;
               buf_tx_232[8+i]=temp1;
               i++;
              }

      ex_name:buf_tx_232[7]=i;


      // buf_tx_232[8+buf_tx_232[7]]=sizeof(password);
      //
      for(i=0;i<sizeof(password);i++)buf_tx_232[9+buf_tx_232[7]+i]=password[i];

        i=0;
         while(i<MAX_VOL_PSW)
              {
               temp1=read_eeprom(ADR_PSW+i);
               if (temp1==0x0d)goto ex_psw;
               buf_tx_232[9+buf_tx_232[7]+i]=temp1;
               i++;
              }

      ex_psw:buf_tx_232[8+buf_tx_232[7]]=i;

        */

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

    SVD2_1_ON;
    UDR0 = buf_tx_232[0];
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
      cnt_rst_pap = vol_cnt_rst_pap;
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
      vol_cnt_rst_pap = VOL_PAP_MC;
      pap_act_buf = SCR;
      PAP_state = SEND_REQ;
      return;
    }

    return;
  }

  if (fl_pap.t0_mi == 1) // TO_MI
  {
    fl_pap.t0_mi = 0;
    switch (PAP_state) {

    case SEND_REQ:
      event_modem = EVM_PPP_ERR;
      // write_log_info(ST_ERROR,ERR7);
      // lock_it();
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

  case RCN:
    PAP_event = NO_EVENT;
    switch (PAP_state) {

    case SEND_REQ:
      vol_cnt_rst_pap = VOL_PAP_MC;
      // IRC;   //!!!!!!!!!!!!!!111
      cnt_pap_tm_out = VOL_PAP_TM_OUT;
      cnt_rst_pap = vol_cnt_rst_pap;
      fl_pap.pap_tm_out_en = 1;
      fl_pap.act_pap_end = 1;

      pap_act_buf = SCR;
      PAP_state = SEND_REQ;
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
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }

    return;
  }

  if (fl_lcp1.down == 1) // DOWN
  {

    fl_lcp1.down = 0;
    switch (LCP_state) {

    case STOP_ST:
      LCP_act_buf[uk_in_act_LCP] = TLS;
      prov_uk_in();
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case OSTANOV:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case SEND_REQ:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case REC_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case SEND_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    }

    return;
  }

  if (fl_lcp.t0_pl == 1) // TO_PL
  {
    fl_lcp.t0_pl = 0;
    switch (LCP_state) {

    case OSTANOV:
      vol_cnt_rst_lcp = VOL_LCP_MT;
      LCP_act_buf[uk_in_act_LCP] = STR;
      prov_uk_in();
      LCP_state = OSTANOV;
      return;
    case SEND_REQ:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case REC_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_ACK;
      return;
    }

    return;
  }
  if (fl_lcp.t0_mi == 1) // TO_MI
  {
    fl_lcp.t0_mi = 0;
    switch (LCP_state) {

    case OSTANOV:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case SEND_REQ:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case REC_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case SEND_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    }
    return;
  }
  switch (LCP_event) {
  case RCR_PL:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCA;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case OSTANOV:
      LCP_state = OSTANOV;

      return;
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
      vol_cnt_rst_lcp = VOL_LCP_MC;
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

    case STOP_ST:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCN;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case OSTANOV:
      LCP_state = OSTANOV;

      return;
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
      vol_cnt_rst_lcp = VOL_LCP_MC;
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

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = STOP_ST;

      return;
    case OSTANOV:
      LCP_state = OSTANOV;

      return;
    case SEND_REQ:

      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_state = REC_ACK;

      return;
    case REC_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = TLU;
      prov_uk_in();
      LCP_state = OPEN_ST;

      return;
    case OPEN_ST:
      vol_cnt_rst_lcp = VOL_LCP_MC;
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

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = STOP_ST;

      return;
    case OSTANOV:
      LCP_state = OSTANOV;

      return;
    case SEND_REQ:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }
    return;
  case RTR:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = STOP_ST;

      return;
    case OSTANOV:
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = OSTANOV;

      return;
    case SEND_REQ:
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case OPEN_ST:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = ZRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = STA;
      prov_uk_in();
      LCP_state = STOP_ST;

      return;
    }
    return;
  case RTA:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = STOP_ST;

      return;
    case OSTANOV:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;

      return;
    case SEND_REQ:
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      LCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      vol_cnt_rst_lcp = VOL_LCP_MC;
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = SCR;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    }
    return;
  case RUC:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = STOP_ST;

      return;
    case OSTANOV:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      LCP_state = OSTANOV;

      return;
    case SEND_REQ:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      LCP_state = SEND_REQ;

      return;
    case REC_ACK:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      LCP_state = REC_ACK;

      return;
    case SEND_ACK:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      LCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      LCP_act_buf[uk_in_act_LCP] = SCJ;
      prov_uk_in();
      LCP_state = OPEN_ST;

      return;
    }
    return;
  case RXJ_PL:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case OSTANOV:
      LCP_state = OSTANOV;
      return;
    case SEND_REQ:
      LCP_state = SEND_REQ;
      return;
    case REC_ACK:
      LCP_state = REC_ACK;
      return;
    case SEND_ACK:
      LCP_state = SEND_ACK;
      return;
    case OPEN_ST:
      LCP_state = OPEN_ST;
      return;
    }
    return;
  case RXJ_MI:
    LCP_event = NO_EVENT;
    switch (LCP_state) {
    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case OSTANOV:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case SEND_REQ:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case REC_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case SEND_ACK:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_act_buf[uk_in_act_LCP] = TLF;
      prov_uk_in();
      LCP_state = STOP_ST;
      return;
    case OPEN_ST:
      vol_cnt_rst_lcp = VOL_LCP_MT;
      LCP_act_buf[uk_in_act_LCP] = TLD;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = IRC;
      prov_uk_in();
      LCP_act_buf[uk_in_act_LCP] = STR;
      prov_uk_in();
      LCP_state = OSTANOV;
      return;
    }
    return;
  case RXR:
    LCP_event = NO_EVENT;
    switch (LCP_state) {

    case STOP_ST:
      fl_lcp.lcp_tm_out_en = 0;
      LCP_state = START_ST;
      return;
    case OSTANOV:
      LCP_state = OSTANOV;
      return;
    case SEND_REQ:
      LCP_state = SEND_REQ;
      return;
    case REC_ACK:
      LCP_state = REC_ACK;
      return;
    case SEND_ACK:
      LCP_state = SEND_ACK;
      return;
    case OPEN_ST:
      //   LCP_act_buf[uk_in_act_LCP]=SER;
      //   prov_uk_in();
      LCP_state = OPEN_ST;
      return;
    }
    return;
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

    if (Control.vol_link != 0)
      fl_ip.ip_tm_cntr_en = 1;
    if (Control.vol_nat != 0)
      fl_ip.ip_tm_nat_en = 1;
    Control.cnt_try = 0;
    Control.cnt_link = BEG_CNTR_LINK;
    clr_cntr_nat();

    break;
  case TLD:
    fl_ipcp.act_ipcp_end = 0;
    layer_PPP = LAYER_LCP;
    fl_ipcp.act_ipcp_end = 1;
    break;
  case TLS:
    fl_ipcp.act_ipcp_end = 0;
    event_modem = EVM_PPP_ERR;
    // write_log_info(ST_ERROR,ERR8);
    // lock_it();
    break;
  case TLF:
    fl_ipcp.act_ipcp_end = 0;
    event_modem = EVM_PPP_ERR;
    // write_log_info(ST_ERROR,ERR9);
    // lock_it();
    break;
  case IRC:
    fl_ipcp.act_ipcp_end = 0;
    cnt_ipcp_tm_out = VOL_IPCP_TM_OUT;
    cnt_rst_ipcp = vol_cnt_rst_ipcp;
    fl_ipcp.ipcp_tm_out_en = 1;
    fl_ipcp.act_ipcp_end = 1;

    break;
  case ZRC:
    fl_ipcp.act_ipcp_end = 0;
    cnt_ipcp_tm_out = VOL_IPCP_TM_OUT;
    cnt_rst_ipcp = 0;
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
    buf_tx_232[6] = 10;

    buf_tx_232[7] = 3; // запрос IP контроллера
    buf_tx_232[8] = 6;
    buf_tx_232[9] = ip_self[0];
    buf_tx_232[10] = ip_self[1];
    buf_tx_232[11] = ip_self[2];
    buf_tx_232[12] = ip_self[3];

    /*
    buf_tx_232[13]=0x81;      //запрос IP_PRI_DNS
    buf_tx_232[14]=6;
    buf_tx_232[15]=ip_pri_dns[0];
    buf_tx_232[16]=ip_pri_dns[1];
    buf_tx_232[17]=ip_pri_dns[2];
    buf_tx_232[18]=ip_pri_dns[3];

    buf_tx_232[19]=0x83;      //запрос IP_SEC_DNS
    buf_tx_232[20]=6;
    buf_tx_232[21]=ip_sec_dns[0];
    buf_tx_232[22]=ip_sec_dns[1];
    buf_tx_232[23]=ip_sec_dns[2];
    buf_tx_232[24]=ip_sec_dns[3];
    */

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[6] + 2) ^ 0xffff;
    buf_tx_232[13] = temp.bytes[0];
    buf_tx_232[14] = temp.bytes[1];

    buf_tx_232[15] = 0x7e;

    count_tx_ppp = 0;
    vol_tx_ppp = buf_tx_232[6] + 6;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

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
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

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
      SVD2_1_ON;
      UDR0 = buf_tx_232[0];

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
      SVD2_1_ON;
      UDR0 = buf_tx_232[0];
    }

    break;
  case STR:
    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;

    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    buf_tx_232[3] = TERM_REQ;
    buf_tx_232[4] = i_str_ipcp;

    buf_tx_232[5] = 0;
    buf_tx_232[6] = 0x4;

    vol_tx_ppp = buf_tx_232[8] + 6;

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[6] + 2) ^ 0xffff;
    buf_tx_232[7] = temp.bytes[0];
    buf_tx_232[8] = temp.bytes[1];

    buf_tx_232[9] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

    break;
  case STA:
    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    buf_tx_232[3] = TERM_ACK;

    buf_tx_232[4] = i_sta_ipcp;

    buf_tx_232[5] = 0;
    buf_tx_232[6] = 0x4;

    vol_tx_ppp = buf_tx_232[6] + 6;

    temp.word =
        pppfcs16(PPPINITFCS16, &buf_tx_232[1], buf_tx_232[6] + 2) ^ 0xffff;
    buf_tx_232[7] = temp.bytes[0];
    buf_tx_232[8] = temp.bytes[1];

    buf_tx_232[9] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

    break;

  case SCJ:
    fl_ipcp.act_ipcp_end = 0;
    buf_tx_232[0] = 0x7e;
    buf_tx_232[1] = 0x80;
    buf_tx_232[2] = 0x21;

    buf_tx_232[3] = CODE_REJ;

    buf_tx_232[4] = i_scj_lcp;
    i_scj_lcp++;

    temp.word = ln_rejc_ipcp + 4;
    buf_tx_232[5] = temp.bytes[1];
    buf_tx_232[6] = temp.bytes[0];

    temp.word = temp.word + 2;
    vol_tx_ppp = temp.word + 4;

    for (i = 0; i < ln_rejc_ipcp; i++)
      buf_tx_232[7 + i] = buf_rejc_opt_ipcp[i];
    temp.word = pppfcs16(PPPINITFCS16, &buf_tx_232[1], temp.word) ^ 0xffff;

    buf_tx_232[7 + ln_rejc_ipcp] = temp.bytes[0];
    buf_tx_232[8 + ln_rejc_ipcp] = temp.bytes[1];
    buf_tx_232[9 + ln_rejc_ipcp] = 0x7e;

    count_tx_ppp = 0;
    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];
    break;
    // case SER:
    //      break;
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
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }

    return;
  }

  if (fl_ipcp1.down == 1) // DOWN
  {

    fl_ipcp1.down = 0;
    switch (IPCP_state) {

    case STOP_ST:
      IPCP_act_buf[uk_in_act_IPCP] = TLS;
      prov_uk_in_ipcp();
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case OSTANOV:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case SEND_REQ:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case REC_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case SEND_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case OPEN_ST:
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    }

    return;
  }

  if (fl_ipcp.t0_pl == 1) // TO_PL
  {
    fl_ipcp.t0_pl = 0;
    switch (IPCP_state) {

    case OSTANOV:
      vol_cnt_rst_ipcp = VOL_IPCP_MT;
      IPCP_act_buf[uk_in_act_IPCP] = STR;
      prov_uk_in_ipcp();
      IPCP_state = OSTANOV;
      return;
    case SEND_REQ:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;
      return;
    case REC_ACK:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;
      return;
    case SEND_ACK:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;
      return;
    }

    return;
  }
  if (fl_ipcp.t0_mi == 1) // TO_MI
  {
    fl_ipcp.t0_mi = 0;
    switch (IPCP_state) {

    case OSTANOV:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case SEND_REQ:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case REC_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case SEND_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    }
    return;
  }
  switch (IPCP_event) {
  case RCR_PL:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    case OSTANOV:
      IPCP_state = OSTANOV;

      return;
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
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    }
    return;
  case RCR_MI:

    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCN;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case OSTANOV:
      IPCP_state = OSTANOV;

      return;
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
    case OPEN_ST:
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCN;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }
    return;
  case RCA:

    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;

      return;
    case OSTANOV:
      IPCP_state = OSTANOV;

      return;
    case SEND_REQ:

      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_state = REC_ACK;

      return;
    case REC_ACK:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
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
    case OPEN_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }
    return;
  case RCN:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;

      return;
    case OSTANOV:
      IPCP_state = OSTANOV;

      return;
    case SEND_REQ:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }
    return;
  case RTR:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;

      return;
    case OSTANOV:
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = OSTANOV;

      return;
    case SEND_REQ:
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case OPEN_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = ZRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = STA;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;

      return;
    }
    return;
  case RTA:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = STOP_ST;

      return;
    case OSTANOV:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;

      return;
    case SEND_REQ:
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      IPCP_state = SEND_REQ;

      return;
    case SEND_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MC;
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = SCR;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    }
    return;
  case RUC:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = STOP_ST;

      return;
    case OSTANOV:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      IPCP_state = OSTANOV;

      return;
    case SEND_REQ:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      IPCP_state = SEND_REQ;

      return;
    case REC_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      IPCP_state = REC_ACK;

      return;
    case SEND_ACK:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      IPCP_state = SEND_ACK;

      return;
    case OPEN_ST:
      IPCP_act_buf[uk_in_act_IPCP] = SCJ;
      prov_uk_in_ipcp();
      IPCP_state = OPEN_ST;

      return;
    }
    return;
  case RXJ_PL:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case OSTANOV:
      IPCP_state = OSTANOV;
      return;
    case SEND_REQ:
      IPCP_state = SEND_REQ;
      return;
    case REC_ACK:
      IPCP_state = REC_ACK;
      return;
    case SEND_ACK:
      IPCP_state = SEND_ACK;
      return;
    case OPEN_ST:
      IPCP_state = OPEN_ST;
      return;
    }
    return;
  case RXJ_MI:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {
    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case OSTANOV:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case SEND_REQ:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case REC_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case SEND_ACK:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_act_buf[uk_in_act_IPCP] = TLF;
      prov_uk_in_ipcp();
      IPCP_state = STOP_ST;
      return;
    case OPEN_ST:
      vol_cnt_rst_ipcp = VOL_IPCP_MT;
      IPCP_act_buf[uk_in_act_IPCP] = TLD;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = IRC;
      prov_uk_in_ipcp();
      IPCP_act_buf[uk_in_act_IPCP] = STR;
      prov_uk_in_ipcp();
      IPCP_state = OSTANOV;
      return;
    }
    return;
  case RXR:
    IPCP_event = NO_EVENT;
    switch (IPCP_state) {

    case STOP_ST:
      fl_ipcp.ipcp_tm_out_en = 0;
      IPCP_state = START_ST;
      return;
    case OSTANOV:
      IPCP_state = OSTANOV;
      return;
    case SEND_REQ:
      IPCP_state = SEND_REQ;
      return;
    case REC_ACK:
      IPCP_state = REC_ACK;
      return;
    case SEND_ACK:
      IPCP_state = SEND_ACK;
      return;
    case OPEN_ST:
      //   IPCP_act_buf[uk_in_act_IPCP]=SER;
      //   prov_uk_in_ipcp();
      IPCP_state = OPEN_ST;
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

      for (i = 0; i < vol_length_opt; i++) // соглашение на любую конфигурацию
      {
        buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
        ln_ack_lcp++;
        // count_length_opt++;
        ind_opt++;
      }

      LCP_event = RCR_PL;
      i_scan_lcp = buf_rx_ppp[6]; // присвоение идентификатора
      return (1);

    next_opt_lcp_1:
      if (count_length_opt == vol_length_opt) {

        goto end_pcp_opt_1;
      } // обработаны все опции

      switch (buf_rx_ppp[ind_opt]) {
      case MRU:

        if (buf_rx_ppp[ind_opt + 1] != 4) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }
        if ((buf_rx_ppp[ind_opt + 2] == DL_PPP_IN_HI) &&
            (buf_rx_ppp[ind_opt + 3] == DL_PPP_IN_LO)) {
          fl_opt.mru = 1;
          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 1];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 2];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 3];
          ln_ack_lcp++;
        } else {
          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 1];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 2];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 3];
          ln_nak_lcp++;
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;

      case ACCM:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        if ((buf_rx_ppp[ind_opt + 2] == 0) && (buf_rx_ppp[ind_opt + 3] == 0) &&
            (buf_rx_ppp[ind_opt + 4] == 0) && (buf_rx_ppp[ind_opt + 5] == 0))

        {

          fl_opt.accm = 1;
          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 1];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 2];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 3];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 4];
          ln_ack_lcp++;

          buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 5];
          ln_ack_lcp++;
        } else {
          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 1];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 2];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 3];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 4];
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 5];
          ln_nak_lcp++;
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;

      case AUT_PROT:
        if (((buf_rx_ppp[ind_opt + 1] - 2) % 2) != 0) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }
        for (i = 0; i < (buf_rx_ppp[ind_opt + 1] - 2); i = i + 2) {
          if ((buf_rx_ppp[ind_opt + 2 + i] == 0xc0) &&
              (buf_rx_ppp[ind_opt + 3 + i] == 0x23)) {

            fl_opt.aut_prot = 1;

            buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
            ln_ack_lcp++;

            buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 1];
            ln_ack_lcp++;

            buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 2 + i];
            ln_ack_lcp++;

            buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 3 + i];
            ln_ack_lcp++;
          } else {

            if (fl_opt.zap_opt == 0) {
              fl_opt.zap_opt = 1;
              buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt];
              ln_nak_lcp++;
              buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 1];
              ln_nak_lcp++;
            }
            buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 2 + i];
            ln_nak_lcp++;
            buf_nak_opt_lcp[ln_nak_lcp] = buf_rx_ppp[ind_opt + 3 + i];
            ln_nak_lcp++;
          }
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;

      case PFC:
        if (buf_rx_ppp[ind_opt + 1] != 2) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        fl_opt.pfc = 1;
        buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
        ln_ack_lcp++;

        buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 1];
        ln_ack_lcp++;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;

      case ACFC:
        if (buf_rx_ppp[ind_opt + 1] != 2) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        fl_opt.acfc = 1;
        buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt];
        ln_ack_lcp++;

        buf_ack_opt_lcp[ln_ack_lcp] = buf_rx_ppp[ind_opt + 1];
        ln_ack_lcp++;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;

      default:

        for (i = 0; i < buf_rx_ppp[ind_opt + 1]; i++) {
          buf_rej_opt_lcp[ln_rej_lcp] = buf_rx_ppp[ind_opt + i];
          ln_rej_lcp++;
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_1;
      }

    end_pcp_opt_1:

      if (ln_rej_lcp != 0) {

        LCP_event = RCR_MI;
        fl_lcp2.nac_rej = 1;
        return (1);
      }

      /*

    if(fl_opt.mru==0)
         {

          buf_nak_opt_lcp[ln_nak_lcp]=MRU;
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp]=4;
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp]=DL_PPP_IN_HI;
          ln_nak_lcp++;

          buf_nak_opt_lcp[ln_nak_lcp]=DL_PPP_IN_LO;
          ln_nak_lcp++;
         }

         */

      if (fl_opt.accm == 0) {
        buf_nak_opt_lcp[ln_nak_lcp] = ACCM;
        ln_nak_lcp++;
        buf_nak_opt_lcp[ln_nak_lcp] = 6;
        ln_nak_lcp++;
        buf_nak_opt_lcp[ln_nak_lcp] = 0;
        ln_nak_lcp++;
        buf_nak_opt_lcp[ln_nak_lcp] = 0;
        ln_nak_lcp++;
        buf_nak_opt_lcp[ln_nak_lcp] = 0;
        ln_nak_lcp++;
        buf_nak_opt_lcp[ln_nak_lcp] = 0;
        ln_nak_lcp++;
      }

      if (fl_opt.aut_prot == 0) {

        buf_nak_opt_lcp[ln_nak_lcp] = AUT_PROT;
        ln_nak_lcp++;

        buf_nak_opt_lcp[ln_nak_lcp] = 4;
        ln_nak_lcp++;

        buf_nak_opt_lcp[ln_nak_lcp] = 0xc0;
        ln_nak_lcp++;

        buf_nak_opt_lcp[ln_nak_lcp] = 0x23;
        ln_nak_lcp++;
      }
      if (fl_opt.pfc == 0) {
        buf_nak_opt_lcp[ln_nak_lcp] = PFC;
        ln_nak_lcp++;

        buf_nak_opt_lcp[ln_nak_lcp] = 2;
        ln_nak_lcp++;
      }
      if (fl_opt.acfc == 0) {
        buf_nak_opt_lcp[ln_nak_lcp] = ACFC;
        ln_nak_lcp++;

        buf_nak_opt_lcp[ln_nak_lcp] = 2;
        ln_nak_lcp++;
      }

      if (ln_nak_lcp != 0) {

        LCP_event = RCR_MI;
        fl_lcp2.nac_rej = 0;
        i_scan_lcp = buf_rx_ppp[6]; // присвоение идентификатора
        return (1);
      }

      if (ln_ack_lcp != 0) {

        LCP_event = RCR_PL;
        i_scan_lcp = buf_rx_ppp[6]; // присвоение идентификатора
        return (1);
      }

      break;

    case CONF_ACK:
      // проверка опций (если вообще не те опции то сброс кода дать)
      // опции та, то событие RCA

      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора

    next_opt_lcp_2:
      if (count_length_opt == vol_length_opt)
        goto end_pcp_opt_2; // обработаны все опции

      switch (buf_rx_ppp[ind_opt]) {
      case ACCM:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        if ((buf_rx_ppp[ind_opt + 2] == 0) &&
            (buf_rx_ppp[ind_opt + 3] == 0x0a) &&
            (buf_rx_ppp[ind_opt + 4] == 0) && (buf_rx_ppp[ind_opt + 5] == 0))
          fl_opt.accm = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_2;

      case PFC:
        if (buf_rx_ppp[ind_opt + 1] != 2) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        fl_opt.pfc = 1;
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_2;

      case ACFC:
        if (buf_rx_ppp[ind_opt + 1] != 2) {
          for (i = 5; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
            ln_rejc_lcp++;
          }
          LCP_event = RUC;
          return (1);
        }

        fl_opt.acfc = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_lcp_2;

      default:
        for (i = 5; i <= count_rx_ppp - 4; i++) {
          buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
          ln_rejc_lcp++;
        }
        LCP_event = RUC;
        return (1);
      }

    end_pcp_opt_2:
      if ((fl_opt.accm == 1) && (fl_opt.acfc == 1) && (fl_opt.pfc == 1)) {

        LCP_event = RCA;
        i_scr_lcp++; // приращение идентификатора
        return (1);
      } else {
        for (i = 5; i <= count_rx_ppp - 4; i++) {
          buf_rejc_opt_lcp[ln_rejc_lcp] = buf_rx_ppp[i];
          ln_rejc_lcp++;
        }
        LCP_event = RUC;
        return (1);
      }

    case CONF_NAK:
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      i_scr_lcp++;  // приращение идентификатора
      LCP_event = RCN;

      return (1);
    case CONF_REJ:
      // то RCN но все равно запрос тех же опций
      if (buf_rx_ppp[6] != i_scr_lcp)
        return (1); // проверка идентификатора
      i_scr_lcp++;  // приращение идентификатора
      LCP_event = RCN;

      return (1);

    case TERM_REQ:
      LCP_event = RTR;
      i_sta_lcp = buf_rx_ppp[6]; // присвоение идентификатора
      return (1);
    case TERM_ACK:
      if (buf_rx_ppp[6] != i_str_lcp)
        return (1);
      LCP_event = RTA;
      i_str_lcp++; // приращение идентификатора
      return (1);
    case CODE_REJ:; // как то сигнализировать. но ничего не делать
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
    case CONF_NAK:
      if (buf_rx_ppp[4] != i_scr_pap)
        return (1); // проверка идентификатора
      PAP_event = RCN;
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

    next_opt_ipcp_1:
      if (count_length_opt == vol_length_opt)
        goto end_ipcp_opt_1; // обработаны все опции

      switch (buf_rx_ppp[ind_opt]) {

      case IP_SELF:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
          return (1);
        }

        fl_opt.ip_self = 1;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt];
        ln_ack_ipcp++;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + 1];
        ln_ack_ipcp++;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + 2];
        ip_serv_gprs[0] = buf_rx_ppp[ind_opt + 2];
        ln_ack_ipcp++;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + 3];
        ip_serv_gprs[1] = buf_rx_ppp[ind_opt + 3];
        ln_ack_ipcp++;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + 4];
        ip_serv_gprs[2] = buf_rx_ppp[ind_opt + 4];
        ln_ack_ipcp++;

        buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + 5];
        ip_serv_gprs[3] = buf_rx_ppp[ind_opt + 5];
        ln_ack_ipcp++;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_1;

      /*
       case IP_PRI_DNS:
       case IP_SEC_DNS:
             if (buf_rx_ppp[ind_opt+1]!=6)
                 {
                   for(i=3;i<=count_rx_ppp-4;i++){buf_rejc_opt_ipcp[ln_rejc_ipcp]=buf_rx_ppp[i];ln_rejc_ipcp++;}
                   IPCP_event=RUC;
                   return(1);
                   }

                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt];
                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt+1];
                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt+2];
                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt+3];
                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt+4];
                   buf_ack_opt_ipcp[ln_ack_ipcp]=buf_rx_ppp[ind_opt+5];
                   ln_ack_ipcp=ln_ack_ipcp+6;

            count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
            ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
            goto next_opt_ipcp_1;




       default:

                   for(i=0;i<buf_rx_ppp[ind_opt+1];i++){buf_rej_opt_ipcp[ln_rej_ipcp]=buf_rx_ppp[ind_opt+i];ln_rej_ipcp++;}
                   count_length_opt=count_length_opt+buf_rx_ppp[ind_opt+1];
                   ind_opt=ind_opt+buf_rx_ppp[ind_opt+1];
             goto next_opt_ipcp_1;
    */
      default: // подтверждать все !!!!!!!!!!

        for (i = 0; i < buf_rx_ppp[ind_opt + 1]; i++) {
          buf_ack_opt_ipcp[ln_ack_ipcp] = buf_rx_ppp[ind_opt + i];
          ln_ack_ipcp++;
        }
        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_1;
      }

    end_ipcp_opt_1:

      if (ln_rej_ipcp != 0) {

        IPCP_event = RCR_MI;
        fl_ipcp2.nac_rej = 1;
        return (1);
      }

      if (fl_opt.ip_self == 0)
        return (1);

      if (ln_ack_ipcp != 0) {

        IPCP_event = RCR_PL;
        i_scan_ipcp = buf_rx_ppp[4]; // присвоение идентификатора
        return (1);
      }

      break;

    case CONF_ACK:
      // проверка опций (если вообще не те опции то сброс кода дать)
      // опции та, то событие RCA
      //  led2_on;    //пока
      //  lock_it();    // пока

      if (buf_rx_ppp[4] != i_scr_ipcp)
        return (1); // проверка идентификатора

    next_opt_ipcp_2:
      if (count_length_opt == vol_length_opt)
        goto end_ipcp_opt_2; // обработаны все опции

      switch (buf_rx_ppp[ind_opt]) {

      case IP_SELF:
        if (buf_rx_ppp[ind_opt + 1] != 6) {

          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
          return (1);
        }

        if (

            (buf_rx_ppp[ind_opt + 2] == ip_self[0]) &&
            (buf_rx_ppp[ind_opt + 3] == ip_self[1]) &&
            (buf_rx_ppp[ind_opt + 4] == ip_self[2]) &&
            (buf_rx_ppp[ind_opt + 5] == ip_self[3])) {
          fl_opt.ip_self = 1;
        }

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_2;

      case IP_PRI_DNS:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
          return (1);
        }

        if ((buf_rx_ppp[ind_opt + 2] == ip_pri_dns[0]) &&
            (buf_rx_ppp[ind_opt + 3] == ip_pri_dns[1]) &&
            (buf_rx_ppp[ind_opt + 4] == ip_pri_dns[2]) &&
            (buf_rx_ppp[ind_opt + 5] == ip_pri_dns[3]))
          fl_opt.ip_pri_dns = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_2;

      case IP_SEC_DNS:
        if (buf_rx_ppp[ind_opt + 1] != 6) {
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
          return (1);
        }

        if ((buf_rx_ppp[ind_opt + 2] == ip_sec_dns[0]) &&
            (buf_rx_ppp[ind_opt + 3] == ip_sec_dns[1]) &&
            (buf_rx_ppp[ind_opt + 4] == ip_sec_dns[2]) &&
            (buf_rx_ppp[ind_opt + 5] == ip_sec_dns[3]))
          fl_opt.mru = 1;

        count_length_opt = count_length_opt + buf_rx_ppp[ind_opt + 1];
        ind_opt = ind_opt + buf_rx_ppp[ind_opt + 1];
        goto next_opt_ipcp_2;

      default:
        for (i = 3; i <= count_rx_ppp - 4; i++) {
          buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
          ln_rejc_ipcp++;
        }
        IPCP_event = RUC;
        return (1);
      }

    end_ipcp_opt_2: // if((fl_opt.ip_self==1)&&(fl_opt.ip_pri_dns==1)&&(fl_opt.mru==1))
      if (fl_opt.ip_self == 1) {

        IPCP_event = RCA;
        i_scr_ipcp++; // приращение идентификатора
        return (1);
      } else {
        for (i = 3; i <= count_rx_ppp - 4; i++) {
          buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
          ln_rejc_ipcp++;
        }
        IPCP_event = RUC;
        return (1);
      }

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
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
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
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
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
          for (i = 3; i <= count_rx_ppp - 4; i++) {
            buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
            ln_rejc_ipcp++;
          }
          IPCP_event = RUC;
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

      default:
        for (i = 3; i <= count_rx_ppp - 4; i++) {
          buf_rejc_opt_ipcp[ln_rejc_ipcp] = buf_rx_ppp[i];
          ln_rejc_ipcp++;
        }
        IPCP_event = RUC;
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

    case TERM_REQ:
      IPCP_event = RTR;
      i_sta_ipcp = buf_rx_ppp[4]; // присвоение идентификатора
      return (1);
    case TERM_ACK:
      if (buf_rx_ppp[4] != i_str_ipcp)
        return (1);
      IPCP_event = RTA;
      i_str_ipcp++; // приращение идентификатора
      return (1);
    case CODE_REJ:; // как то сигнализировать. но ничего не делать
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

    // if(proc_udp_data()==0)return(1);
    /*
     // Crc_out.bytes[1]=buf_rx_ppp[30];    //раскодировка
     // Crc_out.bytes[0]=buf_rx_ppp[31];
     // temp.bytes[1]=buf_rx_ppp[26];
     // temp.bytes[0]=buf_rx_ppp[27];
     // temp.word=temp.word-8;
     // transform_buf(&buf_rx_ppp[32],temp.word,Crc_out.word);

  if((buf_rx_ppp[36]>0)&&(buf_rx_ppp[36]<13))
  if((buf_rx_ppp[37]& 0x01)==1)              // контрольная квитанция для шлюза
     {
      PORTB^=LED_SOST_S; //пока
      if(buf_rx_ppp[36]==0)return(1);
     }
      */

    break;
  }

  return (1);
}

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

void monitor_tx_rs485_2(void) {
  unsigned int i;
  if (fl_485_2.busy == 1)
    return;
  if ((fl_485_2.tx == 1) || (Appl_RS485_2.fl_data_buf == TRUE)) {
    if (fl_485_2.tx == 1)
      fl_485_2.tx = 0;
    if (Appl_RS485_2.fl_data_buf == TRUE)
      Appl_RS485_2.fl_data_buf = FALSE;

    fl_485_2.busy = 1;

    reset_em_485_err();

    Appl_RS485_2.dst_tek = Appl_RS485_2.dst_buf;
    Appl_RS485_2.id_tek = Appl_RS485_2.id_buf;
    Rs485_2.cnt_bt_rx_tx = Appl_RS485_2.ln_data_buf;
    for (i = 0; i < Rs485_2.cnt_bt_rx_tx; i++)
      Rs485_2_buf_rx_tx[i] = Appl_RS485_2_data_buf[i];
    Rs485_2.p_data485 = &Rs485_2_buf_rx_tx[0];

    // здесь должна быть функция настройки порта по контексту.

    if (run_cont_485_2(&Appl_RS485_2.cont_buf[0]) == 1) {
      fl_485_2.busy = 0;
      return;
    }

    /*

  //  Emul_485.bod=6667;//2400
   //  Emul_485.bod=3333;//4800
   // Emul_485.bod=1667;//9600
     Emul_485.bod=1600;//9600


    Rs485_2.cnt_tm_tx_out=0;
    Rs485_2.vol_tm_tx_out=0; // 0 миллисекунд
    Rs485_2.cnt_tm_pre_tx=1;// 20 миллисекунд

    Rs485_2.cnt_tm_rx_out=0;
    Rs485_2.vol_tm_rx_out=5;// 5 миллисекунд

    Rs485_2.cnt_tm_out=0;
    Rs485_2.vol_tm_out=1000; // 1000 миллисекунд
    */

    send_em_485();
  }
}

void monitor2_tx_rs485_1(void) {
  unsigned int i;
  if (fl_485_1.busy == 1)
    return;
  if (fl_232_2.buffed == 1) {
    fl_232_2.buffed = 0;
    fl_485_1.busy = 1;
    fl_tx485.mon2 = 1;

    Rs485_1.cnt_bt_rx_tx = Rs232_2.cnt_bt_rx_tx;
    for (i = 0; i < Rs485_1.cnt_bt_rx_tx; i++)
      Rs485_1_buf_rx_tx[i] = Rs232_2_buf_rx_tx[i];
    Rs485_1.p_data485 = &Rs485_1_buf_rx_tx[0];
    // здесь должна быть функция настройки порта по контексту.
    // UBRR1H=R9600_H;
    // UBRR1L=R9600_L;

    // UBRR1H=R4800_H;
    // UBRR1L=R4800_L;

    UBRR1H = R2400_H;
    UBRR1L = R2400_L;

    UCSR1B = UCSR1B & ~(RXCIE | RXEN);
    Rs485_1.cnt_tm_tx_out = 0;

    // Rs485_1.vol_tm_tx_out=8;

    Rs485_1.vol_tm_tx_out = 15;

    // Rs485_1.cnt_tm_pre_tx=9;// 9 миллисекунд
    Rs485_1.cnt_tm_pre_tx = 20; // 20 миллисекунд

    Rs485_1.cnt_tm_rx_out = 0;
    // Rs485_1.vol_tm_rx_out=5;// 5 миллисекунд
    Rs485_1.vol_tm_rx_out = 10; // 5 миллисекунд

    Rs485_1.cnt_tm_out = 0;
    Rs485_1.vol_tm_out = 1000; // 1000 миллисекунд
    // Rs485_1.vol_tm_out=100; // 100 миллисекунд

    set_rts_485_1; // rts на передачу
  }
}

void monitor2_tm_rs485_1(void) {
  if (fl_tx485.mon2 == 0)
    return;
  if (fl_485_1.tm_out == 0)
    return;

  fl_485_1.tm_out = 0; // вернуть на прием модем 160
  fl_232_2.busy = 0;
  fl_232_2_add.was_tx = 1; // флаг того что была передача
  fl_232_2_add.sinc = 0;   // очистка того что принимались биты
  Emul_232_cnt_post_tx =
      DEF_EM232_POST_TX; // установка защитного интервала на прием
  reset_em_232_new();    // переключение на прием с поиском нового синхро
  fl_485_1.busy = 0;
  fl_tx485.mon2 = 0;
}

void monitor2_rec_rs485_1(void) {
  unsigned int i;
  if (fl_tx485.mon2 == 0)
    return;
  if (fl_485_1.rec == 0)
    return;
  fl_485_1.rec = 0;

  if (fl_485_1.over == 1)
    fl_485_1.over = 0; // все равно посылка сколько нагреблось

  if (Rs485_1.cnt_bt_rx_tx > MAX_BUF_RS232_2)
    Rs232_2.cnt_bt_rx_tx = MAX_BUF_RS232_2;
  else
    Rs232_2.cnt_bt_rx_tx = Rs485_1.cnt_bt_rx_tx; // выдать посылку модема

  if (kamintel == 1)
    Rs232_2.cnt_bt_rx_tx--;

  for (i = 0; i < Rs232_2.cnt_bt_rx_tx; i++)
    Rs232_2_buf_rx_tx[i] = Rs485_1_buf_rx_tx[i]; // перепись в буфер

  Rs232_2.p_data232 = &Rs232_2_buf_rx_tx[0];
  send_em_232_2();

  Rs485_1.cnt_bt_rx_tx = 0;
  fl_485_1.busy = 0;
  fl_tx485.mon2 = 0;
  return;
}

void monitor1_tx_rs485_1(void) {
  unsigned int i;
  if (fl_485_1.busy == 1)
    return;
  if ((fl_485_1.tx == 1) || (Appl_RS485_1.fl_data_buf == TRUE)) {
    if (fl_485_1.tx == 1)
      fl_485_1.tx = 0;
    if (Appl_RS485_1.fl_data_buf == TRUE)
      Appl_RS485_1.fl_data_buf = FALSE;
    fl_485_1.busy = 1;
    fl_tx485.mon1 = 1;
    Appl_RS485_1.dst_tek = Appl_RS485_1.dst_buf;
    Appl_RS485_1.id_tek = Appl_RS485_1.id_buf;
    Rs485_1.cnt_bt_rx_tx = Appl_RS485_1.ln_data_buf;
    for (i = 0; i < Rs485_1.cnt_bt_rx_tx; i++)
      Rs485_1_buf_rx_tx[i] = Appl_RS485_1_data_buf[i];
    Rs485_1.p_data485 = &Rs485_1_buf_rx_tx[0];
    // здесь должна быть функция настройки порта по контексту.

    if (run_cont_485_1(&Appl_RS485_1.cont_buf[0]) == 1) {
      fl_485_1.busy = 0;
      fl_tx485.mon1 = 0;
      return;
    }

    /*
    //UBRR1H=R9600_H;
    //UBRR1L=R9600_L;
    UBRR1H=R4800_H;
    UBRR1L=R4800_L;
    UCSR1B=UCSR1B & ~(RXCIE | RXEN);
    Rs485_1.cnt_tm_tx_out=0;
    Rs485_1.vol_tm_tx_out=5; // 5 миллисекунд
    Rs485_1.cnt_tm_pre_tx=20;// 20 миллисекунд
    Rs485_1.cnt_tm_rx_out=0;
    Rs485_1.vol_tm_rx_out=5;// 5 миллисекунд
    Rs485_1.cnt_tm_out=0;
    Rs485_1.vol_tm_out=1000; // 1000 миллисекунд
    */

    UCSR1B = UCSR1B & ~(RXCIE | RXEN);
    set_rts_485_1; // rts на передачу
  }
}

void monitor1_tm_rs485_1(void) {
  if (fl_tx485.mon1 == 0)
    return;
  if (fl_ip.act_ip_end != 1)
    return;
  if (fl_485_1.tm_out == 0)
    return;

  __disable_interrupt();
  lufkin_port_off;
  __enable_interrupt();

  clr_cntr_nat();
  clr_cntr_link();

  fl_485_1.tm_out = 0;
  if ((PIND & DCD) == 0)
    send_err485(
        NUM_RS485_1, RS_NO_LINK, Appl_RS485_1.id_tek,
        Appl_RS485_1.dst_tek); // игнорировать передачу если нет GPRS канала
  fl_485_1.busy = 0;
  fl_tx485.mon1 = 0;
}

void monitor1_rec_rs485_1(void) {
  unsigned int i;
  if (fl_tx485.mon1 == 0)
    return;
  if (fl_ip.act_ip_end != 1)
    return;
  if (fl_485_1.rec == 0)
    return;

  fl_485_1.rec = 0;

  __disable_interrupt();
  lufkin_port_off;
  __enable_interrupt();

  clr_cntr_nat();
  clr_cntr_link();

  if (PIND & DCD) {
    fl_485_1.over = 0; // игнорировать передачу если нет GPRS канала
    Rs485_1.cnt_bt_rx_tx = 0;
    fl_485_1.busy = 0;
    fl_tx485.mon1 = 0;
    return;
  }

  if (fl_485_1.over == 1) {
    fl_485_1.over = 0;
    send_err485(NUM_RS485_1, RS_OVER_BUF_RX, Appl_RS485_1.id_tek,
                Appl_RS485_1.dst_tek);
    Rs485_1.cnt_bt_rx_tx = 0;
    fl_485_1.busy = 0;
    fl_tx485.mon1 = 0;
    return;
  }

  //!!!!!!!!!!!!!! здесь бы надо проверк на длину буфера для PPP
  if (Rs485_1.cnt_bt_rx_tx + 100 > VOL_TX_PPP) {
    send_err485(NUM_RS485_1, RS_OVER_BUF_PPP_TX, Appl_RS485_1.id_tek,
                Appl_RS485_1.dst_tek);
    Rs485_1.cnt_bt_rx_tx = 0;
    fl_485_1.busy = 0;
    fl_tx485.mon1 = 0;
    return;
  }

  fl_ip.act_ip_end = 0;

  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER1;                 // версия
  Obj_ppp_tx.type_pac = OTV;                 // тип пакета
  Obj_ppp_tx.num_src = num_self;             // номер отправителя
  Obj_ppp_tx.num_dst = Appl_RS485_1.dst_tek; // номер получателя
  Obj_ppp_tx.id_pac = Appl_RS485_1.id_tek;   // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0];         // //указатель буфера опций
  Obj_ppp_tx.l_opt = 0;                      // длина буфера опций
  Obj_ppp_tx.kol_opt = 0;                    // количество опций

  buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
  buf_tx_232[TR_OP_DATA + C1_PORT] = PORT485_1;

  for (i = 0; i < Rs485_1.cnt_bt_rx_tx; i++)
    buf_tx_232[TR_OP_DATA + C1_DATA + i] = Rs485_1_buf_rx_tx[i];
  Obj_ppp_tx.l_data = 2 + Rs485_1.cnt_bt_rx_tx; // длина данных
  Obj_ppp_tx.p_data =
      &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных
  Rs485_1.cnt_bt_rx_tx = 0;

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  fl_485_1.busy = 0;
  fl_tx485.mon1 = 0;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void send_err485(unsigned char port, unsigned char err, unsigned char id,
                 unsigned int dst) {
  if (fl_ip.act_ip_end != 1)
    return;
  fl_ip.act_ip_end = 0;
  clr_cntr_nat();
  clr_cntr_link();
  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER1;         // версия
  Obj_ppp_tx.type_pac = OTV;         // тип пакета
  Obj_ppp_tx.num_src = num_self;     // номер отправителя
  Obj_ppp_tx.num_dst = dst;          // номер получателя
  Obj_ppp_tx.id_pac = id;            // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций
  Obj_ppp_tx.l_opt = 0;              // длина буфера опций
  Obj_ppp_tx.kol_opt = 0;            // количество опций

  buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
  buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_SYS;

  buf_tx_232[TR_OP_DATA + C1_DATA] = RS485_ERR;
  *(unsigned int *)&buf_tx_232[TR_OP_DATA + C1_DATA + 1] = LN_RS485_ERR;
  buf_tx_232[TR_OP_DATA + C1_DATA + 3] = port;
  buf_tx_232[TR_OP_DATA + C1_DATA + 4] = err;
  Obj_ppp_tx.l_data = 5 + LN_RS485_ERR; // длина данных
  Obj_ppp_tx.p_data =
      &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void monitor_tm_rs485_2(void) {
  if (fl_ip.act_ip_end != 1)
    return;
  if (fl_485_2.tm_out == 0)
    return;
  fl_485_2.tm_out = 0;
  clr_cntr_nat();
  clr_cntr_link();
  if ((PIND & DCD) == 0)
    send_err485(
        NUM_RS485_2, RS_NO_LINK, Appl_RS485_2.id_tek,
        Appl_RS485_2.dst_tek); // игнорировать передачу если нет GPRS канала
  fl_485_2.busy = 0;
}

void monitor_rec_rs485_2(void) {
  unsigned int i;

  if (fl_ip.act_ip_end != 1)
    return;
  if (fl_485_2.rec == 0)
    return;
  fl_485_2.rec = 0;

  clr_cntr_nat();
  clr_cntr_link();

  if (PIND & DCD) {
    fl_485_2.over = 0; // игнорировать передачу если нет GPRS канала
    Rs485_2.cnt_bt_rx_tx = 0;
    fl_485_2.busy = 0;
    return;
  }

  if (fl_485_2.over == 1) {

    fl_485_2.over = 0;
    send_err485(NUM_RS485_2, RS_OVER_BUF_RX, Appl_RS485_2.id_tek,
                Appl_RS485_2.dst_tek);
    Rs485_2.cnt_bt_rx_tx = 0;
    fl_485_2.busy = 0;
    return;
  }

  //!!!!!!!!!!!!!! здесь бы надо проверк на длину буфера для PPP
  if (Rs485_2.cnt_bt_rx_tx + 100 > VOL_TX_PPP) {
    send_err485(NUM_RS485_2, RS_OVER_BUF_PPP_TX, Appl_RS485_2.id_tek,
                Appl_RS485_2.dst_tek);
    Rs485_2.cnt_bt_rx_tx = 0;
    fl_485_2.busy = 0;
    return;
  }

  fl_ip.act_ip_end = 0;

  Obj_ppp_tx.prozr = FALSE;
  Obj_ppp_tx.version = VER1;                 // версия
  Obj_ppp_tx.type_pac = OTV;                 // тип пакета
  Obj_ppp_tx.num_src = num_self;             // номер отправителя
  Obj_ppp_tx.num_dst = Appl_RS485_2.dst_tek; // номер получателя
  Obj_ppp_tx.id_pac = Appl_RS485_2.id_tek;   // идентификатор пакета
  Obj_ppp_tx.p_opt = &buf_opt_tr[0];         // //указатель буфера опций
  Obj_ppp_tx.l_opt = 0;                      // длина буфера опций
  Obj_ppp_tx.kol_opt = 0;                    // количество опций

  buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
  buf_tx_232[TR_OP_DATA + C1_PORT] = PORT485_2;

  for (i = 0; i < Rs485_2.cnt_bt_rx_tx; i++)
    buf_tx_232[TR_OP_DATA + C1_DATA + i] = Rs485_2_buf_rx_tx[i];
  Obj_ppp_tx.l_data = 2 + Rs485_2.cnt_bt_rx_tx; // длина данных
  Obj_ppp_tx.p_data =
      &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных
  Rs485_2.cnt_bt_rx_tx = 0;

  form_buf_tx_ppp();
  UCSR0A = UCSR0A | TXC;
  UCSR0B = UCSR0B | TXEN;
  UCSR0B = UCSR0B | TXCIE;
  fl_485_2.busy = 0;
  if (check_cts() == 1)
    return;
  SVD2_1_ON;
  UDR0 = buf_tx_232[0];
  return;
}

void count_crc_dallas(unsigned char *p_beg, unsigned int kol,
                      unsigned char *p_crc) {
  unsigned int i;

  *p_crc = 0;
  for (i = 0; i < kol; i++)
    *p_crc = tab_crc[(*p_crc) ^ (*(p_beg + i))]; // проверка сrc
}

void monitor_rec_rs232_2(void) {

  union {
    unsigned char bytes[2];
    unsigned int word;
  } tempus;

  if (fl_232_2.rec == 0)
    return;
  fl_232_2.rec = 0;

  if (fl_232_2.over == 1) {

    fl_232_2.over = 0;
    reset_em_232_new();
    //!!!!!  fl_485_2.busy=0;
    return;
  }

  //  PORTB=PORTB ^ LED_OHRN_S;////proverka
  //  Rs232_2.p_data232=&Rs232_2_buf_rx_tx[0];
  //  send_em_232_2();
  // reset_em_232_new();

  if (Rs232_2.cnt_bt_rx_tx > (MAX_BUF_RS485_1 - 1)) {
    reset_em_232_new();
    return;
  }

  if (fl_232_2.busy == 1) {
    reset_em_232_new();
    return;
  }

  kamintel = 0;

  tempus.word = crc_m1(&Rs232_2_buf_rx_tx[0], Rs232_2.cnt_bt_rx_tx - 2, 0xffff);
  if ((Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx - 2] != tempus.bytes[1]) ||
      (Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx - 1] != tempus.bytes[0])) {

    if (Rs232_2_buf_rx_tx[1] == 1) // это к самому контроллеру
    {
      switch (Rs232_2_buf_rx_tx[0]) {
      case 18:
        Rs232_2_buf_rx_tx[0] = 18;
        Rs232_2_buf_rx_tx[1] = 1;
        Rs232_2_buf_rx_tx[2] = sv_tc.bytes[0];
        Rs232_2_buf_rx_tx[3] = sv_tc.bytes[1];
        if (PIND & PIN_AKB_SET)
          Rs232_2_buf_rx_tx[4] = 0x64;
        else
          Rs232_2_buf_rx_tx[4] = 0x74;
        Rs232_2_buf_rx_tx[5] = 0;

        *(unsigned long int *)&Rs232_2_buf_rx_tx[6] = 0;
        *(unsigned long int *)&Rs232_2_buf_rx_tx[10] = 0;
        *(unsigned long int *)&Rs232_2_buf_rx_tx[14] = 0;
        *(unsigned long int *)&Rs232_2_buf_rx_tx[18] = 0;

        Rs232_2.cnt_bt_rx_tx = 22;
        Rs232_2.p_data232 = &Rs232_2_buf_rx_tx[0];
        fl_232_2.busy = 1;
        send_em_232_2();
        return;
      }

      reset_em_232_new();
      return;
    }

    count_crc_dallas(&Rs232_2_buf_rx_tx[0], Rs232_2.cnt_bt_rx_tx,
                     &Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx]);
    Rs232_2.cnt_bt_rx_tx++;
    kamintel = 1;
  }

  fl_232_2.busy = 1;
  fl_232_2.buffed = 1;

  return;
}

void monitor_rst(void) {
  unsigned char buf[9];

  if ((MCUCSR & 0x01) == 0x01) {
    write_log_info(ST_PIT_ON, RESET_POWER);

    if (Appl_seq_des == 1) {
      MCUCSR = 0;
      return;
    } // блокировка

    buf[0] = COOL_RESET;
    *(unsigned int *)&buf[1] = L_COOL_RESET;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0],
                     L_COOL_RESET + 3, sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }

  } else {

    if ((MCUCSR & 0x02) == 0x02)
      write_log_info(ST_PIT_ON, RESET_EXT);
    if ((MCUCSR & 0x04) == 0x04)
      write_log_info(ST_PIT_ON, RESET_BROWN);
    if ((MCUCSR & 0x08) == 0x08)
      write_log_info(ST_PIT_ON, RESET_WDR);

    if (Appl_seq_des == 1) {
      MCUCSR = 0;
      return;
    } // блокировка

    buf[0] = HOT_RESET;
    *(unsigned int *)&buf[1] = L_HOT_RESET;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    if ((MCUCSR & 0x02) == 0x02)
      buf[7] = RST_EXT;
    if ((MCUCSR & 0x04) == 0x04)
      buf[7] = RST_BROWN;
    if ((MCUCSR & 0x08) == 0x08)
      buf[7] = RST_WDR;
    buf[8] = 0;
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0],
                     L_HOT_RESET + 3, sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }
  }

  MCUCSR = 0;
}

void monitor_over_net(void) {
  unsigned char buf[9];

  if (Appl_seq_des == 1)
    return; // блокировка опроса охранки
  if (count_block_net != 0)
    return; // блокировка монитора
  if (((PIND & PIN_AKB_SET) && (byte_state_net == YES_NET)) ||
      (((PIND & PIN_AKB_SET) == 0) && (byte_state_net == NO_NET))) {
    if (byte_state_net == YES_NET)
      byte_state_net = NO_NET;
    else
      byte_state_net = YES_NET;
    count_block_net = 3;
    buf[0] = AKB_SET;
    *(unsigned int *)&buf[1] = L_AKB_SET;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    buf[7] = byte_state_net;
    buf[8] = 0;
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_AKB_SET + 3,
                     sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }
  }
}

void monitor_beg_state_seq(unsigned char zad) {
  // здесь должна быть пауза чтобы определились текущие ТС
  unsigned char buf[10];
  unsigned char i = 0;

  if (zad == 0) {
    while (i < 200) // 200 мсек
    {
      __watchdog_reset();
      delay(16000); // 1mcek
      i++;
    }
  }

  sv_tc.bytes[0] =
      ~((real_tc.arr_tc[0] & 0xf) | ((real_tc.arr_tc[1] << 4) & 0xf0));
  sv_tc.bytes[1] =
      ~((real_tc.arr_tc[2] & 0xf) | ((real_tc.arr_tc[3] << 4) & 0xf0));

  sv_tc_old.word = sv_tc.word;

  if (Appl_seq_des == 1)
    return; // блокировка опроса охранки

  if (sv_tc.bytes[0] & 0x01) {
    Appl_seq.event = EV_TC_NORMA;
    Appl_seq.state = ST_VZAT;
    state_seq = VZAT;
    buf[0] = VZAT;
    *(unsigned int *)&buf[1] = L_VZAT;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    buf[7] = sv_tc.bytes[0] & 0x01;
    buf[8] = 0;
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_VZAT + 3,
                     sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }
  } else {
    Appl_seq.event = EV_TC_ALARM;
    Appl_seq.tm_vzat = Appl_seq.vol_tm_vzat;
    Appl_seq.state = ST_ALARM;
    state_seq = ALARM;
    buf[0] = ALARM;
    *(unsigned int *)&buf[1] = L_ALARM;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    buf[7] = sv_tc.bytes[0] & 0x01;
    buf[8] = 0;
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_ALARM + 3,
                     sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }
  }

  if (PIND & PIN_AKB_SET)
    byte_state_net = NO_NET;
  else
    byte_state_net = YES_NET;
  count_block_net = 3;
  buf[0] = AKB_SET;
  *(unsigned int *)&buf[1] = L_AKB_SET;
  *(unsigned long int *)&buf[3] = burst_ds_r();
  buf[7] = byte_state_net;
  buf[8] = 0;
  if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                   &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_AKB_SET + 3,
                   sizeof(Appl_seq_buf)) == 1) {
    fl_appl_seq1.over_buf = 1;
  }
}

void led_alarm(void) {
  if (glutch < 1) {
    led_sost_s_off;
  }
  if (glutch == 1) {
    led_sost_s_on;
  }
  if (glutch > 1)
    glutch = 0;
}

void led_vzatie(void) {
  if (glutch < 2) {
    led_sost_s_off;
  }
  if (glutch > 2) {
    led_sost_s_on;
  }
}

void monitor_change_state(void) {

  unsigned char buf[10];

  // здесь
  sv_tc.bytes[0] =
      ~((real_tc.arr_tc[0] & 0xf) | ((real_tc.arr_tc[1] << 4) & 0xf0));
  sv_tc.bytes[1] =
      ~((real_tc.arr_tc[2] & 0xf) | ((real_tc.arr_tc[3] << 4) & 0xf0));

  if (Appl_seq_des == 1) {
    led_sost_s_off;
    led_ohrn_s_off;
    return;
  } // блокировка опроса охранки

  if (fl_appl_seq1.send_state == 1) {
    fl_appl_seq1.send_state = 0;
    buf[0] = SEQ_STATE;
    *(unsigned int *)&buf[1] = L_STATE;
    *(unsigned long int *)&buf[3] = burst_ds_r();
    buf[7] = state_seq;
    buf[8] = sv_tc.bytes[0] & 0x01;
    // buf[9]=0;
    buf[9] = byte_state_net & 0x01;
    if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                     &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_STATE + 3,
                     sizeof(Appl_seq_buf)) == 1) {
      fl_appl_seq1.over_buf = 1;
    }
  }

  if (sv_tc.bytes[0] & 0x01) {
    led_ohrn_s_off;
  } else {
    led_ohrn_s_on;
  }

  if ((sv_tc.bytes[0] & 0x01) != (sv_tc_old.bytes[0] & 0x01)) {
    if (sv_tc.bytes[0] & 0x01)
      Appl_seq.event = EV_TC_NORMA;
    else
      Appl_seq.event = EV_TC_ALARM;
    sv_tc_old.word = sv_tc.word;
  }

  switch (Appl_seq.state) {
  case ST_ALARM:
    led_alarm();
    switch (Appl_seq.event) {
    case (EV_TC_NORMA):
      Appl_seq.tm_vzat = Appl_seq.vol_tm_vzat;
      Appl_seq.state = ST_VZATIE;
      break;
    case (EV_TM_OUT):
      Appl_seq.tm_vzat = Appl_seq.vol_tm_vzat;
      break;
    default:
      break;
    }
    break;
  case ST_VZAT:
    led_sost_s_on;

    switch (Appl_seq.event) {
    case (EV_TC_ALARM):

      Appl_seq.tm_vzat = Appl_seq.vol_tm_vzat;
      Appl_seq.state = ST_ALARM;
      state_seq = ALARM;
      buf[0] = ALARM;
      *(unsigned int *)&buf[1] = L_ALARM;
      *(unsigned long int *)&buf[3] = burst_ds_r();
      buf[7] = sv_tc.bytes[0] & 0x01;
      buf[8] = 0;
      if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                       &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_ALARM + 3,
                       sizeof(Appl_seq_buf)) == 1) {
        fl_appl_seq1.over_buf = 1;
      }
      break;
    default:
      break;
    }
    break;
  case ST_VZATIE:
    led_vzatie();
    switch (Appl_seq.event) {
    case (EV_TC_ALARM):
      Appl_seq.tm_vzat = Appl_seq.vol_tm_vzat;
      Appl_seq.state = ST_ALARM;
      break;
    case (EV_TM_OUT):

      Appl_seq.state = ST_VZAT;
      state_seq = VZAT;
      buf[0] = VZAT;
      *(unsigned int *)&buf[1] = L_VZAT;
      *(unsigned long int *)&buf[3] = burst_ds_r();
      buf[7] = sv_tc.bytes[0] & 0x01;
      buf[8] = 0;
      if (write_to_buf(&Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out,
                       &Appl_seq.p_out_kv, &Appl_seq.crc, &buf[0], L_VZAT + 3,
                       sizeof(Appl_seq_buf)) == 1) {
        fl_appl_seq1.over_buf = 1;
      }
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }
}

void Application_SEQ_tx() {

  if (Appl_seq_des == 1)
    return; // блокировка опроса охранки

  if (Appl_seq.cntr_cl == TRUE)
    return;

  if ((fl_lcp.act_lcp_end != 1) && (fl_pap.act_pap_end != 1) ||
      (fl_ipcp.act_ipcp_end != 1) || (fl_ip.act_ip_end != 1))
    return;
  // проверять здесь готово ли для отправки буфер PPP

  Obj_ppp_tx.l_data = read_from_buf(
      &Appl_seq_buf[0], &Appl_seq.p_in, &Appl_seq.p_out, &Appl_seq.p_out_kv,
      &Appl_seq.crc, &buf_tx_232[TR_OP_DATA + C1_DATA], sizeof(Appl_seq_buf));
  if (Obj_ppp_tx.l_data != 0) {
    // приращение идентификатора
    if (Appl_seq.l_data != Obj_ppp_tx.l_data) {
      if (fl_appl_seq.en_povtor == 0)
        Appl_seq.id++;
    } else
      return;

    Appl_seq.cnt_waits = Control.vol_waits;
    fl_appl_seq.kv_waits = 1;
    fl_appl_seq.en_povtor = 0;
    Appl_seq.l_data = Obj_ppp_tx.l_data;

    // формирование на передачу в буффер
    clr_cntr_nat();

    // clr_cntr_link_2(); // proverka

    fl_ip.act_ip_end = 0;

    Obj_ppp_tx.prozr = FALSE;
    Obj_ppp_tx.version = VER1;     // версия
    Obj_ppp_tx.type_pac = SOOB;    // тип пакета
    Obj_ppp_tx.num_src = num_self; // номер отправителя
    Obj_ppp_tx.num_dst =
        num_seq_cl; // номер получателя // потом установить подумать не
                    // отошлется ли это другим котнроллерам
    Obj_ppp_tx.id_pac = Appl_seq.id;   // идентификатор пакета
    Obj_ppp_tx.p_opt = &buf_opt_tr[0]; // //указатель буфера опций
    Obj_ppp_tx.l_opt = 0;              // длина буфера опций
    Obj_ppp_tx.kol_opt = 0;            // количество опций

    buf_tx_232[TR_OP_DATA + C1_PROT] = PROT_C1;
    buf_tx_232[TR_OP_DATA + C1_PORT] = PORT_SEQ;

    Obj_ppp_tx.l_data = 2 + Obj_ppp_tx.l_data; // длина данных
    Obj_ppp_tx.p_data =
        &buf_tx_232[TR_OP_DATA + C1_PROT]; // указатель буфера данных

    form_buf_tx_ppp();

    UCSR0A = UCSR0A | TXC;
    UCSR0B = UCSR0B | TXEN;
    UCSR0B = UCSR0B | TXCIE;
    if (check_cts() == 1)
      return;
    SVD2_1_ON;
    UDR0 = buf_tx_232[0];

    return;
  }
}

void load_rw_pdp(void) {
  char buf[70];
  unsigned char i, j, temp;
  unsigned int kol;

  buf[69] = 0;

  if ((MCUCSR & 0x01) != 0x01)
    return;
  if (e_rw_pdp != 0)
    return;
  RdFromFleshToArr(ABUF_C_GPRS, &buf[0], L_C_GPRS_MAX - 2);
  if (buf[0] != 0) {
    j = 0;
    kol = 0;

    temp = strlen(&buf[j]);
    if (temp > 35) {
      e_rw_pdp = 0xff;
      return;
    }
    buf_tx_232[j] = temp;
    for (i = 0; i < temp; i++) {
      buf_tx_232[j + 1] = buf[j];
      j++;
    }
    kol = kol + temp;

    j++;
    temp = strlen(&buf[j]);
    if (temp > 15) {
      e_rw_pdp = 0xff;
      return;
    }
    buf_tx_232[j] = temp;
    for (i = 0; i < temp; i++) {
      buf_tx_232[j + 1] = buf[j];
      j++;
    }
    kol = kol + temp;

    j++;
    temp = strlen(&buf[j]);
    if (temp > 15) {
      e_rw_pdp = 0xff;
      return;
    }
    buf_tx_232[j] = temp;
    for (i = 0; i < temp; i++) {
      buf_tx_232[j + 1] = buf[j];
      j++;
    }
    kol = kol + temp;

    kol = kol + 3;
    if (kol > (L_C_GPRS_MAX - 2)) {
      e_rw_pdp = 0xff;
      return;
    }

    *(unsigned int *)&buf_tx_232[L_C_GPRS_MAX - 2] =
        crc_m1(&buf_tx_232[0], L_C_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C_GPRS, &buf_tx_232[0], L_C_GPRS_MAX, 0, 0);
  }
  e_rw_pdp = 0xff;
}

void load_par_first(void) {
  if (e_first_on == 0xff) {
    __watchdog_reset();
    WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);
    __watchdog_reset();
    funct_ds(0, 0);
    e_first_on = 0;
  }
}

/*
void load_par_bl_timer(void)
{
//блок таймеры и счетчики
  Control.vol_link=e_cntr_link;//контроль канала
  Control.vol_waits=e_cntr_link_waits;//время ожидания квмтка на контроль канала
  Control.vol_nat=e_cntr_nat;//контроль канала
  Control.vol_try=e_cntr_vol_try; // rjkbxtcndj попыток получить квиток
  Appl_seq.vol_tm_vzat=e_vol_tm_vzat;
  Appl_seq.vol_cntr_cl=DEF_TM_CNTR_CL;
}
*/

void load_par(void) {
  load_par_first();
  // load_par_bl_timer();
}

void start_func_stm_on(void) {
  command_AT = TRUE; // включен коммандный режим
  set_rts_232;
  modem_on;
  state_modem = STM_ON;
  cnt_stm_tm1 = DEF_TM_STM_ON;
  event_modem = EVM_NONE;
  write_log_info(ST_MODEM, MDM_ON);
}

void start_func_stm_off(void) {
  st_gprs_no();
  command_AT = TRUE; // включен коммандный режим
  modem_off;
  state_modem = STM_OFF;
  cnt_stm_tm1 = DEF_TM_STM_OFF;
  event_modem = EVM_NONE;
  write_log_info(ST_MODEM, MDM_OFF);
}

void clr_var_ppp(void) {
  UCSR0B = UCSR0B & ~TXEN;
  UCSR0B = UCSR0B & ~TXCIE;

  count_tx_ppp = 0;

  fl_reg3.add_byte = 0;
  fl_lcp.act_lcp_end = 1;
  fl_pap.act_pap_end = 1;
  fl_ipcp.act_ipcp_end = 1;
  fl_ip.act_ip_end = 1;

  cnt_rst_lcp = 0;
  cnt_rst_ipcp = 0;
  cnt_rst_pap = 0;

  cnt_lcp_tm_out = 0;
  cnt_ipcp_tm_out = 0;
  cnt_pap_tm_out = 0;
  cnt_ip_tm_cntr = 0;

  rcvd_protokol = 0;

  i_scr_lcp = 0;
  i_scan_lcp = 0;
  i_scj_lcp = 0;
  i_str_lcp = 0;
  i_sta_lcp = 0;

  i_scr_ipcp = 0;
  i_scan_ipcp = 0;
  i_scj_ipcp = 0;
  i_str_ipcp = 0;
  i_sta_ipcp = 0;

  i_scr_pap = 0;

  ip_self[0] = 0;
  ip_self[1] = 0;
  ip_self[2] = 0;
  ip_self[3] = 0;

  fl_lcp.lcp_tm_out_en = 0;
  fl_lcp.t0_pl = 0;
  fl_lcp.t0_mi = 0;

  fl_ipcp.ipcp_tm_out_en = 0;
  fl_ipcp.t0_pl = 0;
  fl_ipcp.t0_mi = 0;

  fl_ip.ip_tm_cntr_en = 0;
  fl_ip.ip_tm_nat_en = 0;

  fl_pap.pap_tm_out_en = 0;
  fl_pap.t0_pl = 0;
  fl_pap.t0_mi = 0;
  fl_pap.up = 0;

  fl_lcp1.up = 0;
  fl_lcp1.down = 0;

  fl_ipcp1.up = 0;
  fl_ipcp1.down = 0;

  fl_lcp2.nac_rej = 0;

  fl_ipcp2.nac_rej = 0;
  fl_cts_232.on = 0;
  fl_cts_232_ignor = FALSE;

  Control.cnt_link = 0;
  Control.cnt_nat = 0;
  Control.link_no = FALSE;
  Control.link_waits = FALSE;
  Control.cnt_link = 0;
  Control.cnt_nat = 0;
  Control.cnt_try = 0;

  Appl_RS485_1.fl_data_buf = FALSE;
  Appl_RS485_2.fl_data_buf = FALSE;
  Appl_RS485_1.id_tek = 0;
  Appl_RS485_2.id_tek = 0;
  fl_tx485.mon1 = 0;

  Appl_seq.cnt_waits = 0;
  Appl_seq.cnt_try = 0;
  fl_appl_seq.en_tx = 0;
  fl_appl_seq.kv_waits = 0;
  fl_appl_seq.en_povtor = 0;
  Appl_seq.l_data = 0;

  Appl_seq.cntr_cl = FALSE;
  Appl_seq.en_cntr_cl = FALSE;
  Appl_seq.cnt_cntr_cl = 0;

  cnt_cts_off = 0;
}

void clr_var_ppp_rx(void) {
  UCSR0B = UCSR0B & ~RXEN;
  UCSR0B = UCSR0B & ~RXCIE;
  fl_rx_ppp.switcher = 0;
  count_rx_232 = 0;
  kol_rx_232 = 0;
  fl_rx_ppp.fl_7e = 0;
  fl_rx_ppp.fl_7d = 0;
  Buf1_rx_ppp.ln_data = 0;
  Buf1_rx_ppp.rec = FALSE;
  Buf1_rx_ppp.busy = FALSE;
  Buf2_rx_ppp.ln_data = 0;
  Buf2_rx_ppp.rec = FALSE;
  Buf2_rx_ppp.busy = FALSE;
}

unsigned int crc_ozu(void)

{
  unsigned char uchCRCHi;
  unsigned char uchCRCLo;
  unsigned char temp;

  uchCRCLo = 0xff;
  uchCRCHi = 0xff;

  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ ip_ls[0]];
  uchCRCLo = auchCRCLo[uchCRCHi ^ ip_ls[0]];
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ ip_ls[1]];
  uchCRCLo = auchCRCLo[uchCRCHi ^ ip_ls[1]];
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ ip_ls[2]];
  uchCRCLo = auchCRCLo[uchCRCHi ^ ip_ls[2]];
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ ip_ls[3]];
  uchCRCLo = auchCRCLo[uchCRCHi ^ ip_ls[3]];

  temp = port_udp;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = port_udp >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = num_self;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = num_self >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Control.vol_nat;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Control.vol_nat >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Control.vol_link;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Control.vol_link >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Control.vol_waits;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Control.vol_waits >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Control.vol_try;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Control.vol_try >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ Appl_seq_des];
  uchCRCLo = auchCRCLo[uchCRCHi ^ Appl_seq_des];

  temp = num_seq_cl;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = num_seq_cl >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Appl_seq.vol_tm_vzat;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Appl_seq.vol_tm_vzat >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  temp = Appl_seq.vol_cntr_cl;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];
  temp = Appl_seq.vol_cntr_cl >> 8;
  uchCRCHi = uchCRCLo ^ auchCRCHi[uchCRCHi ^ temp];
  uchCRCLo = auchCRCLo[uchCRCHi ^ temp];

  return (uchCRCHi << 8 | uchCRCLo);
}

void load_par_from_memory(void) {
  unsigned char buf[20];

  RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
  if (*(unsigned int *)&buf[OFS_IP_PAR_CRC] !=
      crc_m1(&buf[0], L_IP_PAR - 2, 0xffff)) {

    ip_ls[0] = DEF_IP_SRV_0;
    ip_ls[1] = DEF_IP_SRV_1;
    ip_ls[2] = DEF_IP_SRV_2;
    ip_ls[3] = DEF_IP_SRV_3;
    num_self = DEF_NUM_SELF;
    port_udp = DEF_UDP_PORT;
  } else {

    ip_ls[0] = buf[OFS_IP];
    ip_ls[1] = buf[OFS_IP + 1];
    ip_ls[2] = buf[OFS_IP + 2];
    ip_ls[3] = buf[OFS_IP + 3];
    port_udp = *(unsigned int *)&buf[OFS_PORT];
    num_self = *(unsigned int *)&buf[OFS_NUM];
  }

  RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
  if (*(unsigned int *)&buf[OFS_C_PAR_CRC] !=
      crc_m1(&buf[0], L_C_PAR - 2, 0xffff)) {

    Control.vol_nat = DEF_CNTR_NAT;
    Control.vol_link = DEF_CNTR_LINK;
    Control.vol_waits = DEF_CNTR_LINK_WAITS;
    Control.vol_try = DEF_TM_CNTR_LINK / Control.vol_waits;
  } else {

    Control.vol_nat = *(unsigned int *)&buf[OFS_NAT];
    Control.vol_link = *(unsigned int *)&buf[OFS_CCH];
    Control.vol_waits = DEF_CNTR_LINK_WAITS;
    Control.vol_try = *(unsigned int *)&buf[OFS_TM_CH] / Control.vol_waits;
  }

  RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
  if (*(unsigned int *)&buf[OFS_SEQ_PAR_CRC] !=
      crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff)) {

    Appl_seq_des = ~DEF_DES_SEQ;
    num_seq_cl = DEF_NUM_DST_SEQ;
    Appl_seq.vol_tm_vzat = DEF_VOL_TM_VZAT;
    Appl_seq.vol_cntr_cl = DEF_TM_CNTR_CL;
  } else {

    Appl_seq_des = buf[OFS_DES_SEQ];
    num_seq_cl = *(unsigned int *)&buf[OFS_NUM_CL];
    Appl_seq.vol_tm_vzat = *(unsigned int *)&buf[OFS_TM_VZ];
    Appl_seq.vol_cntr_cl = *(unsigned int *)&buf[OFS_TM_CL];
  }
}

void load_en_gprs(void) {
  unsigned char buf[12];
  RdFromFleshToArr(A_PORT_GPRS, &buf[0], L_PORT_GPRS);
  if (*(unsigned int *)&buf[OFS_PORT_GPRS_CRC] !=
      crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff)) {

    UBRR0H = R9600_H;
    UBRR0L = R9600_L;
    start_func_stm_on();

  } else {

    if (*(unsigned long int *)&buf[OFS_GPRS_S] == 115200) {
      UBRR0H = R115200_H;
      UBRR0L = R115200_L;
    } else {
      UBRR0H = R9600_H;
      UBRR0L = R9600_L;
    }
    if (buf[OFS_GPRS_DES] != 1)
      start_func_stm_on();
  }
}

void start_func_stm_scp(void) {
  clr_var_ppp_rx();

  UCSR0B = UCSR0B & ~RXEN;
  UCSR0B = UCSR0B & ~RXCIE;

  //******************************************************************
  UCSR0B = UCSR0B & ~TXEN;
  UCSR0A = UCSR0A | TXC;

  command_AT = FALSE; // выключен коммандный режим

  clr_var_ppp();

  count_tx_ppp = 0;
  // для активации установки соединения PPP
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  fl_lcp1.up = 1;
  fl_lcp1.down = 0;
  // конфигурация соединения

  // конфигурация канала

  layer_PPP = LAYER_LCP;
  LCP_state = START_ST;

  IPCP_event = NO_EVENT;
  LCP_event = NO_EVENT;
  PAP_event = NO_EVENT;

  uk_in_act_IPCP = 0;
  uk_out_act_IPCP = 0;

  uk_in_act_LCP = 0;
  uk_out_act_LCP = 0;

  cnt_lcp_tm_out = VOL_LCP_TM_OUT;
  fl_lcp.act_lcp_end = 1; // действия все закончены
  fl_pap.act_pap_end = 1;
  fl_ipcp.act_ipcp_end = 1;
  fl_ip.act_ip_end = 1;
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  UCSR0B = UCSR0B | RXEN;
  temp = UDR0;
  UCSR0B = UCSR0B | RXCIE;

  cnt_stm_tm1 = DEF_TM_STM_SCP;
}

void check_wdt(void) {
  fl_wdt.clr_wdt = 0;

  if (fl_wdt.fl_out == 1) {

    if ((fl_wdt.from_timer0 != 1) || (fl_wdt.fl_main != 1) ||
        (fl_wdt.from_timer3 != 1) || (fl_wdt.from_timer2 != 1)) {
      write_log_info(ST_ERROR, ERR1);
      lock_it();
    }

    if (prov_ozu != crc_ozu()) {
      write_log_info(ST_ERROR, ERR2);
      lock_it();
    }

    fl_wdt.from_timer0 = 0;
    fl_wdt.from_timer3 = 0;
    fl_wdt.from_timer2 = 0;
    fl_wdt.fl_main = 0;
    fl_wdt.fl_out = 0;
  }
  __watchdog_reset();
}

void delay_on(void) {
  unsigned char delay_count;

  delay_count = 18;

  PORTA = PORTA | SVD1_1;
  PORTA = PORTA & ~SVD1_2;
  PORTA = PORTA & ~SVD2_1;
  PORTA = PORTA | SVD2_2;

  PORTB = PORTB | 0x04;
  led_sost_s_on;
  led_ohrn_s_on;

  do {
    __watchdog_reset();
    delay(60000);
    delay(60000);
    delay(60000);
    delay(60000);
    delay(60000);
    __watchdog_reset();
    PORTA = PORTA ^ SVD1_1;
    PORTA = PORTA ^ SVD1_2;
    PORTA = PORTA ^ SVD2_1;
    PORTA = PORTA ^ SVD2_2;
    PORTB = PORTB ^ LED_SOST_S;
    PORTB = PORTB ^ LED_OHRN_S;
    delay_count--;
  } while (delay_count != 0);

  led_sost_s_off;
  led_ohrn_s_off;
  PORTB = PORTB & ~0x04;
  SVD1_1_OFF;
  SVD1_2_OFF;
  SVD2_1_OFF;
  SVD2_2_OFF;
}

void monitor_ppp(void) {

  // инициализация ног
  DDRG = DDRG | ON_LUFKIN; // на выход
  lufkin_port_off;

  DDRA = DDRA | ON_OFF; // включение выключение модема на выход

  PORTD = PORTD | DCD; // DCD  на вход
  // DDRC=DDRC | DTR;    //DTR  на выход
  DDRB = DDRB | LED_SOST_S; // led1
  DDRB = DDRB | LED_OHRN_S; // led2
  DDRB = DDRB | RTS_232;    // rts_232
  DDRA = DDRA | RTS_485_1;  // rts_485
  PORTB = PORTB | CTS_232;  // CTS на вход

  PORTA = PORTA | DSR; // DSR на вход

  PORTD = PORTD | PIN_AKB_SET; // акб-сеть на вход

  DDRD = DDRD | RTS_485_2; // rts_485_2 выход
  DDRE = DDRE | OUT_BIT485_2;
  PORTE = PORTE | OUT_BIT485_2;
  PORTD = PORTD | IN_BIT485_2; // НА вход

  DDRE = DDRE | OUT_BIT232_2;
  PORTE = PORTE | OUT_BIT232_2;
  PORTE = PORTE | IN_BIT232_2; // НА вход

  // конфигурация входов по ТС
  PORTC = PORTC | 0x1f;
  DDRD = DDRC | 0x11;
  PORTG = PORTG | 0x02;
  DDRG = DDRG | 0x01;
  DDRD = DDRD | 0x80;
  // конфигурация входов по ТС

  DDRA = DDRA | SVD1_1;
  DDRA = DDRA | SVD1_2;
  DDRA = DDRA | SVD2_1;
  DDRA = DDRA | SVD2_2;

  DDRE = DDRE | SDA;
  DDRE = DDRE | SCL;

  clr_rts_232;
  modem_off;

  WDTCR = 0x18;
  WDTCR = 0x0f;
  __watchdog_reset();

  if (check_memory_map() == 1)
    vosstan_memory();

  __watchdog_reset();
  monitor_terminal(); // проверка терминала

  __watchdog_reset();

  delay_on();

  __watchdog_reset();
  serch_point_log();
  __watchdog_reset();

  //  monitor_rst(); // монитор записи состояний ресета

  load_rw_pdp();
  /*
   load_par_from_memory();

   monitor_rst();

   load_en_gprs();
   load_par();
   */

  load_par();
  load_par_from_memory();
  monitor_rst();
  load_en_gprs();

  prov_ozu = crc_ozu();

  //  monitor_rst(); // монитор записи состояний ресета

  // RS-232
  // UBRR0H=R115200_H;
  // UBRR=R115200_L;

  UBRR0H = R9600_H;
  UBRR0L = R9600_L;

  UCSR0B = UCSR0B | RXEN;
  UCSR0B = UCSR0B | TXEN;
  temp = UDR0;
  UCSR0B = UCSR0B | RXCIE;
  UCSR0A = UCSR0A | TXC;

  UCSR1B = UCSR1B | TXEN;

  // для эмуляторов
  // задать для эмулированного порта
  // Emul_232.bod=6667; //2400
  Emul_232.bod = 3333; // 4800

  // Emul_232.bod=3233; //4800

  // Emul_232.bod=1667; //9600
  //   Emul_232.bod=1600; //9600

  TCCR1B = 0x01;
  EICRA = 0x08; // int1
  EICRB = 0x08; // int5

  OCR0 = 170; // 340mks
  TCCR0 = 0x0b;
  TCNT0 = 0;
  TIMSK = TIMSK | 0x02;

  OCR3AH = 0x3d; // 15625
  OCR3AL = 0x09;
  TCCR3B = 0x0d; // преск 1024            // 1 сек
  ETIMSK = ETIMSK | 0x10;

  OCR2 = 64; // 1,024 мсек
  TCCR2 = 0x0c;
  TIMSK = TIMSK | 0x80;

  Rs232_2.vol_tm_rx_out = 10;
  reset_em_232_new(); // 'эмулированный порт на прием

  __watchdog_reset();
  __enable_interrupt();

  temp = 0;

  // start_func_stm_on();
  monitor_beg_state_seq(0);

  fl_wdt.fl_main = 1;

  while (1) {

    if (fl_wdt.clr_wdt == 1)
      check_wdt();
    fl_wdt.fl_main = 1;

    monitor_change_state(); // записи состоянии охраны
    monitor_over_net();     // записи состоянии сети

    monitor_rec_rs232_2(); // радиомодем-штатный RS485
    monitor2_tx_rs485_1();
    monitor2_rec_rs485_1();
    monitor2_tm_rs485_1();

    switch (state_modem) {
    case STM_ON:

      switch (event_modem) {
      case EVM_TM1:
        SVD2_2_OFF;
        state_modem = STM_SI;
        init_scen_stm_si();
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим
        break;
      }
      break;
    case STM_SS:
      // выполнение процедур - сценарий
      at_com_tx(cnt_com);
      at_com_rx(cnt_com);

      switch (event_modem) {
      case EVM_AT_OK:

        write_log_info(ST_MODEM, MDM_SPEED);
        led_ohrn_s_on;
        while (1)
          ; // proverka
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим
        break;
      case EVM_AT_ERR:
        start_func_stm_off();
        break;
      }
      break;
    case STM_SI:
      // выполнение процедур - сценарий
      at_com_tx(cnt_com);
      at_com_rx(cnt_com);
      at_com_scen_init(&cnt_com, &rep);

      switch (event_modem) {
      case EVM_AT_OK:
        write_log_info(ST_MODEM, MDM_INIT);
        state_modem = STM_SC;
        init_scen_stm_sc();
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим
        break;
      case EVM_AT_ERR:
        start_func_stm_off();

        break;
      }
      break;
    case STM_SC:
      // выполнение процедур - сценарий

      at_com_tx(cnt_com);
      at_com_rx(cnt_com);
      at_com_scen_stm_sc(&cnt_com, &rep);
      switch (event_modem) {
      case EVM_AT_OK:
        write_log_info(ST_MODEM, MDM_CON);
        state_modem = STM_SCP;
        start_func_stm_scp();
        event_modem = EVM_NONE;

        break;
      case EVM_AT_ERR:
        start_func_stm_off();
        break;
      }
      break;
    case STM_SCP:
      // выполнение процедур - сценарий
      if (layer_PPP != LAYER_HW) {
        monitor_event_LCP();
        monitor_act_LCP();
      }

      if (layer_PPP == LAYER_PAP) {
        monitor_event_PAP();
        monitor_act_PAP();
      }

      if ((layer_PPP != LAYER_HW) && (layer_PPP != LAYER_PAP)) {
        monitor_event_IPCP();
        monitor_act_IPCP();
      }

      analiz_ppp_rx();
      // if (layer_PPP!=LAYER_IP) analiz_ppp_rx();

      /*

      switch(event_modem)
             {
             case EVM_TM1:
                           event_modem=EVM_NONE;
             break;

             break;
             case EVM_PPP_ERR:
                           event_modem=EVM_NONE;
             break;
             case EVM_DCD_ERR:
                           event_modem=EVM_NONE;
             break;
             }

             */
      if (PIND & DCD)
        event_modem = EVM_DCD_ERR;

      switch (event_modem) {
      case EVM_PPP_OK:
        write_log_info(ST_SET_PPP, SET_PPP_OK);
        st_ppp_connect();
        cnt_stm_tm1 = 0;
        state_modem = STM_PPP;
        event_modem = EVM_NONE;
        command_AT = FALSE; // выключен коммандный режим
        break;
      case EVM_TM1:
        write_log_info(ST_SET_PPP, SET_PPP_TM);
        goto err_set_ppp;
      case EVM_PPP_ERR:
        write_log_info(ST_SET_PPP, SET_PPP_RJ);
        goto err_set_ppp;
      case EVM_DCD_ERR:
        write_log_info(ST_SET_PPP, SET_PPP_DCD);
        goto err_set_ppp;
      case EVM_CTS_ERR:
        write_log_info(ST_SET_PPP, SET_PPP_CTS);
      err_set_ppp:

        st_gprs_att();
        cnt_cts_off = 0;
        fl_cts_232_ignor = TRUE;
        state_modem = STM_RDC;
        clr_var_ppp_rx(); // обнулить все что касается приему по GPRS и
                          // запретить прием
        event_modem = EVM_NONE;
        command_AT = FALSE; // выключен коммандный режим
        break;
      }

      break;
    case STM_PPP:

      /*
       if (layer_PPP!=LAYER_HW)

        {
          monitor_event_LCP();
          monitor_act_LCP();
         }

        if (layer_PPP==LAYER_PAP)
            {
             monitor_event_PAP();
             monitor_act_PAP();
             }


        if ((layer_PPP!=LAYER_HW)&&(layer_PPP!=LAYER_PAP))
            {
              monitor_event_IPCP();
               monitor_act_IPCP();
            }


        if (layer_PPP!=LAYER_IP) analiz_ppp_rx();
*/

      // if (layer_PPP==LAYER_IP)
      //     {
      analiz_ppp_rx();

      monitor1_tx_rs485_1(); // GPRS-штатный RS485
      monitor1_rec_rs485_1();
      monitor1_tm_rs485_1();

      monitor_tx_rs485_2(); // GPRS-эмулированный RS485
      monitor_rec_rs485_2();
      monitor_tm_rs485_2();

      //  monitor_change_state(); // записи состоянии охраны
      Application_SEQ_tx(); // монитор передачи сообщений охраны
                            // monitor_act_IP();

      monitor_cntr_ch();
      monitor_cntr_cl_seq();
      monitor_cntr_nat();
      //  }

      if (PIND & DCD) {
        // event_modem=EVM_DCD_ERR;
        if ((fl_tx485.mon1 == 0) && (fl_485_2.busy == 0) &&
            (fl_ip.act_ip_end == 1)) {
          write_log_info(ST_PPP, PPP_DCD);
          st_gprs_att();
          cnt_cts_off = 0;
          // fl_cts_232_ignor=TRUE;
          state_modem = STM_RDC;
          clr_var_ppp_rx(); // обнулить все что касается приему по GPRS и
                            // запретить прием
          command_AT = FALSE; // выключен коммандный режим
          // fl_cts_232_ignor=FALSE;
          event_modem = EVM_RDC;
        }
      }

      switch (event_modem) {

      case EVM_PPP_ERR:
        write_log_info(ST_PPP, PPP_RJ);
        goto err_ppp;

        // case EVM_DCD_ERR:
        //                  write_log_info(ST_PPP,PPP_DCD);
        //                  goto err_ppp;

      case EVM_MS_LMT:
        write_log_info(ST_PPP, PPP_LS);
        goto err_ppp;
      case EVM_CTS_ERR:
        write_log_info(ST_PPP, PPP_CTS);
      err_ppp:
        st_gprs_att();
        cnt_cts_off = 0;
        fl_cts_232_ignor = TRUE;
        state_modem = STM_RDC;
        clr_var_ppp_rx(); // обнулить все что касается приему по GPRS и
                          // запретить прием
        event_modem = EVM_NONE;
        command_AT = FALSE; // выключен коммандный режим
        break;
      }
      break;

    case STM_RDC:
      // GPRS-штатный RS485 // это остается чтобы обеспечить работу по
      // радиомодему
      monitor1_rec_rs485_1();
      monitor1_tm_rs485_1();

      // GPRS-эмулированный RS485
      monitor_rec_rs485_2();
      monitor_tm_rs485_2();

      if ((fl_tx485.mon1 == 0) && (fl_485_2.busy == 0) &&
          (fl_ip.act_ip_end == 1)) {
        fl_cts_232_ignor = FALSE;
        event_modem = EVM_RDC;
      }

      switch (event_modem) {
      case EVM_RDC:
        write_log_info(ST_CLR_PPP, CLR_PPP_RDY);

        state_modem = STM_DC;
        // обнуление ppp переменных
        // и запртить передачу по GPRS
        UCSR0B = UCSR0B & ~TXEN;
        UCSR0B = UCSR0B & ~TXCIE;
        clr_var_ppp();
        init_scen_stm_dc_1();
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим
        SVD2_1_OFF;
        SVD2_2_OFF;
        break;
      }

      break;

    case STM_DC:

      at_com_tx(cnt_com);
      at_com_rx(cnt_com);
      at_com_scen_stm_dc_1(&cnt_com, &rep);

      switch (event_modem) {
      case EVM_AT_OK:
        write_log_info(ST_CLR_PPP, CLR_PPP_OK);
        state_modem = STM_SC;
        init_scen_stm_sc();
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим

        break;
      case EVM_AT_ERR:
        write_log_info(ST_CLR_PPP, CLR_PPP_ERR);
        state_modem = STM_OFF;
        start_func_stm_off();
        event_modem = EVM_NONE;
        command_AT = TRUE; // включен коммандный режим

        break;
      }
      break;
    case STM_OFF:
      switch (event_modem) {
      case EVM_TM1:

        start_func_stm_on();
        event_modem = EVM_NONE;
        break;
      }
      break;
    }
  }
}
