// 10/02 16 добавлена функция управления по ТИТ для модуля МТС
#include "arh_zap.h"
#include "dfpin.h"
#include "map_mbus.h"
#include "tc_avtom.h"
#include <inavr.h>
#include <iom2560.h>

unsigned char flag_set_akb;

union { // структура байта управления
  unsigned int config;
  struct {
    unsigned char nomer_ty
        : 3; // 5 - взвод флага для станции, номер ту 1-4, 7-cbros tu
    unsigned char type_ty : 2;  // тип ту 0 1
    unsigned char level_tc : 1; // уровень сигнала ТС
    unsigned char wr_ch : 1;
    unsigned char enable : 1;
  } bit;
} uprava;

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

unsigned int c_config_tc[SEG8];

// USERTC c_config_tc;

extern unsigned char sel_modul;
extern unsigned char cnt_tu1, cnt_tu2;

extern unsigned int modbus_mem1[SEG1];

struct struct_ts_mb {

  unsigned char tek_tc : 1;
  unsigned char old_tc : 1;
  unsigned char fl_ch_tc : 1;
  unsigned char count_tc;
};

struct struct_ts_mb ts0_mb, ts1_mb, ts2_mb, ts3_mb, ts4_mb, ts5_mb, ts6_mb,
    ts7_mb, ts14_mb, ts15_mb;
unsigned char k_a_tit[2], sec_ttt[2];

void insert_zapis_tc(unsigned char nomer, unsigned char kod);
void insert_zapis_tit(unsigned char nomer);
void local_avtom_tit(void);

void set_avatu(char num) {
  if (sel_modul != 0)
    return; // в режиме МКД нет управления !
  if (num == 1) {
    TU1_ON;
  }
  if (num == 2) {
    TU2_ON;
  }
}

void reset_avatu(char num) {
  if (sel_modul != 0)
    return;
  if (num == 1) {
    TU1_OFF;
  }
  if (num == 2) {
    TU2_OFF;
  }
}
void impuls_avatu(char num, char bv) {
  if (sel_modul != 0)
    return;
  if (num == 1) {
    TU1_ON;
    cnt_tu1 = bv + 1;
  }
  if (num == 2) {
    TU2_ON;
    cnt_tu2 = bv + 1;
  }
}

void start_ts_mb(unsigned char num, struct struct_ts_mb *ts) {
  unsigned int maska_ts;
  maska_ts = 1;
  maska_ts = maska_ts << num;
  if (modbus_mem1[AD_TS] & maska_ts)
    ts->tek_tc = 0;
  else
    ts->tek_tc = 1;
  ts->old_tc = ts->tek_tc;

  if ((num == 4) && (sel_modul != 0))
    if (ts->tek_tc == 0)
      flag_set_akb = 0;
    else
      flag_set_akb = 1; // сеть МКД модуль
  if ((num == 3) && (sel_modul == 0))
    if (ts->tek_tc == 0)
      flag_set_akb = 0;
    else
      flag_set_akb = 1; // сеть МТС модуль
}

void begin_ts_mb(void) {
  //  if(sel_modul!=1)return;
  start_ts_mb(14, &ts14_mb);
  start_ts_mb(15, &ts15_mb);

  start_ts_mb(0, &ts0_mb);
  start_ts_mb(1, &ts1_mb);
  start_ts_mb(2, &ts2_mb);
  start_ts_mb(3, &ts3_mb);
  start_ts_mb(4, &ts4_mb);
  if (sel_modul != 0)
    return;
  start_ts_mb(5, &ts5_mb);
  start_ts_mb(6, &ts6_mb);
  start_ts_mb(7, &ts7_mb);
}

void opros_ts_mb(unsigned char num, struct struct_ts_mb *ts)

{
  unsigned int maska_ts;

  maska_ts = 1;
  maska_ts = maska_ts << num;

  if (modbus_mem1[AD_TS] & maska_ts)
    ts->tek_tc = 0;
  else
    ts->tek_tc = 1;
  if (ts->fl_ch_tc == 1) {

    if (ts->tek_tc == ts->old_tc) {

      ts->count_tc--;
      if (ts->count_tc == 0) {
        ts->fl_ch_tc = 0;
        /*
          if(num==4)
          {

            if(ts->tek_tc==1)
            {
             //запись акб
              flag_set_akb=1;
              if (uprava.bit.wr_ch){insert_zapis_tc(num,ZP_CH_AKB);}
            }
            else
            {//запись сеть
              flag_set_akb=0;
              if (uprava.bit.wr_ch){insert_zapis_tc(num,ZP_CH_SET);}
            }
          }
         */
        if ((num == 4) && (sel_modul != 0))
          if (ts->tek_tc == 0)
            flag_set_akb = 0;
          else
            flag_set_akb = 1; // сеть МКД модуль
        if ((num == 3) && (sel_modul == 0))
          if (ts->tek_tc == 0)
            flag_set_akb = 0;
          else
            flag_set_akb = 1; // сеть МТС модуль

        if (uprava.bit.wr_ch)
          insert_zapis_tc(num, ZP_CH_TC); // запись в архив изменений ТС
      }

    } else {
      ts->fl_ch_tc = 0;
    }

  } else {
    if (ts->tek_tc != ts->old_tc) {
      ts->fl_ch_tc = 1;
      ts->count_tc = 3;
    }
  }
  ts->old_tc = ts->tek_tc;
}

