#include "dfpin.h"
#include <inavr.h>
#include <iom2560.h>

#include "dfcnst.h"
#include "map_ef.h"

void delay(unsigned int period);

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                      unsigned int num);

extern __flash unsigned char tab_crc[];

/*
__flash char tab_proxy[]={0x16,0x31,0x49,00,
                  0x30,0xaf,0x4c,00,
                  0xd1,0x55,0x4c,00,
                  0x6f,0x9b,0x4c,00,
                  0x65,0x8e,0x47,00,
                  0x83,0x2e,0x4c,00,
                  0x24,0x71,0x4c,00,
                  0xef,0x64,0x4c,00,
                  0x39,0xaf,0x4c,00,
                  0x12,0x29,0x4c,00 };
*/

extern unsigned char sel_modul;

char bit_registr1;
unsigned char arr_ds[8], nomer_tab;
unsigned int glutch6;
unsigned char kod_tab[4];
char podnos, kodec[4];
unsigned char prihod_uhod;
int glutch5, tim_prihod_uhod, window_prihod_uhod;
extern unsigned int sost;

void s_port(unsigned char ch);

//  сєфхь чряшё√трЄ№ Єюы№ъю 4 срщЄр т срчє
// срчє їЁрэшь тю тэх°эхь Їых°х
//  1 2 3 4 5 6 7 8
//  т срчє яш°хь 2 3 4 5 срщЄ√
//
/*
char   test_tabletka(void )
              {
              int point_pos,i ;

WrArrayToFlesh(A_TCHMEM+76,&buf1[0],4,0,0);
RdFromFleshToArr(A_TOTAL_EN1,&buf[0],10);

                 for(point_pos=0;point_pos<(4*10);point_pos=point_pos+4)
                     {
                            RdFromFleshToArr(A_TOTAL_EN1,&buf[0],4);
                            for(i=0;i<4;i++)
                             {
                             // EEARH=0;
                             // EEARL=point_pos+i;

                             // ёЁртэштрхь яюфэхёхээющ ЄрсыхЄъющ

                             if(tab_proxy[(point_pos+i)] !=arr_ds[i+1]) goto
next_tabl;

                             }
                          return((point_pos/4)+1);
                     next_tabl:;

                     }
                return(0);
                }
*/

char test_tabletka2(void) {
  int point_pos, i;
  unsigned char buf[4];

  for (point_pos = 0; point_pos < (4 * 253); point_pos = point_pos + 4) {

    RdFromFleshToArr(A_SEG6 + point_pos, &buf[0], 4);
    for (i = 0; i < 4; i++) {
      // ёЁртэштрхь яюфэхёхээющ ЄрсыхЄъющ
      if (buf[i] != arr_ds[i + 1])
        goto next_tab2;
    }

    kod_tab[0] = arr_ds[1];
    kod_tab[1] = arr_ds[2];
    kod_tab[2] = arr_ds[3];
    kod_tab[3] = arr_ds[4];

    return ((point_pos / 4) + 1);
  next_tab2:;
  }

  kod_tab[0] = arr_ds[1];
  kod_tab[1] = arr_ds[2];
  kod_tab[2] = arr_ds[3];
  kod_tab[3] = arr_ds[4];

  return (254);
}

/*
unsigned long int hash_tabletka2(void )
               {
               int point_pos;

               unsigned long int hash_kod;
               union {
                     unsigned long int kod;
                     unsigned char b_kod[4];
                      } hash;


                  hash_kod=0;

                  for(point_pos=0;point_pos<(4*253);point_pos=point_pos+4)
                      {

                           RdFromFleshToArr(A_SEG6+point_pos,&hash.b_kod[0],4);
                           hash_kod= hash_kod ^ hash.kod;

                      }
                 return(hash_kod);
                 }
 */

