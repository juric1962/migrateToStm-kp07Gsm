#include "def_conf.h"
#include "map_ef.h"
#include "ozu_map.h"
#include <inavr.h>
#include <iom1280.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "def_link.h"
// #include "def_log.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "map_ad.h"

#define VOL_PUNKT 30 //(меню от 0 до VOL_PUNKT)  //dobavka

#ifdef VERS_BASE
__flash char ozu_vers[] = {'O', 'K', '0', '1', '.', '0', '1',
                           ':', '1', '4', '0', '5', '0', '7'};
#endif

#ifdef VERS_LUFKIN
__flash char ozu_vers[] = {'O', 'K', '0', '2', '.', '0', '1',
                           ':', '1', '4', '0', '5', '0', '7'};
#endif

#ifdef VERS_UNKNOWN
__flash char ozu_vers[] = {'O', 'K', '0', '0', '.', '0', '1',
                           ':', '1', '4', '0', '5', '0', '7'};
#endif

#pragma location = FIRST_ON
__eeprom __no_init unsigned char e_first_on;

#pragma location = A_DEBUG
__eeprom __no_init unsigned char e_debug;

extern unsigned int temperatura;

extern struct {

  unsigned char tek_pin : 1;
  unsigned char old_pin : 1;
  unsigned char fl_ch_tc : 1;
  unsigned char real_tc;
  unsigned int count_tc;
  unsigned char type;
  unsigned char cnt_block_tc;
} struct_tcc1, struct_tcc2;

void control_temperatura(void);

unsigned char fl_at_mom_232;
unsigned char Regim;

extern void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                           unsigned int num, unsigned char flag,
                           unsigned char znach);
extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                             unsigned int num);

extern unsigned long int burst_ds_r(void);
extern void set_rlt(unsigned char address, unsigned char *data);

extern void lock_it(void);

extern void s_port(unsigned char ch);
extern void mov_lf(void);
extern void mov_s(char size, char __flash *p);

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

extern unsigned char point_log_buf;

__flash char err_t[] = {0xd, 0xa, 'E', 'R', 'R'};
__flash char ok_t[] = {0xd, 0xa, 'O', 'K'};

__flash char map_vers[] = {'F', 'K', '0', '2', '.', '0', '1',
                           ':', '2', '7', '0', '9', '0', '7'};
__flash char versia[] = {'V', 'K', '0', '2', '.', '0', '1',
                         ':', '1', '4', '1', '1', '0', '7'};

__flash char zp_udp[] = {'U', 'D', 'P', 'p', 'o', 'r', 't', '-', '>'};
__flash char zp_num[] = {'N', 'u', 'm', '-', '>'};
__flash char zp_nat[] = {'S', 'e', 's', ' ', 'S', 'I', 'M', '1', '-', '>'};
__flash char zp_nat_r[] = {'S', 'e', 's', ' ', 'S', 'I', 'M', '2', '-', '>'};

__flash char zp_link[] = {'T', 'm', ' ', 'c', 'c', 'h', '-', '>'};
__flash char zp_tm_link[] = {'T', 'm', ' ', 'a', 'n', 's', '-', '>'};

__flash char zp_tm_no_link[] = {'T', 'm', ' ', 'd',
                                'c', 'n', '-', '>'}; //!!!!!!!!!!!!!!!!dobavka
__flash char zp_tm_link_res[] = {'T', 'm', ' ', 'r',
                                 'e', 's', '-', '>'}; //!!!!!!!!!!!!!!!!dobavka

__flash char zp_tm_vz[] = {'T', 'm', ' ', 't', 'a', 'k', 'e', '-', '>'};
__flash char zp_tm_cl[] = {'T', 'm', ' ', 'c', 'l', '-', '>'};
__flash char zp_num_seq[] = {'C', 'l', ' ', 'm', 's', 'q', '-', '>'};
__flash char zp_des_seq[] = {'D', 'e', 's', ' ', 'm', 's', 'g', '-', '>'};

__flash char zp_type_tc1[] = {'T', 'y', 'p', 'e', ' ', 'T', 'S', '1', '-', '>'};
__flash char zp_type_tc2[] = {'T', 'y', 'p', 'e', ' ', 'T', 'S', '2', '-', '>'};

__flash char zp_dbg[] = {'D', 'B', 'G', '-', '>'};
//__flash char zp_gprs_s[]=     {'S','.','r','s','2','3','2','-','>'};
__flash char zp_ts1[] = {'T', 'S', '1', '-', '2', '-', '>'};
__flash char zp_temp[] = {'t', '-', '>'};
__flash char zp_vosst[] = {'S', 'e', 't', ' ', 'd', 'e', 'f', '-', '>'};
__flash char zp_time[] = {'T', 'i', 'm', 'e', '-', '>'};
__flash char zp_date[] = {'D', 'a', 't', 'e', '-', '>'};
__flash char zp_log_file[] = {'L', 'o', 'g', ' ', 'f', 'i', 'l', 'e', '-', '>'};
__flash char zp_keys[] = {'K', 'e', 'y', ' ', 'd', 'e', 'f', '-', '>'};

__flash unsigned char keys_def[] = {'s', 'm', 'a', 'r', 't', 'p', 'l', 'u'};

__flash unsigned char def_apn[] = {'i', 'n', 't', 'e', 'r', 'n', 'e', 't',
                                   '.', 'm', 't', 's', '.', 'r', 'u', 0x00};
__flash unsigned char def_usname[] = {0x00};
__flash unsigned char def_uspsw[] = {0x00};

