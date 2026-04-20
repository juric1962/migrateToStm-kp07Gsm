#include "def_pin.h"
#include <iom128.h>

struct { // в двоичном коде
  char r_sec;
  char r_min;
  char r_hor;
  char r_day;
  char r_date;
  char r_month;
  char r_year;
  char r_control;
} real_time;

char *time_keeper;

//   последовательное чтение начиная с адреса в массив
//   определенного количества байт
//
void delay(unsigned int period) {
bad:
  period--;
  if (period != 0)
    goto bad;
}

void start_cond(void) //+rr
{
  PORTE = PORTE | 0x0C; //+ravil PE,2 PE,3 +rr
  // r
  DDRE = DDRE | SDA; // output sda   //+r

  delay(14);            // 14
  PORTE = PORTE & ~SDA; // sda low          //+r
  delay(14);
  PORTE = PORTE & ~SCL; // scl low           //+r
  delay(14);
}

void stop_cond(void) //+rr
{
  PORTE = PORTE & ~0x0C;
  DDRE = DDRE | SDA; // output sda             //+r

  PORTE = PORTE & ~0x0C; //+r
  delay(14);
  PORTE = PORTE | SCL; // scl hi    //+r
  delay(14);
  PORTE = PORTE | SDA; // sda hi      //+r
  delay(14);
}

void cloc_scl(void) //+rr
{
  PORTE = PORTE & ~SCL; // scl low        //+r
  delay(14);
  PORTE = PORTE | SCL; // scl hi       //+r
  delay(14);
  PORTE = PORTE & ~SCL; // scl low        //+r
  delay(14);
}

void out(char tt) //+rr
{
  char i;

  for (i = 0; i < 8; i++) {
    if (tt & 0x80)
      PORTE = PORTE | SDA; //+r
    else
      PORTE = PORTE & ~SDA; //+r
    cloc_scl();
    tt = tt << 1;
  }
  DDRE = DDRE & ~SDA;  // input sda   //+r
  PORTE = PORTE | SDA; //
  delay(14);
  cloc_scl();
  PORTE = PORTE & ~SDA; // r
  DDRE = DDRE | SDA;    // output sda       //+r
}

// Работа с часами
char daa_to_hex(char b) { return ((b & 0xf) + ((b >> 4) * 10)); }

void funct_ds(char address, char num) {

  start_cond();

  out(0xd0);
  out(address);
  out(num);

  stop_cond();
}

char hex_to_daa(char b) { return (((b / 10) << 4) + (b % 10)); }

void set_rlt_poi(void) {

  start_cond();

  out(0xd0);
  out(0);

  stop_cond();
}

unsigned long int wd_time_as(void) {
  unsigned long int sek_32;

  sek_32 =
      (((unsigned long int)real_time.r_year * 365) + (real_time.r_year >> 2));

  switch (real_time.r_month) {
  case 1:
    sek_32 = (sek_32 + real_time.r_date);
    break;
  case 2:
    sek_32 = (sek_32 + real_time.r_date + 31);
    break;
  case 3:
    sek_32 = (sek_32 + real_time.r_date + 59);
    break;
  case 4:
    sek_32 = (sek_32 + real_time.r_date + 90);
    break;
  case 5:
    sek_32 = (sek_32 + real_time.r_date + 120);
    break;
  case 6:
    sek_32 = (sek_32 + real_time.r_date + 151);
    break;
  case 7:
    sek_32 = (sek_32 + real_time.r_date + 181);
    break;
  case 8:
    sek_32 = (sek_32 + real_time.r_date + 212);
    break;
  case 9:
    sek_32 = (sek_32 + real_time.r_date + 243);
    break;
  case 10:
    sek_32 = (sek_32 + real_time.r_date + 273);
    break;
  case 11:
    sek_32 = (sek_32 + real_time.r_date + 304);
    break;
  case 12:
    sek_32 = (sek_32 + real_time.r_date + 334);
    break;
  default:
    sek_32 = (sek_32 + real_time.r_date);
  }
  if ((real_time.r_year & 0x03) == 0) {
    if (real_time.r_month < 3)
      sek_32--;
  }
  // sek_32 = ((sek_32 * 86400)+((unsigned long int)real_time.r_hor *
  // 3600)+((unsigned long int)real_time.r_min * 60)+real_time.r_sec +
  // 946684800);

  sek_32 = ((sek_32 * 86400) + ((unsigned long int)real_time.r_hor * 3600) +
            ((unsigned long int)real_time.r_min * 60) + real_time.r_sec);

  // cek_Unix_2000 = 946684800
  return sek_32;
}

void set_rlt(unsigned char address, unsigned char *data) {

  start_cond();

  out(0xd0);
  out(address);

  out(hex_to_daa(*data++));
  out(hex_to_daa(*data++));
  out(hex_to_daa(*data++));
  out(0);
  out(hex_to_daa(*data++));
  out(hex_to_daa(*data++));
  out(hex_to_daa(*data++));

  stop_cond();
}

unsigned long int burst_ds_r(void) {

  char j, i, data;

  data = 0;

  time_keeper = (char *)&real_time;
  set_rlt_poi();
  delay(27);
  start_cond();

  out(0xd1);

  for (j = 0; j < 7; j++) {
    DDRE = DDRE & ~SDA;  // input sda              //+r
    PORTE = PORTE | SDA; // rr

    for (i = 0; i < 8; i++) {
      data = data << 1;
      PORTE = PORTE | SCL; //+r
      delay(14);
      if (PINE & SDA)
        data = data | 0x01; //+r

      else
        data = data & ~1;

      PORTE = PORTE & ~SCL; //+r

      delay(14);
    }
    //
    //        ��� ������������ ������
    //        ����� ������ ���������� ���� ���� �� �����
    //        � ������������ � 1 ��� �������������
    //
    if (j == 6)
      PORTE = PORTE | SDA; //  no ack      //+r
    else
      PORTE = PORTE & ~SDA; //  ack              //+r
                            //
    DDRE = DDRE | SDA;      // output sda                   //+r
    cloc_scl();
    *time_keeper++ = daa_to_hex(data & 0x7f);
  }

  stop_cond();

  return (wd_time_as());
}

////////////////////////////////////////////
// Работа с часами
