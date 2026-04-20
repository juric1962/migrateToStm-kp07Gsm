#include <iom2560.h> 
#include <inavr.h>
#include "dfpin.h"
#include "ozu_map.h"
#include "map_mbus.h"
extern unsigned long int unix;
extern unsigned char Rs232_2_buf_rx_tx[MAX_BUF_RS232_2];
extern void delay(unsigned int period);
char erri2c;
struct
        { // в двоичном коде
          char r_sec;
          char r_min;
          char r_hor;
          char r_day;
          char r_date;
          char r_month;
          char r_year;
          char r_control;
        }  real_time; 

extern unsigned int modbus_mem1[SEG1];
 

struct
        { 
          unsigned long int predUnix;
          unsigned long int currrentUnix;
          char errL;
          char errH;
        }  errRealTime; 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///////////////////////////////  функции для работы с последовательной флеш
 
 
  void start_cond(void)   
  {
   SET_SCL;
   SET_SDA;
   SDA_OUT;
   delay(10);
   CLR_SDA;
   delay(10);
   CLR_SCL;
   delay(10);
  }


 
  void stop_cond(void)      
  { 
   CLR_SCL; 
   CLR_SDA;
   SDA_OUT;
   delay(10);
   SET_SCL; 
   delay(10);
   SET_SDA;
   delay(10);
  }

 
 void cloc_scl(void)  
  {
   CLR_SCL;
   delay(3);
   SET_SCL;
   delay(3);
   CLR_SCL;
   delay(3);
  }

 
    void out(char tt)      // записать символ
  {
   char i;
   for(i=0;i<8;i++)
    {
      if(tt & 0x80)    {SET_SDA;}    
         else  CLR_SDA;     
     cloc_scl();
     tt=tt<<1;
    }
  /*   11 03 23
   SDA_IN;  
   SET_SDA;    
   delay(3);
   cloc_scl();
   CLR_SDA; 
   SDA_OUT; 
   */
   SDA_IN;  
   SET_SDA;    
   delay(3);
   
    CLR_SCL;
   delay(3);
   SET_SCL;     // установить scl
   delay(3);
   if(PIND & SDA) { erri2c=1;  modbus_mem1[AD_TII1 + 8]++; } // если на линии ack высокий уровень это ошибка
   CLR_SCL;
   delay(3);
   
   CLR_SDA; 
   SDA_OUT; 
   
  }
 
 

 
 
 char read_byte(void)         
  {
   char i,data; 
   SDA_IN;   
   SET_SDA; 
   for(i=0;i<8;i++)
   {
     data=data<<1;
     SET_SCL;     
     delay(3);
     if(PIND & SDA)   data=data | 0x01;      //+r
        else data=data & ~1;
     CLR_SCL;              //+r
     delay(3);
   }
//
//        при произвольном чтении
//        после приема последнего бита порт на вывод
//        и удерживается в 1 нет подтверждения
//
    CLR_SDA; 
    SDA_OUT;
    cloc_scl();
    return(data);
  }
  
  
  char read_last_byte(void)         
  {
   char i,data; 
   SDA_IN;   
   SET_SDA; 
   for(i=0;i<8;i++)
   {
     data=data<<1;
     SET_SCL;     
     delay(3);
     if(PIND & SDA)   data=data | 0x01;      //+r
        else data=data & ~1;
     CLR_SCL;              //+r
     delay(3);
   }
//
//        при произвольном чтении
//        после приема последнего бита порт на вывод
//        и удерживается в 1 нет подтверждения
//
    SET_SDA; 
    SDA_OUT;
    cloc_scl();
    return(data);
  } 
 ///////////////////////////////  функции для работы с последовательной флеш


//EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Функции работы с реальным временем

 
 
 unsigned long int wd_time_as(void)
{ unsigned long int sek_32;
   
   
   sek_32 = ( ( (unsigned long int)real_time.r_year * 365 ) + (real_time.r_year>>2) );
   
     switch ( real_time.r_month )
         {
          case 1: sek_32 = (sek_32 + real_time.r_date); break;
          case 2: sek_32 = (sek_32 + real_time.r_date + 31); break;
          case 3: sek_32 = (sek_32 + real_time.r_date + 59); break;
          case 4: sek_32 = (sek_32 + real_time.r_date + 90); break;
          case 5: sek_32 = (sek_32 + real_time.r_date + 120); break;
          case 6: sek_32 = (sek_32 + real_time.r_date + 151); break;
          case 7: sek_32 = (sek_32 + real_time.r_date + 181); break;
          case 8: sek_32 = (sek_32 + real_time.r_date + 212); break;
          case 9: sek_32 = (sek_32 + real_time.r_date + 243); break;
          case 10: sek_32 = (sek_32 + real_time.r_date + 273); break;
          case 11: sek_32 = (sek_32 + real_time.r_date + 304); break;
          case 12: sek_32 = (sek_32 + real_time.r_date + 334); break;
          default:
          sek_32 = (sek_32 + real_time.r_date);
         }
    if ( (real_time.r_year & 0x03) == 0 )
         {
          if (real_time.r_month < 3) sek_32--;
         }
 // sek_32 = ((sek_32 * 86400)+((unsigned long int)real_time.r_hor * 3600)+((unsigned long int)real_time.r_min * 60)+real_time.r_sec + 946684800);
 
    sek_32 = ((sek_32 * 86400)+((unsigned long int)real_time.r_hor * 3600)+((unsigned long int)real_time.r_min * 60)+real_time.r_sec);
 // cek_Unix_2000 = 946684800   
    return sek_32;
} 




