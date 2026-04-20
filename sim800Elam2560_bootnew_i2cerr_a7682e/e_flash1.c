#include "def_conf.h"
#include "def_link.h"
#include "def_log.h"
#include "def_pin.h"
#include "map_ef.h"
#include "ozu_map.h"
#include <inavr.h>
#include <iom128.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOL_PUNKT 24 //(меню от 0 до VOL_PUNKT)

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

unsigned char fl_at_mom_232;

extern void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                           unsigned int num, unsigned char flag,
                           unsigned char znach);
extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                             unsigned int num);

extern void lock_it(void);
extern unsigned long int burst_ds_r(void);

extern void set_rlt(unsigned char address, unsigned char *data);

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

unsigned char proverka1;
extern union {
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

__flash char err_t[] = {0xd, 0xa, 'E', 'R', 'R'};
__flash char ok_t[] = {0xd, 0xa, 'O', 'K'};

__flash char map_vers[] = {'F', 'K', '0', '1', '.', '0', '1',
                           ':', '0', '6', '0', '4', '0', '7'};
__flash char versia[] = {'V', 'K', '0', '1', '.', '0', '1',
                         ':', '0', '2', '0', '7', '0', '7'};

__flash char zp_udp[] = {'U', 'D', 'P', 'p', 'o', 'r', 't', '-', '>'};
__flash char zp_num[] = {'N', 'u', 'm', '-', '>'};
__flash char zp_nat[] = {'T', 'm', ' ', 's', 'e', 's', '-', '>'};
__flash char zp_link[] = {'T', 'm', ' ', 'c', 'c', 'h', '-', '>'};
__flash char zp_tm_link[] = {'T', 'm', ' ', 'a', 'n', 's', '-', '>'};
__flash char zp_tm_vz[] = {'T', 'm', ' ', 's', 'e', 'q', '-', '>'};
__flash char zp_tm_cl[] = {'T', 'm', ' ', 'c', 'l', '-', '>'};
__flash char zp_num_seq[] = {'C', 'l', ' ', 's', 'e', 'q', '-', '>'};
__flash char zp_des_seq[] = {'D', 'e', 's', ' ', 's', 'e', 'q', '-', '>'};
__flash char zp_des_gprs[] = {'D', 'e', 's', ' ', 'g', 'p', 'r', 's', '-', '>'};
__flash char zp_gprs_s[] = {'S', '.', 'r', 's', '2', '3', '2', '-', '>'};
__flash char zp_ts1[] = {'T', 'S', '1', '-', '8', '-', '-', '>'};
__flash char zp_ts2[] = {'T', 'S', '9', '-', '1', '6', '-', '>'};
__flash char zp_vosst[] = {'S', 'e', 't', ' ', 'd', 'e', 'f', '-', '>'};

__flash char zp_time[] = {'T', 'i', 'm', 'e', '-', '>'};
__flash char zp_date[] = {'D', 'a', 't', 'e', '-', '>'};
__flash char zp_log_file[] = {'L', 'o', 'g', ' ', 'f', 'i', 'l', 'e', '-', '>'};

__flash unsigned char def_apn[] = {'i', 'n', 't', 'e', 'r', 'n', 'e', 't',
                                   '.', 'm', 't', 's', '.', 'r', 'u', 0x00};
__flash unsigned char def_usname[] = {0x00};
__flash unsigned char def_uspsw[] = {0x00};

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

extern unsigned int crc_m1(unsigned char *ka, unsigned int num,
                           unsigned int crc);
extern void delay(unsigned int period);
extern void stop_cond(void);
extern void cloc_scl(void);
extern void out(char tt);
extern void start_cond(void);
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

void seq_read_ext_flesh(int address, int num, unsigned char *buf) {
  char address_high, address_low, i, data;
  int j;

  data = 0;

  address_low = address;
  address_high = address >> 8;

  start_cond();

  out(0xa0);
  out(address_high);
  ////////// out( ((address_high<<1)&0x0e)|0xa0 );                //ravil
  out(address_low); // write adres

  start_cond();
  out(0xa1);
  ///////////  out( ((address_high<<1)&0x0e)|0xa1 );                //ravil

  for (j = 0; j < num; j++) {
    DDRE = DDRE & ~SDA;  // input sda              //+r
    PORTE = PORTE | SDA; // rr

    for (i = 0; i < 8; i++) {
      data = data << 1;
      PORTE = PORTE | SCL; //+r
      // delay(5);
      delay(15);

      // delay(40);

      if (PINE & SDA)
        data = data | 0x01; //+r

      else
        data = data & ~1;

      PORTE = PORTE & ~SCL; //+r

      // delay(5);
      delay(15);
      // delay(40);
    }
    //
    //        при произвольном чтении
    //        после приема последнего бита порт на вывод
    //        и удерживается в 1 нет подтверждения
    //
    if (j == (num - 1))
      PORTE = PORTE | SDA; //  no ack        //+r
    else
      PORTE = PORTE & ~SDA; //  ack                       //+r
                            //
    DDRE = DDRE | SDA;      // output sda                           //+r
    cloc_scl();
    *buf++ = data;
  }

  stop_cond();
}

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                      unsigned int num)
//        adres_flesh  start adres in flesh
//        sdres_ozu    point to start adres in ozu
//        num          number of byte
//
//        if crc compare accomplish return 1
//                            else  return 0
//
//

