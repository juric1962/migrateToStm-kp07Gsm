// #include <iom128.h>
// #include <string.h>
// #include <stdio.h>
// #include <inavr.h>
// #include <stdlib.h>
// #include "ozu_map.h"
// #include "def_pin.h"
#include "def_conf.h"
#include "map_ef.h"
// #include "def_link.h"
// #include "def_log.h"

extern void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                           unsigned int num, unsigned char flag,
                           unsigned char znach);
extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                             unsigned int num);
extern unsigned long int burst_ds_r(void);
extern unsigned int crc_m1(unsigned char *ka, unsigned int num,
                           unsigned int crc);

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