__flash unsigned char def_apnr[] = {'i', 'n', 't', 'e', 'r', 'n', 'e',
                                    't', '.', 'b', 'e', 'e', 'l', 'i',
                                    'n', 'e', '.', 'r', 'u', 0x00};
__flash unsigned char def_usnamer[] = {'b', 'e', 'e', 'l', 'i', 'n', 'e', 0x00};
__flash unsigned char def_uspswr[] = {'b', 'e', 'e', 'l', 'i', 'n', 'e', 0x00};

__flash unsigned char def_apn1[] = {'i', 'n', 't', 'e', 'r', 'n', 'e', 't',
                                    '.', 'm', 't', 's', '.', 'r', 'u', 0x00};
__flash unsigned char def_usname1[] = {0x00};
__flash unsigned char def_uspsw1[] = {0x00};

__flash unsigned char def_apn2[] = {'i', 'n', 't', 'e', 'r', 'n', 'e',
                                    't', '.', 'b', 'e', 'e', 'l', 'i',
                                    'n', 'e', '.', 'r', 'u', 0x00};
__flash unsigned char def_usname2[] = {'b', 'e', 'e', 'l', 'i', 'n', 'e', 0x00};
__flash unsigned char def_uspsw2[] = {'b', 'e', 'e', 'l', 'i', 'n', 'e', 0x00};

__flash unsigned char def_apn3[] = {'i', 'n', 't', 'e', 'r',
                                    'n', 'e', 't', 0x00};
__flash unsigned char def_usname3[] = {0x00};
__flash unsigned char def_uspsw3[] = {0x00};

unsigned char keys[10];

extern unsigned int crc_m1(unsigned char *ka, unsigned int num,
                           unsigned int crc);
extern void delay(unsigned int period);

unsigned char index_pa;

extern unsigned char buf_tx_232[VOL_TX_PPP];
extern unsigned int vol_tx_ppp;

unsigned char ret_version(unsigned char unit, unsigned char *ptr) {
  unsigned char i;

  if (unit == 2) {
    for (i = 0; i < sizeof(ozu_vers); i++) {
      *ptr = ozu_vers[i];
      ptr++;
    }
    *ptr = 0;
    return (sizeof(ozu_vers) + 1);
  }

  if (unit == 0) {
    for (i = 0; i < sizeof(versia); i++) {
      *ptr = versia[i];
      ptr++;
    }
    *ptr = 0;
    return (sizeof(versia) + 1);
  } else {
    for (i = 0; i < sizeof(versia); i++) {
      *ptr = map_vers[i];
      ptr++;
    }
    *ptr = 0;
    return (sizeof(map_vers) + 1);
  }
}