char daa_to_hex(char b)
{
return( (b & 0xf) + ((b>>4)*10) );
}

char hex_to_daa(char b)
{
return( ((b/10)<<4) + (b%10) );
}   
 

void set_rlt_poi(void)
  {
  wasa_i2c:
    erri2c=0;
   start_cond();
   out(0xd0);
   out(2);
// 11 03 23   stop_cond();
   
     start_cond(); // 11 03 23
     out(0xd1);    // 11 03 23 
     if( erri2c==0) return;
     else {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa_i2c;
     }
  } 

void write_byte_rlt(char adres, char data)
  { 
  wasa_2__i2c:   
    erri2c=0;
   start_cond();
   out(0xd0);
   out(adres);
   if( erri2c)
      {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa_2__i2c;
     }
   
   out(data);
   stop_cond();
  }

char read_byte_rlt(char adres)
{
   char data;
wasa_1__i2c:
   erri2c=0;
   start_cond();
   out(0xd0);
   out(adres);
   start_cond();
   out(0xd1);
    if( erri2c)
      {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa_1__i2c;
     }
   
   
   data=read_last_byte();
   stop_cond();
   return(data);
}   
   

void start_rlt(void)
  {
  char data;
  data=read_byte_rlt(1);
  write_byte_rlt(1, data & ~0x80); 
  }  

/*
unsigned long int burst_ds_r(void)
  {                                                   
   char j,data, *time_keeper;
   time_keeper=(  char*)&real_time;         
   write_byte_rlt(0, 1);  // lock timer
   set_rlt_poi(); 
   start_cond();
   out(0xd1);
    for(j=0;j<7;j++)
     {
     data=read_byte();
     *time_keeper++=daa_to_hex(data & 0x7f);
     }
     
   stop_cond();
   write_byte_rlt(0, 0);  // unlock timer   
   return (wd_time_as());
  }

*/
  
  
  unsigned long int burst_ds_r(void)
  {                                                   
   char j,data, *time_keeper, *time_keeper_1;
   unsigned char sumak;
   
   
   // создаем временную структуру
   // если все нормально то из временной структуры переписываем в основную
   struct
        { // в двоичном коде
          char r_sec;
          char r_min;
          char r_hor;
          char r_day;
          char r_date;
          char r_month;
          char r_year;
          char r_control;
        }  real_time_temp; 
   time_keeper=(  char*)&real_time_temp;  
        
   write_byte_rlt(0, 1);  // lock timer
   set_rlt_poi(); 
 // 11 03 23  start_cond();
 // 11 03 23  out(0xd1);
    for(j=0;j<6;j++)
     {
     data=read_byte();
     *time_keeper++=daa_to_hex(data & 0x7f);
     }
   
    data=read_last_byte();
     *time_keeper++=daa_to_hex(data & 0x7f); 
    
   stop_cond();
   write_byte_rlt(0, 0);  // unlock timer 
  
   
   
   // 
   // проверка на допустимость значений
   // если неправильно прочиталось то unix не меняется 
   
   /*
   time_keeper=(  char*)&real_time_temp;              
              
                sumak=*time_keeper++;
                if (sumak >=60) return (unix);
                  sumak=*time_keeper++;
                  if (sumak >=60) return (unix);
                  sumak=*time_keeper++;
                   if (sumak >=24) return (unix);
                     *time_keeper++;  
                      sumak=*time_keeper++; 
                     if (sumak ==0) return (unix);
                     if (sumak > 31) return (unix);
                        sumak=*time_keeper++;
                      if (sumak ==0) return (unix);
                     if (sumak > 12) return (unix); 
                          sumak=*time_keeper++;
                          if (sumak > 99) return (unix);

   */
   
   
                          // переписываем структуру 
                          time_keeper  =(  char*)&real_time_temp; 
                          time_keeper_1=(  char*)&real_time;  
                          for(j=0;j<7;j++) *time_keeper_1++ = *time_keeper++;
                       
   
   //                       засинхронизировать часы 19 01 2024
   //   
                          
                          errRealTime.currrentUnix = wd_time_as();
                          if( errRealTime.currrentUnix < errRealTime.predUnix) {
                          errRealTime.errL++;
                          modbus_mem1[AD_TII9]++;
                          errRealTime.errH = 0;
                          } else if ((errRealTime.currrentUnix - errRealTime.predUnix) <=3) {
                          errRealTime.errL=0;
                          errRealTime.errH=0;
                          errRealTime.predUnix = errRealTime.currrentUnix;
                          unix=wd_time_as();
                          return (unix);
                          } else {
                          errRealTime.errH++;
                          modbus_mem1[AD_TII9]++;
                          errRealTime.errL = 0;
                         }
                
                          if ( ( errRealTime.errL >= 10) || ( errRealTime.errH >= 10) )  {
                          errRealTime.predUnix = errRealTime.currrentUnix;
                          errRealTime.errL = 0;
                          errRealTime.errH = 0;
                         }
                          
                         
                          
   //
   //        эти две строчки надо закоментарить если включена синхронизация
   //   
                          
   /*
   unix=wd_time_as();
   return (unix);
  */
  }
  
 
 
 void set_rlt_prov(void)
  {
   start_cond();
   out(0xd0);
   out(0);
   stop_cond();
  } 