void send_pin(char comand)
//;
//;               БРОСАЮ КОД
//;               МЛАДШИМИ ВПЕРЕД
{
  char countds;

  for (countds = 0; countds < 8; countds++) {

    __disable_interrupt();
    TCH_O_ONE; //+r
    delay(6);
    // delay(4);
    if (comand & 1)
      TCH_O_ZERO; //+r

    delay(108);
    //  delay(74);
    TCH_O_ZERO; //+r
    __enable_interrupt();
    delay(108);
    // delay(74);
    comand = comand >> 1;
  }
}

//;
//;               ЧТЕНИЕ БАЙТА
//;
char read_pin(void) {
  char countds, data_ds;

  for (countds = 0; countds < 8; countds++) {
    data_ds = data_ds >> 1;

    __disable_interrupt();
    TCH_O_ONE; //+r
    delay(12);

    // delay(8);
    TCH_O_ZERO; //+r
    delay(19);  // 7 jura
    // delay(13);
    //
    //              read pin
    //
    //
    if (PINK & TCH_I)
      data_ds = data_ds | 0x80; // ravil
    else
      data_ds = data_ds & ~0x80;

    // delay(100);
    __enable_interrupt();
    delay(401);
    // delay(276);
  }

  return (data_ds);
}

/*
 char sbros(void)
    {
    char we;

                __disable_interrupt();
                TCH_O_ONE;        //+r
                 __enable_interrupt();
                delay(1871);
               //   delay(1290);
                __disable_interrupt();
                 TCH_O_ZERO;        //+r
                __enable_interrupt();
//
//    wait_low
//
    for (we=0;we<150;we++)
      {
                  delay(19);
                  //delay(13);
      if ((PINK & TCH_I)==0) goto present_lo;           //ravil
      }
    goto bad_ds;
present_lo:

//
//    wait_hi
//
    for (we=0;we<150;we++)
      {
                  delay(19);
             //  delay(13);
      if ((PINK & TCH_I)) goto present_hi;         //ravil
      }
//
//    no ds detected
//
//
bad_ds:
           return(0);    // эхЄ ЄрсыхЄъш !!!!

present_hi:

      return(1);
    //_NOP();

    }

 */

char sbros(void) {
  char we;
  struct {
    char sost;
    unsigned char count_0;
  } sbr;
  delay(1871);

  __disable_interrupt();
  TCH_O_ONE; //+r
  __enable_interrupt();
  delay(1871);
  //   delay(1290);
  __disable_interrupt();
  TCH_O_ZERO; //+r
  __enable_interrupt();
  //
  //    wait_low
  //
  delay(40);
  sbr.sost = 0;
  sbr.count_0 = 0;
  for (we = 0; we < 150; we++) {
    delay(19);
    // эрїюфшь їюЄ с√ фтр эєыхт√ї юЄёўхЄр яюЁ ф
    switch (sbr.sost) {
    case 0: {
      if (PINK & TCH_I)
        break;
      else
        sbr.sost = 1;
      break;
    }
    case 1: {
      if ((PINK & TCH_I) == 0) {
        sbr.count_0++;
        break;
      } else {
        if (sbr.count_0 < 3)
          sbr.sost = 2;
        else {
          sbr.sost = 3;
          sbr.count_0 = 0;
        }
      }
      break;
    }
    case 3: {
      if (PINK & TCH_I) {
        sbr.count_0++;
        if (sbr.count_0 > 10)
          sbr.sost = 4;
        break;
      } else
        sbr.sost = 2;

      break;
    }

    default:
      break;
    }

    if (sbr.sost == 2) {
      delay(1871);
      return (0);
    } // ёсющ яЁш шэшЎшрышчрЎшш
    if (sbr.sost == 4) {
      delay(1871);
      return (1);
    }
  }

  delay(1871);
  return (1);
}

char zamer(void) {
  char cc;

  cc = sbros();
  if (cc == 0)
    return (0);

  delay(187);
  // delay(129);

  send_pin(0x33);

  //
  //              read pin
  //
  for (cc = 0; cc < 8; cc++)
    arr_ds[cc] = read_pin();

  return (1);
}