void vosstan_memory(void) {
  unsigned char i, j, buf[70], temp;

  j = sizeof(map_vers);
  for (i = 0; i < j; i++)
    buf[i] = map_vers[i];
  *(unsigned int *)&buf[j] = crc_m1(&buf[0], j, 0xffff);
  WrArrayToFlesh(A_VER_MAP, &buf[0], j + 2, 0, 0);

  // загрузка поумолчанию
  temp = sizeof(def_apn) - 1;
  j = 0;
  buf[0] = temp;
  j = 1;
  for (i = 0; i < temp; i++) {
    buf[j] = def_apn[i];
    j++;
  }

  temp = sizeof(def_usname) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_usname[i];
    j++;
  }

  temp = sizeof(def_uspsw) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_uspsw[i];
    j++;
  }

  for (i = 0; i < L_C_GPRS_MAX; i++) {
    buf[j] = 0;
    j++;
  }
  *(unsigned int *)&buf[L_C_GPRS_MAX - 2] =
      crc_m1(&buf[0], L_C_GPRS_MAX - 2, 0xffff);
  WrArrayToFlesh(A_C_GPRS, &buf[0], L_C_GPRS_MAX, 0, 0);

  temp = sizeof(def_apnr) - 1;
  j = 0;
  buf[0] = temp;
  j = 1;
  for (i = 0; i < temp; i++) {
    buf[j] = def_apnr[i];
    j++;
  }

  temp = sizeof(def_usnamer) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_usnamer[i];
    j++;
  }

  temp = sizeof(def_uspswr) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_uspswr[i];
    j++;
  }

  for (i = 0; i < L_CR_GPRS_MAX; i++) {
    buf[j] = 0;
    j++;
  }
  *(unsigned int *)&buf[L_CR_GPRS_MAX - 2] =
      crc_m1(&buf[0], L_CR_GPRS_MAX - 2, 0xffff);
  WrArrayToFlesh(A_CR_GPRS, &buf[0], L_CR_GPRS_MAX, 0, 0);

  buf[OFS_IP] = DEF_IP_SRV_0;
  buf[OFS_IP + 1] = DEF_IP_SRV_1;
  buf[OFS_IP + 2] = DEF_IP_SRV_2;
  buf[OFS_IP + 3] = DEF_IP_SRV_3;
  *(unsigned int *)&buf[OFS_PORT] = DEF_UDP_PORT; // UDP порт
  *(unsigned int *)&buf[OFS_NUM] = DEF_NUM_SELF;  // свой номер по умолчанию
  *(unsigned int *)&buf[OFS_IP_PAR_CRC] = crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
  WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);

  *(unsigned int *)&buf[OFS_NAT] = DEF_CNTR_NAT;
  *(unsigned int *)&buf[OFS_CCH] = DEF_CNTR_LINK;
  *(unsigned int *)&buf[OFS_TM_CH] = DEF_TM_CNTR_LINK;

  *(unsigned int *)&buf[OFS_TM_NO_LINK] = DEF_TM_NO_LINK;   // dobavka
  *(unsigned int *)&buf[OFS_TM_LINK_RES] = DEF_TM_LINK_RES; // dobavka

  *(unsigned int *)&buf[OFS_NAT_R] = DEF_CNTR_NAT_R;

  *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
  WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

  buf[OFS_DES_SEQ] = (~DEF_DES_SEQ) & 0x01;
  *(unsigned int *)&buf[OFS_NUM_CL] = DEF_NUM_DST_SEQ;
  *(unsigned int *)&buf[OFS_TM_VZ] = DEF_VOL_TM_VZAT;
  *(unsigned int *)&buf[OFS_TM_CL] = DEF_TM_CNTR_CL;
  buf[OFS_TP_TS] = DEF_TP_TS1;
  buf[OFS_TP_TS + 1] = DEF_TP_TS2;
  *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
      crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
  WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

  buf[OFS_GPRS_DES] = ~DEF_DES_PORT_GPRS;
  *(unsigned long int *)&buf[OFS_GPRS_S] = DEF_PORT_GPRS_S;
  *(unsigned int *)&buf[OFS_PORT_GPRS_CRC] =
      crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff);
  WrArrayToFlesh(A_PORT_GPRS, &buf[0], L_PORT_GPRS, 0, 0);

  j = 0;
  temp = sizeof(def_apn1) - 1;
  buf[0] = temp;
  j = 1;
  for (i = 0; i < temp; i++) {
    buf[j] = def_apn1[i];
    j++;
  }

  temp = sizeof(def_usname1) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_usname1[i];
    j++;
  }

  temp = sizeof(def_uspsw1) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_uspsw1[i];
    j++;
  }

  for (i = 0; i < L_C1_GPRS_MAX; i++) {
    buf[j] = 0;
    j++;
  }
  *(unsigned int *)&buf[L_C1_GPRS_MAX - 2] =
      crc_m1(&buf[0], L_C1_GPRS_MAX - 2, 0xffff);
  WrArrayToFlesh(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX, 0, 0);

  j = 0;
  temp = sizeof(def_apn2) - 1;
  buf[0] = temp;
  j = 1;
  for (i = 0; i < temp; i++) {
    buf[j] = def_apn2[i];
    j++;
  }

  temp = sizeof(def_usname2) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_usname2[i];
    j++;
  }

  temp = sizeof(def_uspsw2) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_uspsw2[i];
    j++;
  }

  for (i = 0; i < L_C2_GPRS_MAX; i++) {
    buf[j] = 0;
    j++;
  }
  *(unsigned int *)&buf[L_C2_GPRS_MAX - 2] =
      crc_m1(&buf[0], L_C2_GPRS_MAX - 2, 0xffff);
  WrArrayToFlesh(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX, 0, 0);

  j = 0;
  temp = sizeof(def_apn3) - 1;
  buf[0] = temp;
  j = 1;
  for (i = 0; i < temp; i++) {
    buf[j] = def_apn3[i];
    j++;
  }

  temp = sizeof(def_usname3) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_usname3[i];
    j++;
  }

  temp = sizeof(def_uspsw3) - 1;
  buf[j] = temp;
  j++;
  for (i = 0; i < temp; i++) {
    buf[j] = def_uspsw3[i];
    j++;
  }

  for (i = 0; i < L_C3_GPRS_MAX; i++) {
    buf[j] = 0;
    j++;
  }
  *(unsigned int *)&buf[L_C3_GPRS_MAX - 2] =
      crc_m1(&buf[0], L_C3_GPRS_MAX - 2, 0xffff);
  WrArrayToFlesh(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX, 0, 0);
}