void control_ts(void) {
  unsigned char ivx, cc;

  if (sel_modul != 0)
    cc = 5;
  else
    cc = 8;

  uprava.config = c_config_tc[14];
  opros_ts_mb(14, &ts14_mb);

  uprava.config = c_config_tc[15];
  opros_ts_mb(15, &ts15_mb);

  for (ivx = 0; ivx < cc; ivx++) {

    uprava.config = c_config_tc[ivx];
    switch (ivx) {
    case 0:
      opros_ts_mb(ivx, &ts0_mb);
      break;
    case 1:
      opros_ts_mb(ivx, &ts1_mb);
      break;
    case 2:
      opros_ts_mb(ivx, &ts2_mb);
      break;
    case 3:
      opros_ts_mb(ivx, &ts3_mb);
      break;
    case 4:
      opros_ts_mb(ivx, &ts4_mb);
      break; // акб сеть
    case 5:
      opros_ts_mb(ivx, &ts5_mb);
      break;
    case 6:
      opros_ts_mb(ivx, &ts6_mb);
      break;
    case 7:
      opros_ts_mb(ivx, &ts7_mb);
      break;
    }
  }

  local_avtom_tit();
  return;
}

////////////////////////////////////////////////////////////////////*****************************************************

/////////////////////////////////////  вставка функции управления по ТИТ