{
  unsigned char *ff;
  // delay(8000);
  delay(24000);
  // delay(50000);

  ff = adres_ozu;
  seq_read_ext_flesh(adres_flesh, num, ff);
}

void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                    unsigned int num, unsigned char flag,
                    unsigned char znach) // caienu eiioeao?aoee ei ai oeao
{
  unsigned int i, nums;
  unsigned int pages, offset;
  unsigned char address_high, address_low;

  pages = num / 64 + 1;      // eiee?anoai no?aieo aey caiene
  offset = adres_flesh % 64; // Niauaiea ioiineoaeuii ia?aea no?aieou
  if ((adres_flesh + num) >
      32767) //?iau ia caoa?aou aaiiua, ii?ao yoi e ia iaai anee ii oeeeo ienaou
    return;
  nums = 64 - offset;
  if (num < nums)
    nums = num;
  else
    pages++;
  do { // delay(8000);
    delay(24000);
    // delay(50000);
    address_low = adres_flesh;
    address_high = adres_flesh >> 8;
    start_cond();
    out(0xa0);
    out(address_high);
    out(address_low);
    for (i = 0; i < nums; i++) {

      if (flag == 0)
        out(*adres_ozu++);
      else
        out(znach);

      num--;
      adres_flesh++;
      // Ii?ao caanu iaai aoaao iaiyou caoeeeeou
    }
    stop_cond();
    pages--;
    if (num > 64)
      nums = 64;
    else
      nums = num;
  } while (pages > 0);
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
  *(unsigned int *)&buf[OFS_C_PAR_CRC] = crc_m1(&buf[0], L_C_PAR - 2, 0xffff);
  WrArrayToFlesh(A_C_PAR, &buf[0], L_C_PAR, 0, 0);

  buf[OFS_DES_SEQ] = ~DEF_DES_SEQ;
  *(unsigned int *)&buf[OFS_NUM_CL] = DEF_NUM_DST_SEQ;
  *(unsigned int *)&buf[OFS_TM_VZ] = DEF_VOL_TM_VZAT;
  *(unsigned int *)&buf[OFS_TM_CL] = DEF_TM_CNTR_CL;
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

/// работа с терминаломработа с терминаломработа с терминаломработа с
/// терминаломабота с терминаломработа с терминаломработа с терминаломработа с
/// терминалом меню
extern void s_port(unsigned char ch);
/*
void s_port(unsigned char ch)
      {
      wawa:
      if((UCSR0A & 0x20) ==0) goto wawa;
      UDR0=ch;
      }
      */
extern void mov_lf(void);
/*
void    mov_lf (void)
     {
     s_port(0xd);
     s_port(0xa);
     }
     */
extern void mov_s(char size, char __flash *p);
/*void    mov_s (char size, char flash* p)
      {
       while(size--) s_port(*p++);
      }
      */

void mov_massiv(char size, char *p) {
  while (size--)
    s_port(*p++);
}

void read_menu(unsigned char index_menu) {
  char massiv[8];
  unsigned char buf[70];

  unsigned char i, j, k;

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
    //  s_port('C');
    //  s_port(':');
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

  case 5:

    RdFromFleshToArr(A_PORT, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_udp), zp_udp);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 6:
    RdFromFleshToArr(A_NUM, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_num), zp_num);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 7:
    RdFromFleshToArr(A_NAT, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_nat), zp_nat);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 8:
    RdFromFleshToArr(A_CCH, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_link), zp_link);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;
  case 9:
    RdFromFleshToArr(A_TM_CH, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_link), zp_tm_link);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 10:
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

  case 11:

    RdFromFleshToArr(A_NUM_CL, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_num_seq), zp_num_seq);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 12:

    RdFromFleshToArr(A_TM_VZ, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_vz), zp_tm_vz);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 13:

    RdFromFleshToArr(A_TM_CL, &buf[0], 2);
    mov_lf();
    mov_s(sizeof(zp_tm_cl), zp_tm_cl);
    sprintf(massiv, "%u\0", *(unsigned int *)&buf[0]);
    mov_massiv(strlen(massiv), massiv);
    mov_lf();
    s_port('>');
    break;

  case 14:
    RdFromFleshToArr(A_GPRS_DES, &buf[0], 1);
    mov_lf();
    mov_s(sizeof(zp_des_gprs), zp_des_gprs);
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
    RdFromFleshToArr(A_GPRS_S, &buf[0], 4);
    mov_lf();
    mov_s(sizeof(zp_gprs_s), zp_gprs_s);

    if (*(unsigned long int *)&buf[0] == 9600)
      sprintf(massiv, "%s", "9600");
    else if (*(unsigned long int *)&buf[0] == 115200)
      sprintf(massiv, "%s", "115200");
    else
      sprintf(massiv, "%s", "-----");

    mov_massiv(strlen(massiv), massiv);

    mov_lf();
    s_port('>');
    break;

  case 16:
    RdFromFleshToArr(A_C1_GPRS, &buf[0], L_C1_GPRS_MAX);
    mov_lf();
    s_port('1');
    s_port(':');
    goto povt_1;

  case 17:
    RdFromFleshToArr(A_C2_GPRS, &buf[0], L_C2_GPRS_MAX);
    mov_lf();
    s_port('2');
    s_port(':');
    goto povt_1;

  case 18:
    RdFromFleshToArr(A_C3_GPRS, &buf[0], L_C3_GPRS_MAX);
    mov_lf();
    s_port('3');
    s_port(':');
    goto povt_1;

  case 19:
    j = ~((real_tc.arr_tc[0] & 0xf) | ((real_tc.arr_tc[1] << 4) & 0xf0));
    mov_lf();
    mov_s(sizeof(zp_ts1), zp_ts1);
  gmn1:
    for (i = 0; i < 8; i++) {
      if (j & 0x01)
        s_port('1');
      else
        s_port('0');
      j = j >> 1;
    }

    mov_lf();
    s_port('>');
    break;
  case 20:

    j = ~((real_tc.arr_tc[2] & 0xf) | ((real_tc.arr_tc[3] << 4) & 0xf0));
    mov_lf();
    mov_s(sizeof(zp_ts2), zp_ts2);
    goto gmn1;

  case 21:

    mov_lf();
    mov_s(sizeof(zp_vosst), zp_vosst);
    mov_lf();
    s_port('>');
    break;
  case 22:
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

  case 23:
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

  case 24:
    mov_lf();
    mov_s(sizeof(zp_log_file), zp_log_file);
    for (i = 0; i <= 6; i++)
      s_port('-');
    mov_lf();
    s_port('>');
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

  case 5:
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

  case 6:
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

  case 7:

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

  case 8:

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

  case 9:

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

  case 10:
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

  case 11:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    // if (cifra_long.word==1)goto bad_com_232;
    // if (cifra_long.word==2) goto bad_com_232;
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

  case 12:

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

  case 13:

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

  case 14:
    if (vol_tx_ppp != 2)
      goto bad_com_232;
    if ((buf_tx_232[0] == '0') || (buf_tx_232[0] == '1'))
      cifra_long.bytes[0] = buf_tx_232[0] - 0x30;
    else
      goto bad_com_232;

    RdFromFleshToArr(A_PORT_GPRS, &buf[0], L_PORT_GPRS);
    buf[OFS_GPRS_DES] = cifra_long.bytes[0];
    *(unsigned int *)&buf[OFS_PORT_GPRS_CRC] =
        crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff);
    WrArrayToFlesh(A_PORT_GPRS, &buf[0], L_PORT_GPRS, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 15:

    cifra_long.word = atol((char *)&buf_tx_232[0]);
    if ((cifra_long.word != 9600) && (cifra_long.word != 115200))
      goto bad_com_232;

    RdFromFleshToArr(A_PORT_GPRS, &buf[0], L_PORT_GPRS);
    *(unsigned long int *)&buf[OFS_GPRS_S] = cifra_long.word;
    *(unsigned int *)&buf[OFS_PORT_GPRS_CRC] =
        crc_m1(&buf[0], L_PORT_GPRS - 2, 0xffff);
    WrArrayToFlesh(A_PORT_GPRS, &buf[0], L_PORT_GPRS, 0, 0);

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 16:
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

  case 17:
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

  case 18:
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

  case 21:
    if (vol_tx_ppp != 2)
      goto bad_com_232;
    if (buf_tx_232[0] != '1')
      goto bad_com_232;

    vosstan_memory();

    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;
  case 22:
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

    set_rlt(0, &buf[0]);
    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 23:
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

    set_rlt(0, &buf[0]);
    vol_tx_ppp = 0;
    mov_s(sizeof(ok_t), ok_t);
    return;

  case 24:
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
  if ((PINB & CTS_232) == CTS_232) // здесь надо DSR
  {

    OCR0 = 170; // 340mks
    TCCR0 = 0x0b;
    TCNT0 = 0;
    TIMSK = TIMSK | 0x02;

    UBRR0H = R4800_H;
    UBRR0L = R4800_L; /* the baud rate */

    UCSR0B = 0x98;

    WDTCR = 0x18;
    WDTCR = 0x0f;
    __watchdog_reset();

    fl_at_mom_232 = 1;
    __enable_interrupt();
    delay(30000);

    index_pa = 0;

    vol_tx_ppp = 1;
    buf_tx_232[0] = 0x0d;

    temp_long.word = 0;
    PORTA = PORTA | SVD1_1;
    PORTA = PORTA & ~SVD1_2;
    PORTA = PORTA | SVD2_1;
    PORTA = PORTA & ~SVD2_2;

    do {
      __watchdog_reset();
      temp_long.word++;
      if (temp_long.word >= 30000) {
        PORTA = PORTA ^ SVD1_1;
        PORTA = PORTA ^ SVD1_2;
        PORTA = PORTA ^ SVD2_1;
        PORTA = PORTA ^ SVD2_2;
        temp_long.word = 0;
      }

      at_mon_232();

    } while ((PINB & CTS_232) != 0); /// здесь надо DSR

    lock_it(); // должна быть функция перезагрузки
  }
}

//////