unsigned char check_memory_map(void) {
  unsigned char i, j, buf[70], temp;

  j = sizeof(map_vers);

  RdFromFleshToArr(A_VER_MAP, &buf[0], j + 2);
  if (*(unsigned int *)&buf[j] != crc_m1(&buf[0], j, 0xffff))
    return (1);
  for (i = 0; i < j; i++)
    if (buf[i] == map_vers[i])
      continue;
    else
      return (1);

  RdFromFleshToArr(A_C_GPRS, &buf[0], L_C_GPRS_MAX);
  if (*(unsigned int *)&buf[L_C_GPRS_MAX - 2] !=
      crc_m1(&buf[0], L_C_GPRS_MAX - 2, 0xffff)) {

    // загрузка поумолчанию
    j = 0;
    temp = sizeof(def_apn) - 1;
    buf[0] = temp;
    j = 1;
    for (i = 0; i < temp; i++) {
      buf[j] = def_apn[i];
      j++;
    }

    temp = sizeof(def_usname) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_usname[i];
      j++;
    }

    temp = sizeof(def_uspsw) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_uspsw[i];
      j++;
    }

    for (i = 0; i < L_C_GPRS_MAX; i++) {
      buf[j] = 0;
      j++;
    }

    *(unsigned int *)&buf[L_C_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C_GPRS, &buf[0], L_C_GPRS_MAX, 0, 0);
  }

  RdFromFleshToArr(A_CR_GPRS, &buf[0], L_CR_GPRS_MAX);
  if (*(unsigned int *)&buf[L_CR_GPRS_MAX - 2] !=
      crc_m1(&buf[0], L_CR_GPRS_MAX - 2, 0xffff)) {

    // загрузка поумолчанию
    j = 0;
    temp = sizeof(def_apnr) - 1;
    buf[0] = temp;
    j = 1;
    for (i = 0; i < temp; i++) {
      buf[j] = def_apnr[i];
      j++;
    }

    temp = sizeof(def_usnamer) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_usnamer[i];
      j++;
    }

    temp = sizeof(def_uspswr) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_uspswr[i];
      j++;
    }

    for (i = 0; i < L_CR_GPRS_MAX; i++) {
      buf[j] = 0;
      j++;
    }

    *(unsigned int *)&buf[L_CR_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_CR_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_CR_GPRS, &buf[0], L_CR_GPRS_MAX, 0, 0);
  }

  RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
  if (*(unsigned int *)&buf[OFS_IP_PAR_CRC] !=
      crc_m1(&buf[0], L_IP_PAR - 2, 0xffff)) {

    buf[OFS_IP] = DEF_IP_SRV_0;
    buf[OFS_IP + 1] = DEF_IP_SRV_1;
    buf[OFS_IP + 2] = DEF_IP_SRV_2;
    buf[OFS_IP + 3] = DEF_IP_SRV_3;

    *(unsigned int *)&buf[OFS_PORT] = DEF_UDP_PORT; // UDP порт
    *(unsigned int *)&buf[OFS_NUM] =                // свой номер по умолчанию

        *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
            crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
    WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);
  }

  RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
  if (*(unsigned int *)&buf[OFS_C_PAR_CRC] !=
      crc_m1(&buf[0], L_C_PAR - 2, 0xffff)) {

    *(unsigned int *)&buf[OFS_NAT] = DEF_CNTR_NAT;
    *(unsigned int *)&buf[OFS_CCH] = DEF_CNTR_LINK;
    *(unsigned int *)&buf[OFS_TM_CH] = DEF_TM_CNTR_LINK;

    *(unsigned int *)&buf[OFS_TM_NO_LINK] = DEF_TM_NO_LINK;   // dobavka
    *(unsigned int *)&buf[OFS_TM_LINK_RES] = DEF_TM_LINK_RES; // dobavka

    *(unsigned int *)&buf[OFS_NAT_R] = DEF_CNTR_NAT_R;

    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);
  }

  RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
  if (*(unsigned int *)&buf[OFS_SEQ_PAR_CRC] !=
      crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff)) {

    buf[OFS_DES_SEQ] = ~DEF_DES_SEQ;
    *(unsigned int *)&buf[OFS_NUM_CL] = DEF_NUM_DST_SEQ;
    *(unsigned int *)&buf[OFS_TM_VZ] = DEF_VOL_TM_VZAT;
    *(unsigned int *)&buf[OFS_TM_CL] = DEF_TM_CNTR_CL;
    buf[OFS_TP_TS] = DEF_TP_TS1;
    buf[OFS_TP_TS + 1] = DEF_TP_TS2;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);
  }

  RdFromFleshToArr(A_PORT_GPRS, &buf[0], L_PORT_GPRS);
  if (*(unsigned int *)&buf[OFS_PORT_GPRS_CRC] !=
      crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff)) {

    buf[OFS_GPRS_DES] = ~DEF_DES_PORT_GPRS;
    *(unsigned long int *)&buf[OFS_GPRS_S] = DEF_PORT_GPRS_S;

    *(unsigned int *)&buf[OFS_PORT_GPRS_CRC] =
        crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff);
    WrArrayToFlesh(A_PORT_GPRS, &buf[0], L_PORT_GPRS, 0, 0);
  }

  RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);
  if (*(unsigned int *)&buf[L_C1_GPRS_MAX - 2] !=
      crc_m1(&buf[0], L_C1_GPRS_MAX - 2, 0xffff)) {

    // загрузка поумолчанию
    j = 0;
    temp = sizeof(def_apn1) - 1;
    buf[0] = temp;
    j = 1;
    for (i = 0; i < temp; i++) {
      buf[j] = def_apn1[i];
      j++;
    }

    temp = sizeof(def_usname1) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_usname1[i];
      j++;
    }

    temp = sizeof(def_uspsw1) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_uspsw1[i];
      j++;
    }

    for (i = 0; i < L_C1_GPRS_MAX; i++) {
      buf[j] = 0;
      j++;
    }

    *(unsigned int *)&buf[L_C1_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C1_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX, 0, 0);
  }

  RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);
  if (*(unsigned int *)&buf[L_C2_GPRS_MAX - 2] !=
      crc_m1(&buf[0], L_C2_GPRS_MAX - 2, 0xffff)) {

    // загрузка поумолчанию
    j = 0;
    temp = sizeof(def_apn2) - 1;
    buf[0] = temp;
    j = 1;
    for (i = 0; i < temp; i++) {
      buf[j] = def_apn2[i];
      j++;
    }

    temp = sizeof(def_usname2) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_usname2[i];
      j++;
    }

    temp = sizeof(def_uspsw2) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_uspsw2[i];
      j++;
    }

    for (i = 0; i < L_C2_GPRS_MAX; i++) {
      buf[j] = 0;
      j++;
    }

    *(unsigned int *)&buf[L_C2_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C2_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX, 0, 0);
  }

  RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);
  if (*(unsigned int *)&buf[L_C3_GPRS_MAX - 2] !=
      crc_m1(&buf[0], L_C3_GPRS_MAX - 2, 0xffff)) {

    // загрузка поумолчанию
    j = 0;
    temp = sizeof(def_apn3) - 1;
    buf[0] = temp;
    j = 1;
    for (i = 0; i < temp; i++) {
      buf[j] = def_apn3[i];
      j++;
    }

    temp = sizeof(def_usname3) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_usname3[i];
      j++;
    }

    temp = sizeof(def_uspsw3) - 1;
    buf[j] = temp;
    j++;
    for (i = 0; i < temp; i++) {
      buf[j] = def_uspsw3[i];
      j++;
    }

    for (i = 0; i < L_C3_GPRS_MAX; i++) {
      buf[j] = 0;
      j++;
    }

    *(unsigned int *)&buf[L_C3_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C3_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX, 0, 0);
  }
  return (0);
}