void local_avtom_tit(void) {
  // Локальная автоматика===============================
  // extern unsigned char switch_acc; //0-сеть 1-раб.от акк.
  // unsigned char k_a_tit[8]; //количество аварий по входам ТИТ
  // unsigned char ivx,j;//Индекс входа, временный байт

  int run_1;
  int analog_max, analog_min;
  char ivx;

  unsigned char *triger_titmin;
  unsigned char *triger_titmax;

  union {
    unsigned int data;
    struct { // описание записи типа "поле"
      unsigned int ustavka : 10;
      unsigned int delay : 3;
      unsigned int impuls : 3;
    } map;
  } analog_ust_1;

  union {
    unsigned int data;
    struct { // описание записи типа "поле"
      unsigned int ustavka : 10;
      unsigned int gisteresis : 6;

    } map;
  } analog_ust_2;

  union {
    unsigned int data;
    struct { // описание записи типа "поле"
      unsigned int nomer_ty_low : 3;
      unsigned int type_ty_low : 3;
      unsigned int en_alarm_low : 2;
      unsigned int nomer_ty_hi : 3;
      unsigned int type_ty_hi : 3;
      unsigned int en_alarm_hi : 2;
    } map;
  } analog_uprava;

  if (*(unsigned int *)&c_config_tc[0x16] != 0x55aa)
    return;

  triger_titmin = (unsigned char *)&c_config_tc[0x17];
  triger_titmax = triger_titmin + 1;

  // c_config_tc[0x17] это регистр флагов аварий

  /*
   if (c_config_tc.mb[23]==255)

   {//Сброс аварий
    c_config_tc[23]=0;


    triger_titmax=0;
    triger_titmin=0;

    for (j=0;j<2;j++) {k_a_tit[j]=0;}



   }

  */

  // Блок контроля ТИТ====================

  run_1 = 1;
  for (ivx = 0; ivx < 2; ivx++) //
  {
    // Исключение контроля ТИТ была авария по этому входу ТИТ)

    if (sec_ttt[ivx] == real_time.r_sec) {
      run_1 = run_1 << 1;
      continue;
    } // Идет отработка паузы. След.вх.

    analog_ust_1.data = *(unsigned int *)&c_config_tc[0x10 + ivx * 3];
    analog_ust_2.data = *(unsigned int *)&c_config_tc[0x11 + ivx * 3];
    analog_uprava.data = *(unsigned int *)&c_config_tc[0x12 + ivx * 3];

    analog_min = analog_ust_1.map.ustavka;
    analog_max = analog_ust_2.map.ustavka;

    if (analog_uprava.map.en_alarm_low != 1) {
      goto control_max_tit;
    } // запрещено сигнализация о авариях

    // Контроль Уmin -----------------
    if (((*triger_titmin & run_1) == 0) && // ТУ еще не выдавалоь

        (modbus_mem1[AD_TIT1 + ivx] < analog_min) // Меньше Уmin
    ) {
      sec_ttt[ivx] = real_time.r_sec;
      k_a_tit[ivx] = (k_a_tit[ivx] + 1) & 0xf;
      if (k_a_tit[ivx] ==
          (3 + analog_ust_1.map
                   .delay)) // программированная выдержка авария -> выдатьТУ
      {

        *triger_titmin =
            *triger_titmin | run_1; // установить бит аварии по минимуму
        *triger_titmax =
            *triger_titmax &
            ~run_1; // сбросить бит аварии по максимуму для разблокировки

        if (analog_uprava.map.nomer_ty_low > 0 &&
            analog_uprava.map.nomer_ty_low <= 3) // Допустимый.NТУ

        {
          if (analog_uprava.map.type_ty_low == 0)
            impuls_avatu(analog_uprava.map.nomer_ty_low,
                         analog_ust_1.map.impuls);
          if (analog_uprava.map.type_ty_low == 1)
            set_avatu(analog_uprava.map.nomer_ty_low);
          if (analog_uprava.map.type_ty_low == 3)
            set_avatu(analog_uprava.map.nomer_ty_low);
          if (analog_uprava.map.type_ty_low == 2)
            reset_avatu(analog_uprava.map.nomer_ty_low);
        }

        insert_zapis_tit(ivx); // записать в архив

      } // if 3я авария

    } // end if Контроль Уmin------

  // Контроль Уmax -----------------
  control_max_tit:

    if (analog_uprava.map.en_alarm_hi != 1)
      goto control_ala;
    if (((*triger_titmax & run_1) == 0) && // ТУ еще не выдавалоь

        (modbus_mem1[AD_TIT1 + ivx] >= analog_max) //>= Уmax
    ) {
      sec_ttt[ivx] = real_time.r_sec;
      k_a_tit[ivx] = (k_a_tit[ivx] + 1) & 0xf;
      if (k_a_tit[ivx] == (3 + analog_ust_1.map.delay)) // 3я авария -> выдатьТУ
      {

        *triger_titmax =
            *triger_titmax | run_1; // установить бит аварии по максиму
        *triger_titmin =
            *triger_titmin &
            ~run_1; // сбросить бит аварии по минимуму для разблокировки

        if (analog_uprava.map.nomer_ty_hi > 0 &&
            analog_uprava.map.nomer_ty_hi <= 3) // Допустимый.NТУ

        {
          if (analog_uprava.map.type_ty_hi == 0)
            impuls_avatu(analog_uprava.map.nomer_ty_hi,
                         analog_ust_1.map.impuls);
          if (analog_uprava.map.type_ty_hi == 1)
            set_avatu(analog_uprava.map.nomer_ty_hi);
          if (analog_uprava.map.type_ty_hi == 3)
            set_avatu(analog_uprava.map.nomer_ty_hi);
          if (analog_uprava.map.type_ty_hi == 2)
            reset_avatu(analog_uprava.map.nomer_ty_hi);
        }

        insert_zapis_tit(ivx); // записать в архив

      } // if 3я авария
    } // end if Контроль Уmax------

  control_ala:

    if (k_a_tit[ivx] == (3 + analog_ust_1.map.delay) && // ТУ  выдавалоь
        ((modbus_mem1[AD_TIT1 + ivx]) >=
         (analog_min + analog_ust_2.map.gisteresis)) && // >= Уmin + гистерезис
        ((modbus_mem1[AD_TIT1 + ivx]) <
         (analog_max - analog_ust_2.map.gisteresis)) // < Уmax  - гистерезис
        )                                            // Значение в допуске
    {
      k_a_tit[ivx] = 0;
      if (analog_uprava.map.type_ty_low == 3)
        reset_avatu(analog_uprava.map.nomer_ty_low);
      if (analog_uprava.map.type_ty_hi == 3)
        reset_avatu(analog_uprava.map.nomer_ty_hi);
      *triger_titmax = *triger_titmax & ~run_1;
      *triger_titmin = *triger_titmin & ~run_1;
    }

    // Контроль допуска -----------------
    if (k_a_tit[ivx] < (3 + analog_ust_1.map.delay) &&  // ТУ еще не выдавалоь
        ((modbus_mem1[AD_TIT1 + ivx]) >= analog_min) && // >= Уmin
        ((modbus_mem1[AD_TIT1 + ivx]) < analog_max)     // < Уmax
        )                                               // Значение в допуске
    {
      k_a_tit[ivx] = 0;
      // m_reset_ty(c1_byte.mb[68+ivx]-1);//???Уст.сост.ТУ Выкл.реле
      // m_reset_ty(c1_byte.mb[92+ivx]-1);
    }

    run_1 = run_1 << 1;
  } // for вх.ТИТ =======================
  // end Блокa контроля ТИТ====================
  // modbus_mem1[8]=(triger_titmax<<8) + triger_titmin;
}
/////////////////////////////////////
