#include <iom2560.h> 
#include <inavr.h> 
#include "dfpin.h"
#include "bpa.h"
#include "map_mbus.h"
BPA bpa;


extern union {
       unsigned char mb[SEG3*2];//байтовый массив 
       } c2_byte; //1я страница конфигурации 

//tii_moto.config=c2_byte.mb[8<<1]; время

void bpa_avtomat(void)
{
  switch(bpa.sost) 
  {
  case 0: break;
  case 1: 
    {
     TU2_ON ;
     bpa.time_on = TestUstBpa();
     bpa.sost=2;
     break;
    }
  case 2:
    {
      if(bpa.time_on) {bpa.time_on--;break;} 
      TU2_OFF;
      TU1_OFF;
      bpa.sost=0;
      break;
    }
     case 3:   // выключить
    {
     TU2_ON ;
     bpa.time_off = TestUstBpa();
     bpa.sost=4;
     break;  
    }
     case 4:
    {
      if(bpa.time_off) {bpa.time_off--;break;} 
      TU2_OFF;
      TU1_ON;
      bpa.sost=0;
      break;  
    }
    
  default: bpa.sost=0;break;
  }
}

char TestUstBpa(void)
{
  
  if( c2_byte.mb[16] < 5 ) return(30);
  if( c2_byte.mb[16] > 50 ) return(30);
  return( c2_byte.mb[16] );
  
 
}