void load_par_first(void) {
  if (e_first_on == 0xff) {
    __watchdog_reset();
    WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);
    __watchdog_reset();
    e_first_on = 0;
  }
  if (e_debug > 3)
    e_debug = RG_WORK; // dobavka
  Regim = e_debug;
}

unsigned char check_keys(void) {
  /*unsigned char i,buf[11];


  RdFromFleshToArr(A_KEYS,&buf[0],L_KEYS);

  if(*(unsigned int*)&buf[L_KEYS-2]==crc_m1(&buf[0],L_KEYS-2,0xffff))
   {
    for(i=0;i<L_KEYS;i++) keys[i]=buf[i];//загрузка ключей в массив ключей
    return(0);
   }

  else return(1);
  */

  RdFromFleshToArr(A_KEYS, &keys[0], L_KEYS);
  if (*(unsigned int *)&keys[L_KEYS - 2] !=
      crc_m1(&keys[0], L_KEYS - 2, 0xffff))
    return (1);
  return (0);
}

/// работа с терминаломработа с терминаломработа с терминаломработа с
/// терминаломабота с терминаломработа с терминаломработа с терминаломработа с
/// терминалом

void mov_massiv(char size, char *p) {
  while (size--)
    s_port(*p++);
}

void read_menu(unsigned char index_menu) {
  char massiv[8];
  unsigned char buf[70];

  unsigned char i, k;

  memset(massiv, 0, 8);

  switch (index_menu)

  {
  case 0:

  menu_beg:
    mov_lf();
    mov_s(sizeof(versia), versia);
    mov_lf();
    s_port('>');
    break;
  case 1:
    mov_lf();
    mov_s(sizeof(map_vers), map_vers);
    mov_lf();
    s_port('>');
    break;
  case 2:
    mov_lf();
    mov_s(sizeof(ozu_vers), ozu_vers);
    mov_lf();
    s_port('>');
    break;

  case 3:

    RdFromFleshToArr(A_C_GPRS, &buf[0], L_C_GPRS_MAX);
    mov_lf();
    s_port('B');
    s_port(':');
  povt_1:

    if ((buf[0] != 0) && (buf[0] < 31)) {
      s_port('"');
      i = 0;
      k = 0;
      while (i < buf[k]) {
        s_port(buf[k + 1 + i]);
        i++;
      }
      s_port('"');

      s_port('"');
      k = k + i + 1;
      i = 0;
      while (i < buf[k]) {
        s_port(buf[k + 1 + i]);
        i++;
      }
      s_port('"');

      s_port('"');
      k = k + i + 1;
      i = 0;
      while (i < buf[k]) {
        s_port(buf[k + 1 + i]);
        i++;
      }
      s_port('"');
    } else {
      s_port('-');
      s_port('-');
      s_port('-');
      s_port('-');
      s_port('-');
    }

    mov_lf();
    s_port('>');
    break;

  case 4:

    RdFromFleshToArr(A_CR_GPRS, &buf[0], L_CR_GPRS_MAX);
    mov_lf();
    s_port('R');
    s_port(':');
    goto povt_1;

  case 5:

    RdFromFleshToArr(A_IP, &buf[0], 4);
    mov_lf();

    sprintf(massiv, "%d\0", buf[0]);
    mov_massiv(strlen(massiv), massiv);
    s_port('.');
    sprintf(massiv, "%d\0", buf[1]);
    mov_massiv(strlen(massiv), massiv);
    s_port('.');
    sprintf(massiv, "%d\0", buf[2]);
    mov_massiv(strlen(massiv), massiv);
    s_port('.');
    sprintf(massiv, "%d\0", buf[3]);
    mov_massiv(strlen(massiv), massiv);

    mov_lf();
    s_port('>');
    break;

  case 6:

    RdFromFleshToArr(A_PORT, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_udp), zp_udp);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 7:
    RdFromFleshToArr(A_NUM, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_num), zp_num);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 8:
    RdFromFleshToArr(A_NAT, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_nat), zp_nat);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;
  case 9:
    RdFromFleshToArr(A_NAT_R, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_nat_r), zp_nat_r);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 10:
    RdFromFleshToArr(A_CCH, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_link), zp_link);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;
  case 11:
    RdFromFleshToArr(A_TM_CH, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_link), zp_tm_link);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

    //!!!!!!!!!!!!!!! dobavka!!!!!!!!!!
  case 12:
    RdFromFleshToArr(A_TM_NO_LINK, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_no_link), zp_tm_no_link);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 13:
    RdFromFleshToArr(A_TM_LINK_RES, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_link_res), zp_tm_link_res);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;
    // EEEEEEEEEEEEEEEEE dobavka

  case 14:
    RdFromFleshToArr(A_DES_SEQ, &buf[0], 1);
    mov_lf();
    mov_s(sizeof(zp_des_seq), zp_des_seq);
    if (buf[0] == 1) {
      s_port('o');
      s_port('n');
    } else {
      s_port('o');
      s_port('f');
      s_port('f');
    }
    mov_lf();
    s_port('>');
    break;

  case 15:

    RdFromFleshToArr(A_NUM_CL, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_num_seq), zp_num_seq);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 16:

    RdFromFleshToArr(A_TM_VZ, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_vz), zp_tm_vz);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 17:

    RdFromFleshToArr(A_TM_CL, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_cl), zp_tm_cl);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 18:
    RdFromFleshToArr(A_TP_TS, &buf[0], 1);
    mov_lf();
    mov_s(sizeof(zp_type_tc1), zp_type_tc1);
    sprintf(massiv, "%u\0", *(unsigned char *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 19:
    RdFromFleshToArr(A_TP_TS + 1, &buf[0], 1);
    mov_lf();
    mov_s(sizeof(zp_type_tc2), zp_type_tc2);
    sprintf(massiv, "%u\0", *(unsigned char *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 20:

    buf[0] = e_debug;
    buf[1] = 0;
    mov_lf();
    mov_s(sizeof(zp_dbg), zp_dbg);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 21:
    RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);
    mov_lf();
    s_port('1');
    s_port(':');
    goto povt_1;

  case 22:
    RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);
    mov_lf();
    s_port('2');
    s_port(':');
    goto povt_1;

  case 23:
    RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);
    mov_lf();
    s_port('3');
    s_port(':');
    goto povt_1;

  case 24:
    mov_lf();
    mov_s(sizeof(zp_ts1), zp_ts1);
    if (struct_tcc1.tek_pin == 1)
      s_port('0');
    else
      s_port('1');
    if (struct_tcc2.tek_pin == 1)
      s_port('0');
    else
      s_port('1');
    mov_lf();
    s_port('>');
    break;
  case 25:

    mov_lf();
    mov_s(sizeof(zp_temp), zp_temp);
    sprintf(massiv, "%u\0", temperatura);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 26:

    mov_lf();
    mov_s(sizeof(zp_vosst), zp_vosst);
    mov_lf();
    s_port('>');
    break;
  case 27:
    burst_ds_r();
    mov_lf();
    mov_s(sizeof(zp_time), zp_time);
    i = (unsigned char)real_time.r_hor;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    s_port(':');
    i = (unsigned char)real_time.r_min;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    s_port(':');
    i = (unsigned char)real_time.r_sec;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 28:
    burst_ds_r();
    mov_lf();
    mov_s(sizeof(zp_date), zp_date);
    i = (unsigned char)real_time.r_date;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    s_port('.');
    i = (unsigned char)real_time.r_month;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    s_port('.');
    i = (unsigned char)real_time.r_year;
    sprintf(massiv, "%d\0", i);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 29:
    mov_lf();
    mov_s(sizeof(zp_log_file), zp_log_file);
    for (i = 0; i <= 6; i++)
      s_port('-');
    mov_lf();
    s_port('>');
    break;
  case 30:
    mov_lf();
    mov_s(sizeof(zp_keys), zp_keys);
    for (i = 0; i <= 6; i++)
      s_port('-');
    mov_lf();
    s_port('>');
    RdFromFleshToArr(A_KEYS, &buf[0], L_KEYS);
    s_port(buf[0]);
    s_port(buf[1]);
    s_port(buf[2]);
    s_port(buf[3]);
    s_port(buf[4]);
    s_port(buf[5]);
    s_port(buf[6]);
    s_port(buf[7]);
    s_port(buf[8]);
    s_port(buf[9]);
    break;

  default:
    goto menu_beg;
  }
  vol_tx_ppp = 0;
}

