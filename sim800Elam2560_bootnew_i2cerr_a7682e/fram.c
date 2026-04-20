#include "dfpin.h"
#include <inavr.h>
#include <iom1280.h>

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

void delay_fl(char period) {
bady:
  period--;
  if (period != 0)
    goto bady;
}

void delay_sh(void) {
  char period;
  period = 10;
bbbb:
  period--;
  if (period != 0)
    goto bbbb;
}

/*
void delay_sh(void)
    {
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    }
*/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///////////////////////////////  функции для работы с последовательной флеш
void start_cond(void) {
  SET_SCL;
  SET_SDA;
  SDA_OUT;
  delay_fl(8);
  CLR_SDA;
  delay_fl(8);
  CLR_SCL;
  delay_fl(8);
}

void stop_cond(void) {
  CLR_SCL;
  CLR_SDA;
  SDA_OUT;
  delay_fl(8);
  SET_SCL;
  delay_fl(8);
  SET_SDA;
  delay_fl(8);
}

void cloc_scl(void) {
  CLR_SCL;
  delay_sh();
  SET_SCL;
  delay_sh();
  CLR_SCL;
  delay_sh();
}

void out(char tt) // записать символ
{
  char i;
  for (i = 0; i < 8; i++) {
    if (tt & 0x80) {
      SET_SDA;
    } else
      CLR_SDA;
    cloc_scl();
    tt = tt << 1;
  }

  SDA_IN;
  SET_SDA;
  delay_sh();
  cloc_scl();
  CLR_SDA;
  SDA_OUT;
}

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe

char read_byte(void) {
  char i, data;
  SDA_IN;
  SET_SDA;
  for (i = 0; i < 8; i++) {
    data = data << 1;
    SET_SCL;
    delay_sh();
    if (PIND & SDA)
      data = data | 0x01; //+r
    else
      data = data & ~1;
    CLR_SCL; //+r
    delay_sh();
  }
  //
  //        при произвольном чтении
  //        после приема последнего бита порт на вывод
  //        и удерживается в 1 нет подтверждения
  //
  CLR_SDA;
  SDA_OUT;
  cloc_scl();
  return (data);
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Функции работы с реальным временем

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

char daa_to_hex(char b) { return ((b & 0xf) + ((b >> 4) * 10)); }

char hex_to_daa(char b) { return (((b / 10) << 4) + (b % 10)); }

void set_rlt_poi(void) {
  start_cond();
  out(0xd0);
  out(2);
  stop_cond();
}

void write_byte_rlt(char adres, char data) {
  start_cond();
  out(0xd0);
  out(adres);
  out(data);
  stop_cond();
}

char read_byte_rlt(char adres) {
  char data;
  start_cond();
  out(0xd0);
  out(adres);
  start_cond();
  out(0xd1);
  data = read_byte();
  stop_cond();
  return (data);
}

void start_rlt(void) {
  char data;
  data = read_byte_rlt(1);
  write_byte_rlt(1, data & ~0x80);
}

unsigned long int burst_ds_r(void) {
  char j, data, *time_keeper;
  time_keeper = (char *)&real_time;
  write_byte_rlt(0, 1); // lock timer
  set_rlt_poi();
  start_cond();
  out(0xd1);
  for (j = 0; j < 7; j++) {
    data = read_byte();
    *time_keeper++ = daa_to_hex(data & 0x7f);
  }

  stop_cond();
  write_byte_rlt(0, 0); // unlock timer
  return (0);
  // return (wd_time_as());
  //  unix= wd_time_as();
}

void set_rlt(char address, char *data) {
  write_byte_rlt(0, 2); // lock timer
                        //  old_unix1=unix;

  ////////////////////////////////
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
  write_byte_rlt(0, 0); // update timer

  // записали правильно
  /*
      burst_ds_r();
      unix= wd_time_as();

     if (old_unix1 > unix )

                         {
                         if (( old_unix1 - unix) < 600 ) return;
                         }

     else
                        {
                         if (( unix - old_unix1) < 600 ) return;
                         }



    {
       bit_level=1;
       if ( sost_psm==4)bit_level_psm=1;
     }

   */
}

void start_time(void) {
  stop_cond();
  delay_fl(200);
  stop_cond();
  start_rlt();
}