void ds_handler(void) {

  char ii, i, ds_crc;
  int ak;
  union {
    unsigned long int kod;
    unsigned char by[4];
  } tosch;

  if (sel_modul != 1)
    return;

  if (bit_registr1 & DS_DETECT)

  {

    //            SOUND_ON;    !!!!!!!!!!!!!!!!!

    bit_registr1 = bit_registr1 & ~DS_DETECT;

    if (bit_registr1 & BLOCK_TABLETKA)
      goto end_ds;

    for (ii = 0; ii < 7; ii++) {
      if (zamer() == 0)
        continue;

      //   zamer();
      ds_crc = 0;
      ak = 0;
      for (i = 0; i < 7; i++) {
        ak += arr_ds[i];
        ds_crc = tab_crc[ds_crc ^ arr_ds[i]];
      }

      tosch.by[0] = arr_ds[1];
      tosch.by[1] = arr_ds[2];
      tosch.by[2] = arr_ds[3];
      tosch.by[3] = arr_ds[4];

      if ((arr_ds[4] == 0xff) || (ak == 1785) || (ak == 0) ||
          (tosch.kod > 1000000000))
        continue; // яю яЁюё№сх ╥ю∙хтшъютр

      if ((ds_crc == arr_ds[7])) {

        SOUND_ON; // probe

        nomer_tab = test_tabletka2();
        glutch6 = 1000; // ртЄюфхъЁхьхэЄэ√щ ЄрщьхЁ фы  ЁрчсыюъшЁютъш ёўшЄ√трэш 
                        // ЄрсыхЄъш
        bit_registr1 = bit_registr1 | BLOCK_TABLETKA;
        if ((nomer_tab != 254) && (sost == 4)) {
          bit_registr1 = bit_registr1 | TABLETKA;
          // timer_samovzatia=0; ???????
          // flag_sam_sam=0;   ???????
          // TIFR=TIFR | 0x20; ???????
          // TIMSK=TIMSK | 0x20;   // ICR enable  ??????
          goto end_ds;

          // delay(184);
          // TCH_O_ZERO;
          // EIFR=EIFR   | 0x02;
          // EIMSK=EIMSK | 0x02;   // INT1 enable
          // SOUND_OFF;
          // return;
        }

        //    if(nomer_tab==0) goto end_ds;
        //    bit_registr1=bit_registr1 | TABLETKA;
        //    goto end_ds;

        // яЁютхЁър яютЄюЁэюую яюфэхёхэш  ъы■ўр

        if (podnos != 0) {
          if (kodec[0] != arr_ds[1]) {
            podnos = 0;
            goto bad_pod;
          }
          if (kodec[1] != arr_ds[2]) {
            podnos = 0;
            goto bad_pod;
          }
          if (kodec[2] != arr_ds[3]) {
            podnos = 0;
            goto bad_pod;
          }
          if (kodec[3] != arr_ds[4]) {
            podnos = 0;
            goto bad_pod;
          }
          podnos++;
          window_prihod_uhod = 40;
          goto end_ds;
        }

      bad_pod:
        if (podnos == 0) {
          kodec[0] = arr_ds[1];
          kodec[1] = arr_ds[2];
          kodec[2] = arr_ds[3];
          kodec[3] = arr_ds[4];
          podnos = 1;
          window_prihod_uhod = 5440;
          prihod_uhod = 3;
          glutch5 = 2040;
          goto end_ds;
        }
      }

    } // ъ ЇюЁ

  end_ds:
    delay(267);
    //  delay(184);

    TCH_O_ZERO;
    EIFR = EIFR | 0x02;
    // EIMSK=EIMSK | 0x02;   // INT1 enable
    PCIFR = PCIFR | 0x04;
    PCMSK2 = PCMSK2 | 0x80;
    //_NOP();

    SOUND_OFF;
  }
}