void write_menu(unsigned char index_menu) {

  union {
    unsigned char bytes[4];
    unsigned long int word;
  } cifra_long;

  unsigned char i, j, k, m, n;
  unsigned char jj;
  unsigned char buf[70];

  switch (index_menu) {

  case 3:
    if (vol_tx_ppp != 2)
      goto bad_com_232;

    switch (buf_tx_232[0])

    {
    case '1':
      RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    case '2':
      RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    case '3':
      RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    default:
      goto bad_com_232;
    }

    *(unsigned int *)&buf[L_C_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C_GPRS, &buf[0], L_C_GPRS_MAX, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 4:
    if (vol_tx_ppp != 2)
      goto bad_com_232;

    switch (buf_tx_232[0])

    {
    case '1':
      RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    case '2':
      RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    case '3':
      RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);
      if ((buf[0] == 0) || (buf[0] > 30))
        goto bad_com_232;
      break;
    default:
      goto bad_com_232;
    }

    *(unsigned int *)&buf[L_CR_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_CR_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_CR_GPRS, &buf[0], L_CR_GPRS_MAX, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 5:
    if (vol_tx_ppp != 16)
      goto bad_com_232;
    if (buf_tx_232[3] != '.')
      goto bad_com_232;
    if (buf_tx_232[7] != '.')
      goto bad_com_232;
    if (buf_tx_232[11] != '.')
      goto bad_com_232;

    RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 255)
      goto bad_com_232;
    else
      buf[OFS_IP] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[4]);
    if (cifra_long.word > 255)
      goto bad_com_232;
    else
      buf[OFS_IP + 1] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[8]);
    if (cifra_long.word > 255)
      goto bad_com_232;
    else
      buf[OFS_IP + 2] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[12]);
    if (cifra_long.word > 255)
      goto bad_com_232;
    else
      buf[OFS_IP + 3] = cifra_long.bytes[0];

    *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
        crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
    WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 6:
    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
    *(unsigned int *)&buf[OFS_PORT] = cifra_long.word; // UDP порт
    *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
        crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
    WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 7:
    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 3)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_IP_PAR, &buf[0], L_IP_PAR);
    *(unsigned int *)&buf[OFS_NUM] = cifra_long.word;
    *(unsigned int *)&buf[OFS_IP_PAR_CRC] =
        crc_m1(&buf[0], L_IP_PAR - 2, 0xffff);
    WrArrayToFlesh(A_IP_PAR, &buf[0], L_IP_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 8:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 10)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
    *(unsigned int *)&buf[OFS_NAT] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 9:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 10)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
    *(unsigned int *)&buf[OFS_NAT_R] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 10:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 10)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
    *(unsigned int *)&buf[OFS_CCH] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 11:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 5)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);
    *(unsigned int *)&buf[OFS_TM_CH] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

    //!!!!!!!!!!! dobavka
  case 12:

    cifra_long.word = atol((char *)&buf_tx_232[0]);

    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);

    if (cifra_long.word != 0)
      if ((cifra_long.word * 60) <= (*(unsigned int *)&buf[OFS_CCH]))
        goto bad_com_232;

    *(unsigned int *)&buf[OFS_TM_NO_LINK] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 13:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < MIN_TM_LINK_RES)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_C_PAR, &buf[0], L_C_PAR);

    if ((cifra_long.word * 60) <= (*(unsigned int *)&buf[OFS_CCH]))
      goto bad_com_232;

    *(unsigned int *)&buf[OFS_TM_LINK_RES] = cifra_long.word;
    *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
    WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;
    // EEEEEEEEEEEEEEEEE dobavka

  case 14:
    if (vol_tx_ppp != 2)
      goto bad_com_232;
    if ((buf_tx_232[0] == '0') || (buf_tx_232[0] == '1'))
      cifra_long.bytes[0] = buf_tx_232[0] - 0x30;
    else
      goto bad_com_232;

    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    buf[OFS_DES_SEQ] = cifra_long.bytes[0];
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 15:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 3)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    *(unsigned int *)&buf[OFS_NUM_CL] = cifra_long.word;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 16:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word == 0)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    *(unsigned int *)&buf[OFS_TM_VZ] = cifra_long.word;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 17:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word < 5)
      goto bad_com_232;
    if (cifra_long.word > 65535)
      goto bad_com_232;

    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    *(unsigned int *)&buf[OFS_TM_CL] = cifra_long.word;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 18:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 2)
      goto bad_com_232;
    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    *(unsigned char *)&buf[OFS_TP_TS] = cifra_long.word;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 19:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 1)
      goto bad_com_232;
    RdFromFleshToArr(A_SEQ_PAR, &buf[0], L_SEQ_PAR);
    *(unsigned char *)&buf[OFS_TP_TS + 1] = cifra_long.word;
    *(unsigned int *)&buf[OFS_SEQ_PAR_CRC] =
        crc_m1(&buf[0], L_SEQ_PAR - 2, 0xffff);
    WrArrayToFlesh(A_SEQ_PAR, &buf[0], L_SEQ_PAR, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 20:
    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 3)
      goto bad_com_232; // dobavka
    e_debug = cifra_long.bytes[0];

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 21:
    if ((vol_tx_ppp < 7) || (vol_tx_ppp > 60))
      goto bad_com_232;

    if (buf_tx_232[0] != '"')
      goto bad_com_232;
    if (buf_tx_232[vol_tx_ppp - 2] != '"')
      goto bad_com_232;

    RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);

    i = 0;
    j = 1;
    n = i;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    *(unsigned int *)&buf[L_C1_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C1_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);

  case 22:
    if ((vol_tx_ppp < 7) || (vol_tx_ppp > 60))
      goto bad_com_232;

    if (buf_tx_232[0] != '"')
      goto bad_com_232;
    if (buf_tx_232[vol_tx_ppp - 2] != '"')
      goto bad_com_232;

    RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);

    i = 0;
    j = 1;
    n = i;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    *(unsigned int *)&buf[L_C2_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C2_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 23:
    if ((vol_tx_ppp < 7) || (vol_tx_ppp > 60))
      goto bad_com_232;

    if (buf_tx_232[0] != '"')
      goto bad_com_232;
    if (buf_tx_232[vol_tx_ppp - 2] != '"')
      goto bad_com_232;

    RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);

    i = 0;
    j = 1;
    n = i;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 2)
      goto bad_com_232;
    j++;
    if (buf_tx_232[j] != '"')
      goto bad_com_232;
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    n = n + buf[n] + 1;
    i = 0;
    j++;
    m = j;
    while (buf_tx_232[j] != '"') {
      j++;
      i++;
    }
    if (j >= vol_tx_ppp - 1)
      goto bad_com_232;
    buf[n] = i;
    for (k = 0; k < buf[n]; k++)
      buf[n + 1 + k] = buf_tx_232[m + k];

    *(unsigned int *)&buf[L_C3_GPRS_MAX - 2] =
        crc_m1(&buf[0], L_C3_GPRS_MAX - 2, 0xffff);
    WrArrayToFlesh(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 26:
    if (vol_tx_ppp != 2)
      goto bad_com_232;
    if (buf_tx_232[0] != '1')
      goto bad_com_232;

    e_debug = RG_WORK;
    vosstan_memory();

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;
  case 27:
    if (vol_tx_ppp != 9)
      goto bad_com_232;
    if (buf_tx_232[2] != ':')
      goto bad_com_232;
    if (buf_tx_232[5] != ':')
      goto bad_com_232;

    burst_ds_r();
    buf[3] = real_time.r_date;
    buf[4] = real_time.r_month;
    buf[5] = real_time.r_year;

    cifra_long.word = atol((char *)&buf_tx_232[6]);
    if (cifra_long.word > 59)
      goto bad_com_232;
    else
      buf[0] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[3]);
    if (cifra_long.word > 59)
      goto bad_com_232;
    else
      buf[1] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word > 23)
      goto bad_com_232;
    else
      buf[2] = cifra_long.bytes[0];

    set_rlt(2, &buf[0]);
    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 28:
    if (vol_tx_ppp != 9)
      goto bad_com_232;
    if (buf_tx_232[2] != '.')
      goto bad_com_232;
    if (buf_tx_232[5] != '.')
      goto bad_com_232;

    burst_ds_r();
    buf[0] = real_time.r_sec;
    buf[1] = real_time.r_min;
    buf[2] = real_time.r_hor;

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if (cifra_long.word == 0)
      goto bad_com_232;
    if (cifra_long.word > 31)
      goto bad_com_232;
    else
      buf[3] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[3]);
    if (cifra_long.word == 0)
      goto bad_com_232;
    if (cifra_long.word > 12)
      goto bad_com_232;
    else
      buf[4] = cifra_long.bytes[0];

    cifra_long.word = atol((char *)&buf_tx_232[6]);
    buf[5] = cifra_long.bytes[0];

    set_rlt(2, &buf[0]);
    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 29:
    if ((vol_tx_ppp == 2) && (buf_tx_232[0] == '0')) {
      WrArrayToFlesh(BEG_BUF_LOG, 0, (L_LOG * 6), 0x01, 0x00);
      point_log_buf = 0;
      vol_tx_ppp = 0;
      mov_s(sizeof(ok_t), ok_t);
      return;
    }

    if (vol_tx_ppp != 4)
      goto bad_com_232;
    if (buf_tx_232[0] != 'l')
      goto bad_com_232;
    if (buf_tx_232[1] != 'o')
      goto bad_com_232;
    if (buf_tx_232[2] != 'g')
      goto bad_com_232;

    RdFromFleshToArr(BEG_BUF_LOG, &buf_tx_232[0], (L_LOG * 6));

    s_port(0xd);
    s_port(0xa);
    for (jj = 0; jj < (L_LOG * 6); jj++)
      s_port(buf_tx_232[jj]);
    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 30:
    if ((vol_tx_ppp == 2) && (buf_tx_232[0] == '1')) {
      for (i = 0; i < (L_KEYS - 2); i++)
        buf[i] = keys_def[i];
      *(unsigned int *)&buf[L_KEYS - 2] = crc_m1(&buf[0], L_KEYS - 2, 0xffff);
      WrArrayToFlesh(A_KEYS, &buf[0], L_KEYS, 0, 0);

      if (check_keys() == 1)
        goto bad_com_232;

      mov_s(sizeof(ok_t), ok_t);
      return;
    } else
      goto bad_com_232;

  default:
    goto bad_com_232;
  }

  //***//*************************************************************