/* 
 unsigned long int burst(void)
  {                                                   
   char j;     
   write_byte_rlt(0, 1);  // lock timer
   set_rlt_prov(); 
   start_cond();
   out(0xd1);
   
   //for(j=0;j<1;j++)
    for(j=0;j<12;j++)
     {
     Rs232_2_buf_rx_tx[j+1]=read_byte();
     }    
   stop_cond();
   write_byte_rlt(0, 0);  // unlock timer  
   return(0);
   //  return (wd_time_as());
  }
*/

 
 

void set_rlt(unsigned char address,unsigned char * data)
  {                      
   write_byte_rlt(0, 2);  // lock timer
       
   //////////////////////////////// 
wasa9_i2c: 
    
   erri2c=0;
   start_cond();
   out(0xd0);
   out(address); 
    if( erri2c)
      {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa9_i2c;
     }
   
   out(hex_to_daa(*data++));
   out(hex_to_daa(*data++));
   out(hex_to_daa(*data++));
   out(0);
   out(hex_to_daa(*data++));
   out(hex_to_daa(*data++));
   out(hex_to_daa(*data++));           
   stop_cond();       
   write_byte_rlt(0, 0);  // update timer
  } 

void start_time(void)
{
  stop_cond();
  start_rlt();
}
//////////EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
//работа с памятью

 void set_point_to_read(unsigned int address)
  {
   char address_high,address_low;
   address_low=address;
   address_high=address>>8; 
   
  wasa2_i2c: 
    
   erri2c=0;
   start_cond();
   out(0xa0);
   out(address_high);  
   out(address_low);     
   start_cond();
   out(0xa1);
   
    if( erri2c==0) return;     // долбить пока не будет синхронизации адреса
     else {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa2_i2c;
     }
   } 

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,unsigned int num)   
{
  unsigned int i;
  unsigned char *ff;
  ff = adres_ozu;
  set_point_to_read(adres_flesh);
  if (num==1) {*ff++=read_last_byte();stop_cond();return;}
  
  for (i=0;i<(num-1);i++) *ff++=read_byte();
  *ff++=read_last_byte();
  stop_cond();
 }
 
void RdFromFleshToArrInt(unsigned int adres_flesh, unsigned int *adres_ozu,unsigned int num)   
{
  unsigned int i,data;
  unsigned int *ff;
  ff = adres_ozu;
  set_point_to_read(adres_flesh);
   if(num==1) { *ff=read_byte(); data=read_last_byte(); *ff=*ff | (data<<8); ff++; stop_cond();return;}
  
  for (i=0;i<(num-1);i++) 
  { 
    *ff=read_byte();
    data=read_byte();
    *ff=*ff | (data<<8);
  ff++;
  }
   *ff=read_byte();
    data=read_last_byte();
    *ff=*ff | (data<<8);
  ff++;
  stop_cond();
 }
 
   void set_point_to_write(int address)
  {
   char address_high,address_low;
   address_low=address;
   address_high=address>>8;
   
  wasa3_i2c:
   erri2c=0; 
   start_cond();
   out(0xa0);
   out(address_high);
   out(address_low); 
   
    if( erri2c==0) return;     // долбить пока не будет синхронизации адреса
     else {
       delay(100);
       stop_cond();
       delay(100);
       goto wasa3_i2c;
     }
   
  } 

  void WrArrayToFlesh(unsigned int adres_flesh,  unsigned char *adres_ozu, unsigned int num,unsigned char flag,unsigned char znach)  //запись конфигурации кп во флеш
  {
    unsigned int i;
    set_point_to_write(adres_flesh);
    for (i=0;i<num;i++){ if(flag==0)out(*adres_ozu++);else out(znach);}
    stop_cond();                                           
   }
  
  
    void WrArrayToFleshInt(unsigned int adres_flesh,  unsigned int *adres_ozu, unsigned int num,unsigned char flag,unsigned int znach)  //запись конфигурации кп во флеш
  {
    unsigned int i;
    set_point_to_write(adres_flesh);
    for (i=0;i<num;i++)
        { 
           if(flag==0)
           {
             out(*adres_ozu);
             out((*adres_ozu)>>8);
             adres_ozu++; 
           }
           else
           {
             out(znach);
             out(znach>>8);
           }
        
        }
    stop_cond();                                           
   }

//EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
 
 
 
 
 