bad_com_232:
  vol_tx_ppp = 0;
  mov_s(sizeof(err_t), err_t);
  // read_menu(index_menu);
  return;
}

void at_mon_232(void) {

  //************************

  switch (buf_tx_232[vol_tx_ppp - 1]) {
  case 0x1b: {

    UCSR0B = UCSR0B & ~0x90;
    index_pa = 1;
    read_menu(index_pa - 1);
    buf_tx_232[vol_tx_ppp - 1] = 0xff;
    vol_tx_ppp = 0;
    UCSR0B = UCSR0B | 0x90;
    break;
  }
  case 0x2b: // верх
  {

    UCSR0B = UCSR0B & ~0x90;

  pusto:

    index_pa++;
    if ((index_pa - 1) > VOL_PUNKT)
      index_pa = 1;
    read_menu(index_pa - 1);
    buf_tx_232[vol_tx_ppp - 1] = 0xff;
    vol_tx_ppp = 0;
    UCSR0B = UCSR0B | 0x90;
    break;
  }

  case 0x2d: // вниз
  {
    UCSR0B = UCSR0B & ~0x90;
    if (index_pa == 0)
      index_pa = 1;
    if ((index_pa - 1) != 0)
      index_pa--;
    else
      index_pa = VOL_PUNKT + 1;
    read_menu(index_pa - 1);
    buf_tx_232[vol_tx_ppp - 1] = 0xff;
    vol_tx_ppp = 0;
    UCSR0B = UCSR0B | 0x90;
    break;
  }

  case 0x0d: // ввод
  {

    UCSR0B = UCSR0B & ~0x90;
    if ((vol_tx_ppp == 1) || (index_pa == 0))
      goto pusto;
    write_menu(index_pa - 1);
    read_menu(index_pa - 1);
    buf_tx_232[vol_tx_ppp - 1] = 0xff;
    vol_tx_ppp = 0;
    UCSR0B = UCSR0B | 0x90;
    break;
  }
  }
}

void monitor_terminal(void) {

  union {
    unsigned char bytes[4];
    unsigned long int word;
  } temp_long;

  //
  //        rts invers
  //
  if ((PINB & CTS2) == CTS2) {
    fl_at_mom_232 = 1;
    index_pa = 0;
    temp_long.word = 0;
    vol_tx_ppp = 1;
    buf_tx_232[0] = 0x0d;
    UCSR2B = 0;
    UBRR2H = R4800_H;
    UBRR2L = R4800_L; /* the baud rate */
    UCSR2B = UCSR2B | RXCIE | RXEN | TXEN;

    WDTCSR = 0x18;
    WDTCSR = 0x0f;
    __watchdog_reset();

    __enable_interrupt();
    delay(30000);

    S1_RD;
    S2_GR;
    S4_RD;
    S3_GR;
    S5_RD;

    do {
      __watchdog_reset();

      control_temperatura();

      temp_long.word++;
      if (temp_long.word >= 30000) {
        S1_CH;
        S2_CH;
        S3_CH;
        S4_CH;
        S5_CH;
        temp_long.word = 0;
      }

      at_mon_232();

    } while ((PINB & CTS2) != 0); //

    lock_it(); // должна быть функция перезагрузки
  }
}

//////
