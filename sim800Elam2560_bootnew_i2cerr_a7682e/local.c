#include <iom2560.h>      
#include <inavr.h>

#include "map_ef.h"
#include "map_mbus.h"
#include "dfcnst.h"
#include "dfpin.h"
#include "dfproc.h"
#include "bpa.h"

//__flash char prov1[]={'P','R','O','V','1'};
//__flash char prov2[]={'P','R','O','V','2'};
char try_find_arhiv(unsigned long int matrix,char m);
char try_form_arhiv(char m);

extern void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,unsigned int num);
extern void WrArrayToFlesh(unsigned int adres_flesh,  unsigned char *adres_ozu, unsigned int num,unsigned char flag,unsigned char znach);

int crc_sum(unsigned char *ff,unsigned char kol);

extern unsigned long int burst_ds_r(void);
             
extern unsigned int modbus_mem1[SEG1];
                 
extern void s_port(unsigned char ch); 

//extern void send_info(char size,char __flash *p,unsigned char fl_id,unsigned char id);

  /*                                      
   extern   union
            {
          char arr_tii_byte[32];
          unsigned int  arr_tii[16];
            } sstii; 
    
  */
 
     
      
// для разрешения счета мотоминут
// аналогично как в УЗЭП надо сделать
union
            {                        // структура байта управления
             unsigned int config;
             struct
                   {
                   unsigned int interval_zamera      :11;       // 
                   unsigned int type_zam   :2;
                   unsigned int TC_zam     :3;  
                  // unsigned int level_tc             :1;       // ТС_ON_OFF
                  // unsigned int nomer_tc             :3;       // номер ТС 0-7
                  // unsigned int enable               :1;  
                   }  bit;
             }tii_moto; 


 
 
   
   extern  struct
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


unsigned long int cnt_moto[16];

 
  
 


 //YES!!! //YES!!! //YES!!! 

//char err_cnt[8];

union {
       unsigned char mb[SEG3*2];//байтовый массив 
       } c2_byte; //1я страница конфигурации 

union {
       unsigned char mb[SEG99*2];//байтовый массив 
       } c1_byte; // страница конфигурации ГЗУ 

char bit_zamer_psm,sost_psm ;
unsigned long int timer_psm,unix_psm;

unsigned long int timer_tii[16],timer_tit[2];
char old_realsec[16];

unsigned int lock_bit,tii_bit,tii_bit_minut;
unsigned char lock_bit_tit,tit_bit;

extern unsigned long int unix; 

extern char kodec[4];
extern unsigned char sel_modul;
//char arr_cnt_tii_heap;

char bit_level,bit_level_psm; 

unsigned char tarhiv;
unsigned long int start_ttii[16],arr_tii_32[16],start_tii_gzu,delta_ys;
unsigned int cnt_sec;
//YES!!!  //YES!!! //YES!!! //YES!!! 


unsigned long int day_accum; 
char day_chan,day_zap,start_day_zamer,day_good;
unsigned long int day_unix,time_sutok;

char err_psm,err_switch,cnt_try_ys,tekpsm,pred_mask,fin_ys,count_try,count_sboj,try_ty;
char zadan_ys,global_try=3,sost_psm;
 


///////// 
//////////////////////
void end_zamer(char m);
unsigned long int set_end_time(char mi,char ch,char da,char ma,char ye);


void insert_zapis_kd(unsigned char nomer,unsigned char type)
{
     unsigned char i,buf[4];
     unsigned char zapis[14];

     for(i=0;i<12;i++)zapis[i]=0;
      *(unsigned long int*)&zapis[0]=unix;
      zapis[4]=0;
      zapis[5]=0;
      for(i=0;i<6;i++)zapis[4+i]=0;
    //  if(nomer!=0)for(i=0;i<4;i++)zapis[6+i]=tab_proxy[(nomer-1)*4+i];   
      
      if(nomer!=0)
      {
        
        if(nomer!=254)
        {
        RdFromFleshToArr(A_SEG6+((nomer-1)*4),&buf[0],4);
        for(i=0;i<4;i++)zapis[6+i]=buf[i]; 
        }
        else
        {
          for(i=0;i<4;i++)zapis[6+i]=kodec[i];
        }
        
      }
      
      if(nomer==0)
      {
       for(i=0;i<4;i++)zapis[6+i]=0x55;
      }
      

      *(unsigned int*)&zapis[4]=nomer;
      *(unsigned int*)&zapis[10]=type;  
      *(unsigned int*)&zapis[12]=crc_sum(&zapis[0],12); 
     
     WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);      
     modbus_mem1[AD_IND_DNS]++;
     if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}          
     i=modbus_mem1[AD_IND_DNS];     
     WrArrayToFlesh(A_IND_DNS,0,1,1,i);
}




void insert_zapis_tc(unsigned char nomer, unsigned char kod)
{
    unsigned char i;
    unsigned char zapis[14];

     for(i=0;i<12;i++)zapis[i]=0;
     *(unsigned long int*)&zapis[0]=unix;
     *(unsigned int*)&zapis[4]=nomer+1;
     *(unsigned int*)&zapis[6]=modbus_mem1[AD_TS];
     *(unsigned int*)&zapis[8]=(modbus_mem1[AD_TS]>>nomer) & 0x01;
     *(unsigned int*)&zapis[10]=kod; 
     *(unsigned int*)&zapis[12]=crc_sum(&zapis[0],12); 
     
     WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);      
     modbus_mem1[AD_IND_DNS]++;
     if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}          
     i=modbus_mem1[AD_IND_DNS];     
     WrArrayToFlesh(A_IND_DNS,0,1,1,i);
}  


void insert_zapis_tit(unsigned char nomer)
{
    unsigned char i;
    unsigned char zapis[14];

     for(i=0;i<12;i++)zapis[i]=0;
     *(unsigned long int*)&zapis[0]=unix;
     
     *(unsigned int*)&zapis[6]=modbus_mem1[AD_TIT1+nomer];
     
     *(unsigned int*)&zapis[10]=16+nomer; 
     *(unsigned int*)&zapis[12]=crc_sum(&zapis[0],12); 
     
     WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);      
     modbus_mem1[AD_IND_DNS]++;
     if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}          
     i=modbus_mem1[AD_IND_DNS];     
     WrArrayToFlesh(A_IND_DNS,0,1,1,i);
}  
////////////////////////////////////////// 10 11 2014

void day_zamer(void)
  {  
    
    int zamer;
    unsigned char i;
//    char i, akaka, mus;
  
  
    unsigned char zapis[14];

   union
   {     
   long int unix;
   char st[4]; 
   } tik;
   
    union
   {     
   long int acc;
   char a[4]; 
   } sum;
   
   // проверка совпадения на время суточного замера
   
   
    if( start_day_zamer==0) 
          {
    
     if (unix <= time_sutok)   return;
   
   
 // время ч
 time_sutok+=86400;
    
 
      start_day_zamer=1;
      day_good=0;
      day_accum=0;
      day_chan=0;
      day_zap=0;
      
      day_unix=unix;
      return;
           }
           
    
     if ( (day_chan>8) && (day_chan<14)) 
     {
     day_zap=0;day_good=0; day_accum=0;day_chan++; return; 
                  
     }
           
                             zamer=c2_byte.mb[0+(2*day_chan)]+(c2_byte.mb[1+(2*day_chan)] & 7)*256;
                         if (  (zamer >= 1440)  || (zamer ==0)  )  // не снимать замеры для таких каналов
                                                        {
                                                        day_zap=0;
                                                        day_good=0;
                                            day_accum=0;
                                            day_chan++;
                                         
                                            if (day_chan==16) start_day_zamer=0;
                                           
                                            return;
                                                        }
                       /*
                       ДОЛЖНА БЫТЬ ЧТЕНИЕ!!!!!!!!! ПРОВЕРКРЙ СРС!!!
                         if (eprom_busy()==1)  return;
                       p_sys1= & zapis[0];// point to arr                
                       sys1= STRUCT_TII_HEAP + (int)day_zap*14;
                       sys2=12;  
                       sys3=sys1+12;   
                       w_result(22);   
                       */
                  
                       RdFromFleshToArr(STRUCT_TII_HEAP + (int)day_zap*14,&zapis[0],14);   
                         
                         
                      
                       // считали правильно
                       day_zap++; 
                       if((*(int*)&zapis[12])==crc_sum(&zapis[0],12))  //!!!!!ДОЛЖНА БЫТЬ ЧТЕНИЕ!!!!!!!!! ПРОВЕРКРЙ СРС!!! записи
                                        {
                                        
                                        if ( zapis[10]!=day_chan)  goto hlop;  // ищем записи только для даного канала в куче
                                        
                                        
                                         tik.st[0]=zapis[0];
                                         tik.st[1]=zapis[1];
                                         tik.st[2]=zapis[2];
                                         tik.st[3]=zapis[3];  // круг назад и еще 200 секунд  чтобы не зацепить замеры
                                         if ( ( tik.unix >= (day_unix-86400-200) ) && ( tik.unix < (day_unix -10)))
                                                      {
                                                    if ( (zapis[5]==5) && (zapis[4]==0xa0) ) {goto hlop;} // если есть суточная запись не писать
                                                     day_good++;
                                                    
                                                    /*
                                                      #if DEBUG==1
                                                     day_accum++;
                                                      #endif
                                                      #if DEBUG==0
                                                      */
                                                     
                                                      sum.a[0]=zapis[6];
                                                      sum.a[1]=zapis[7];
                                                      sum.a[2]=zapis[8];
                                                      sum.a[3]=zapis[9];
                                                      day_accum=day_accum+sum.acc;
                                                      
                                                     
                                                    //  #endif
                                                      }
                                        } 
                                        
                                        
                         
                                        
                                        
                                        
hlop:                                         
                       if(day_zap >99)  
                                         { 
                                       
                                       
                                       if (day_good==0) goto no_zap;   // если нет ни одного замера то нет и суточного
                                       
                                       
                                        tik.unix=day_unix+2;        
                                        zapis[0]=tik.st[0];
                                        zapis[1]=tik.st[1];
                                        zapis[2]=tik.st[2];
                                        zapis[3]=tik.st[3]; 
  zapis[4]=0xa0;    // 1440  записать суточное время по этому каналу 
  zapis[5]=5;
  
  
  zapis[10]=day_chan;    
  zapis[11]=0;
  
  sum.acc=day_accum;
  
  zapis[6]=sum.a[0];
  zapis[7]=sum.a[1];
  zapis[8]=sum.a[2];
  zapis[9]=sum.a[3]; 
 

 
/* ДОЛЖНА БЫТЬ ЗАПИСЬ В ОСНОВНУЮ КУЧУ!!!!!!!! 
       

*/ 
 
  
*(int*)&zapis[12]=crc_sum(&zapis[0],12);     
WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);
  
  
  
  
/* 
            arr_cnt_tii_heap++; 
            if ( arr_cnt_tii_heap >99) {arr_cnt_tii_heap=0;arr_cnt_tii_heap_modbus=99;}
                                         else arr_cnt_tii_heap_modbus=arr_cnt_tii_heap-1;
                     
           
             dop_sys1=CNT_TII;
             dop_sys2=arr_cnt_tii_heap; 
              
             w_result(23);
            
  передвижка указателя
  */     
  
                        
           modbus_mem1[AD_IND_DNS]++;
           if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}
           i=modbus_mem1[AD_IND_DNS];
           WrArrayToFlesh(A_IND_DNS,0,1,1,i);

  
            
            no_zap:
            
                                            day_zap=0;
                                            day_good=0;
                                            day_accum=0;
                                            day_chan++;
                                            if (day_chan==16) start_day_zamer=0;
                                            
                                          }
           
    }       


/////////////////////////////////////////



////////////////////////////////////////////// 10 11 14

unsigned long int set_time_day_zamer(char date,char month,char year)
{ unsigned long int sek_32;
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
         
        real_time.r_sec=c2_byte.mb[0x12*2];
        real_time.r_min=c2_byte.mb[0x11*2];
        real_time.r_hor=c2_byte.mb[0x10*2];
        real_time.r_date=date;
        real_time.r_month=month;
        real_time.r_year=year; 
//  real_time.r_year = real_time.r_year & 0x7F; 
//  if (real_time.r_year > 99) real_time.r_year = 0; 
//  if ( (real_time.r_date > 31) | (real_time.r_date <1) ) real_time.r_date = 1;    
//  if (real_time.r_hor > 60) real_time.r_hor = 0;      
//  if (real_time.r_min > 60) real_time.r_min = 0;  
//  if (real_time.r_sec > 60) real_time.r_sec = 1;  
//  
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
  sek_32 = ((sek_32 * 86400)+((unsigned long int)real_time.r_hor * 3600)+((unsigned long int)real_time.r_min * 60)+real_time.r_sec );
  sek_32=sek_32+2;   // чтобы избежать 00 часов
 // cek_Unix_2000 = 946684800   
   return sek_32;
} 



  


   void  end_zamer(char m)
{
unsigned long int u_i;
unsigned int zamer;                                     
                              ////////////////////
                             
                                            
                                            zamer=c2_byte.mb[0+(2*m)]+(c2_byte.mb[1+(2*m)] & 7)*256;
                                            if ( zamer> 1440) {timer_tii[m]=0;cnt_moto[m]=0;return;}
                                            cnt_moto[m]=0;
                                            old_realsec[m]=real_time.r_sec;
                                         switch(zamer)
                                              {              
                                                 
                                                 case 10: 
                                                 {  
                                               
                  timer_tii[m]=set_end_time((real_time.r_min/10)*10,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+600; 
                                             
                                             lock_bit= lock_bit & ~(1<<m);
                                                
                                                 break;
                                                 } 
                                                
                                                 
                                                  case 20: 
                                                 { 
                  timer_tii[m]=set_end_time((real_time.r_min/20)*20,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+1200; 
                                             
                                               lock_bit= lock_bit & ~(1<<m);
                                              
                                                 break;
                                                 }  
                                                  case 30: 
                                                 {
                    timer_tii[m]=set_end_time((real_time.r_min/30)*30,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+1800;                                
                                                   
                                               lock_bit= lock_bit & ~(1<<m);
                                                
                                                 break;
                                                 }
                                                 
                                                 case 60: 
                                                 {
                  timer_tii[m]=set_end_time(0,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+3600;                                  
                                                   
                                               lock_bit= lock_bit & ~(1<<m);
                                                
                                                 break;
                                                 }
                                               
                                                 case 120: 
                                                 { 
                 timer_tii[m]=set_end_time(0,
                                             (real_time.r_hor/2)*2,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+7200;                                   
                                               
                                               lock_bit= lock_bit & ~(1<<m);
                                                
                                                 break;
                                                 } 
                                               
                                                 case 240: 
                                                 { 
                 timer_tii[m]=set_end_time(0,
                                             (real_time.r_hor/4)*4,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+14400;                                   
                                              
                                               lock_bit= lock_bit & ~(1<<m);
                                               
                                                 break;
                                                 }
                                                 
                                                 case 360: 
                                                 {
           timer_tii[m]=set_end_time(0,
                                             (real_time.r_hor/6)*6,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+21600;                                   
                                              
                                               lock_bit= lock_bit & ~(1<<m);
                                              
                                                 break;
                                                 }    
                                                 case 0: {timer_tii[m]=0;break;}
                                                 
                                                 
                                   case 1440:
                                                      {  
                                     u_i=set_end_time(c2_byte.mb[34],
                                             c2_byte.mb[32],
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+(unsigned long int)c2_byte.mb[36];                                   
                                      if (u_i <= unix ) timer_tii[m]=u_i+86400;  // если время окончания замера меньше текущего
                                      else  timer_tii[m]=u_i+5;       
                                               lock_bit= lock_bit & ~(1<<m); 
                                                                          
                                     
                                                      break;
                                                      }               
                                                 
                                                 
                                                 
                                                 
                                                 
                                                    
                                                default:
                                                             {
                                               
                                               timer_tii[m]=unix+( (long int)c2_byte.mb[0+2*m]+(long int)(c2_byte.mb[1+2*m] & 7)*256)*60; 
                                               lock_bit= lock_bit & ~(1<<m);
                                               
                                                              break;
                                                              }
                                              }     // sw
                                              
                                          
                             /////////////////////
                                
    }                     
 


// замеры по ТИТ
//
void  end_zamer_tit(char m)
{
unsigned long int u_i;
unsigned int zamer;    
if ( m>=2 ) return;                                 
                              ////////////////////
                                         
                                            zamer=c2_byte.mb[0x40+(2*m)]+(c2_byte.mb[0x41+(2*m)] & 7)*256;
                                            if ( zamer> 1440) {timer_tit[m]=0;return;}
                                         switch(zamer)
                                              {
                                              
                                              
                                              case 5: 
                                                 {  
                                               
                  timer_tit[m]=set_end_time((real_time.r_min/5)*5,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+300; 
                                             
                                             lock_bit_tit= lock_bit_tit & ~(1<<m);
                                                
                                                 break;
                                                 }               
                                                 
                                                 case 10: 
                                                 {  
                                               
                  timer_tit[m]=set_end_time((real_time.r_min/10)*10,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+600; 
                                             
                                             lock_bit_tit= lock_bit_tit & ~(1<<m);
                                                
                                                 break;
                                                 } 
                                                
                                                 
                                                  case 20: 
                                                 { 
                  timer_tit[m]=set_end_time((real_time.r_min/20)*20,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+1200; 
                                             
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                              
                                                 break;
                                                 }  
                                                  case 30: 
                                                 {
                    timer_tit[m]=set_end_time((real_time.r_min/30)*30,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+1800;                                
                                                   
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                                
                                                 break;
                                                 }
                                                 
                                                 case 60: 
                                                 {
                  timer_tit[m]=set_end_time(0,
                                             real_time.r_hor,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+3600;                                  
                                                   
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                                
                                                 break;
                                                 }
                                               
                                                 case 120: 
                                                 { 
                 timer_tit[m]=set_end_time(0,
                                             (real_time.r_hor/2)*2,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+7200;                                   
                                               
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                                
                                                 break;
                                                 } 
                                               
                                                 case 240: 
                                                 { 
                 timer_tit[m]=set_end_time(0,
                                             (real_time.r_hor/4)*4,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+14400;                                   
                                              
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                               
                                                 break;
                                                 }
                                                 
                                                 case 360: 
                                                 {
           timer_tit[m]=set_end_time(0,
                                             (real_time.r_hor/6)*6,
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+21600;                                   
                                              
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                              
                                                 break;
                                                 }    
                                                 case 0: {timer_tit[m]=0;break;}
                                                 
                                                 
                                   case 1440:
                                                      {  
                                     u_i=set_end_time(c2_byte.mb[34],
                                             c2_byte.mb[32],
                                             real_time.r_date,
                                             real_time.r_month,
                                             real_time.r_year)+(unsigned long int)c2_byte.mb[36];                                   
                                      if (u_i <= unix ) timer_tit[m]=u_i+86400;  // если время окончания замера меньше текущего
                                      else  timer_tit[m]=u_i+5;   
                                      
                                     
                                      
                                          
                                               lock_bit_tit= lock_bit_tit & ~(1<<m); 
                                                                          
                                     
                                                      break;
                                                      }               
                                                 
                                                 
                                                 
                                                 
                                                 
                                                    
                                                default:
                                                             {
                                               
                                               timer_tit[m]=unix+( (long int)c2_byte.mb[0x40+ (2*m)]+(long int)(c2_byte.mb[0x41+ (2*m)] & 7)*256)*60; 
                                               lock_bit_tit= lock_bit_tit & ~(1<<m);
                                               
                                                              break;
                                                              }
                                              }     // sw
                                              
                                          
                             /////////////////////
                                
    }                     


    
////////////////////// 
unsigned long int set_end_time(char mi,char ch,char da,char ma,char ye)
{ unsigned long int sek_32;
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
         
        real_time.r_sec=0;
        real_time.r_min=mi;
        real_time.r_hor=ch;
        real_time.r_date=da;
        real_time.r_month=ma;
        real_time.r_year=ye; 
  
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
  sek_32 = ((sek_32 * 86400)+((unsigned long int)real_time.r_hor * 3600)+((unsigned long int)real_time.r_min * 60)+real_time.r_sec );
  sek_32=sek_32+2;   // чтобы избежать 00 часов
   return sek_32;
} 
//////////////////////////////////

  
int crc_sum(unsigned char *ff,unsigned char kol)
{
  int sum_crc;
  char i;
  sum_crc=0;
               
               for (i=0;i<kol;i++)
                                {
                                sum_crc+= *ff;                                
                                *ff++ ;
                                 }     
  
  return(sum_crc);
}



//////////////////
// прочитать сначала архивы на предмет действительности
 unsigned long int read_arhiv_tii(char m)  // 
 {
 char index;

 unsigned int t_i; 
  union
   {     
   long int unix;
   char st[4]; 
   } tik;
   
unsigned char zapis[14];
 
index=0+m*2; 
t_i=  ( ( unsigned long int)c2_byte.mb[index] + ( unsigned long int)(c2_byte.mb[index+1] & 7)*256);
if (t_i > 1440) return(0);
if (t_i==0) return(0);
                       


                        
                      RdFromFleshToArr(STRUCT_TII+m*14,&zapis[0],14); 
                     
                     /*
                      //proverka
                     
                      __disable_interrupt(); 
                      UCSR2B=0; 
                      UBRR2H = R115200_H;
                      UBRR2L = R115200_L;     
                      UCSR2B=UCSR2B | TXEN;
                      s_port('r');
                      s_port('d');
                      s_port(' ');
                      s_port('f');
                      s_port('l');
                      s_port('a');
                      s_port('s');
                      s_port('h');
                      s_port(':');
                      s_port(zapis[0]);
                      s_port(zapis[1]);
                      s_port(zapis[2]);
                      s_port(zapis[3]);
                      s_port(zapis[4]);
                      s_port(zapis[5]);
                      s_port(zapis[6]);
                      s_port(zapis[7]);
                      s_port(zapis[8]);
                      s_port(zapis[9]);
                      s_port(zapis[10]);
                      s_port(zapis[11]);
                      s_port(zapis[12]);
                      s_port(zapis[13]);
                      */
                      
                      if((*(int*)&zapis[12])==crc_sum(&zapis[0],12)) 
                                        {     
                                           tik.st[0]=zapis[0];
                                           tik.st[1]=zapis[1];
                                           tik.st[2]=zapis[2];
                                           tik.st[3]=zapis[3];
                                           
                                           /*
                                           S3_RD;//proverka
                                            s_port('r');
                                            s_port('d');
                                            s_port(' ');
                                            s_port('t');
                                            s_port('i');
                                            s_port('k');
                                            s_port(':');
                                           
                                           s_port(tik.st[0]);
                                           s_port(tik.st[1]);
                                           s_port(tik.st[2]);
                                           s_port(tik.st[3]);
                                           s_port('e');
                                           s_port('n');
                                           s_port('d');
                                             */
                                           
                                           
                                          if( tik.unix==timer_tii[m])
                                                                      {
                                                                      tik.st[0]=zapis[6];
                                                                      tik.st[1]=zapis[7];
                                                                      tik.st[2]=zapis[8];
                                                                      tik.st[3]=zapis[9];
                                                                   //   err_cnt[m]=1;
                                                                    
                                                                      return(tik.unix);  // пишем в тот же архив
                                                                      
                                                                      }
                                          
                                            
                                           
                                          else
                                                          {
                                                      //    err_cnt[m]=2;                                         
                                                          return(0);  // создаем новый архив 
                                                          }
                                          }
                       else  
                       
                                                         {
                                                      //    err_cnt[m]=3;                                         
                                                          return(0);  // архив неправильный 
                                                          }      
                                                       
/////////////////////////////////
}                  
 //////////           



 void store_tii(void)  // записать в архив значения 1 минутные
 {
 char index,m;
 unsigned char zapis[14];
 
 unsigned int t_i,te; 
 
  union
   {     
   long int unix;
   char st[4]; 
   } tik;
   
 te=tii_bit_minut;   
 if (te==0) return;   
 
 if( te & 0x8000) { m=15;goto zap_tii; }
 if( te & 0x4000) { m=14;goto zap_tii; }
 

 if(sel_modul==1)
 {
   for (m=0; m < 5;m++)
          {
          if( te & 1)goto zap_tii; 
                          
           te=te >> 1;               
          }
 }
 else
 {
 for (m=0; m < 8;m++)
          {
          if( te & 1)goto zap_tii; 
                          
           te=te >> 1;               
          }
 
 }
 
zap_tii:               
 
      
te=  1<<m;
                          
tii_bit_minut= tii_bit_minut & ~te; 
 
index=0+m*2; 
t_i=  ( ( unsigned long int)c2_byte.mb[index] + ( unsigned long int)(c2_byte.mb[index+1] & 7)*256);
if (t_i > 1440) return;
if (t_i==0) return;

                      
                                        {
                                        tik.unix=timer_tii[m];        
     
                                         zapis[0]=tik.st[0];
                                         zapis[1]=tik.st[1];
                                         zapis[2]=tik.st[2];
                                         zapis[3]=tik.st[3];
                                             
 
  
    __disable_interrupt();
  tik.unix=(arr_tii_32[m ]- start_ttii[m]);
   __enable_interrupt();
  
                                         zapis[6]=tik.st[0];
                                         zapis[7]=tik.st[1];
                                         zapis[8]=tik.st[2];
                                         zapis[9]=tik.st[3];
  
  
  

  
  zapis[4]=t_i;
  zapis[5]=(t_i >>8);  
  
  zapis[10]=m;
  zapis[11]=0;  
                                     }
 *(int*)&zapis[12]=crc_sum(&zapis[0],12);
  

//send_info(sizeof(prov1),prov1,0,0);
   
WrArrayToFlesh(STRUCT_TII+m*14,&zapis[0],14,0,0);            

                     /*
                     //proverka
                      S4_CH;
                      UCSR2B=0; 
                      UBRR2H = R115200_H;
                      UBRR2L = R115200_L;     
                      UCSR2B=UCSR2B | TXEN;
                      s_port('w');
                      s_port('r');
                      s_port(' ');
                      s_port('f');
                      s_port('l');
                      s_port('a');
                      s_port('s');
                      s_port('h');
                      s_port(':');
                      s_port(zapis[0]);
                      s_port(zapis[1]);
                      s_port(zapis[2]);
                      s_port(zapis[3]);
                      s_port(zapis[4]);
                      s_port(zapis[5]);
                      s_port(zapis[6]);
                      s_port(zapis[7]);
                      s_port(zapis[8]);
                      s_port(zapis[9]);
                      s_port(zapis[10]);
                      s_port(zapis[11]);
                      s_port(zapis[12]);
                      s_port(zapis[13]);
                      */

/////////////////////////////////
}                  
 //////////
 
 
 
 void store_tii_lock(char m)  // записать в архив значения 1 минутные
 {
 char index;
 unsigned char zapis[14];
 unsigned int t_i,te; 
 
  union
   {     
   long int unix;
   char st[4]; 
   } tik;
   
 
 

 
index=0+m*2; 
t_i=  ( ( unsigned long int)c2_byte.mb[index] + ( unsigned long int)(c2_byte.mb[index+1] & 7)*256);
if (t_i > 1440) return;
if (t_i==0) return;

                      
                                        {
                                        tik.unix=timer_tii[m];        
     
                                         zapis[0]=tik.st[0];
                                         zapis[1]=tik.st[1];
                                         zapis[2]=tik.st[2];
                                         zapis[3]=tik.st[3];
                                             
 
 __disable_interrupt();
  tik.unix=(arr_tii_32[m ]- start_ttii[m]);
  __enable_interrupt();
                                         zapis[6]=tik.st[0];
                                         zapis[7]=tik.st[1];
                                         zapis[8]=tik.st[2];
                                         zapis[9]=tik.st[3];
  
  // записываем в архив либо замер импульсов либо мотоминуты
  
                        tii_moto.config=*(unsigned int*)&c2_byte.mb[m<<1];
                        if (tii_moto.bit.type_zam==1)  { *(unsigned long int*)&zapis[6]=cnt_moto[m];   }
                         *(unsigned int*)&zapis[4]=t_i; 
                        
  
  
  //zapis[4]=t_i;
  //zapis[5]=(t_i >>8);  
  
  zapis[10]=m;
  zapis[11]=0;  
                                     }
  
  

 te=  1<<m;
                          
tii_bit_minut= tii_bit_minut & ~te;         
                          
                                                   
*(int*)&zapis[12]=crc_sum(&zapis[0],12);     
WrArrayToFlesh(STRUCT_TII+m*14,&zapis[0],14,0,0);   


 // записать в общую кучу



WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);



 
}                  
 ////////// 


 void store_tit_lock(char m)  // записать в архив значения 1 минутные
 {
 char index;
 unsigned char zapis[14];
 unsigned int t_i; 
 
  union
   {     
   long int unix;
   char st[4]; 
   } tik;
   
 
 

 
index=0x40+m*2; 
t_i=  ( ( unsigned long int)c2_byte.mb[index] + ( unsigned long int)(c2_byte.mb[index+1] & 7)*256);


                      
                                        {
                                        tik.unix=timer_tit[m];        
     
                                         zapis[0]=tik.st[0];
                                         zapis[1]=tik.st[1];
                                         zapis[2]=tik.st[2];
                                         zapis[3]=tik.st[3];
                                             

                                         zapis[6]=modbus_mem1[AD_TIT1+m];
                                         zapis[7]=modbus_mem1[AD_TIT1+m]>>8;
                                         zapis[8]=0;
                                         zapis[9]=0;
  
  
  

  
  
  zapis[4]=t_i;
  zapis[5]=(t_i >>8);  
  
  zapis[10]=m+80;
  zapis[11]=0;  
                                     }
  
  

     
                          
                                                   
*(int*)&zapis[12]=crc_sum(&zapis[0],12);     
//WrArrayToFlesh(STRUCT_TII+m*14,&zapis[0],14,0,0);   26 01 2015


 // записать в общую кучу



WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);



 
}                  
 ////////// 



void lock_arh_tit(void)  // записать в архив значения 
 {
 char m;
 unsigned char i;

 unsigned int te; 
 
   
 te=tit_bit;   
 if (te==0) return; 
 
 
 
  
 for (m=0; m < 2;m++)
          {
          if( te & 1)goto zap_titx; 
                          
           te=te >> 1;               
          }
 return;
  
zap_titx:   
     

  
  
    te=  1<<m;
                          
tit_bit= tit_bit & ~te;

  //          send_info(sizeof(prov2),prov2,0,0);

            store_tit_lock(m);    // записать результат
           
           
             
            
            
            /*
            arr_cnt_tii_heap++; 
           if ( arr_cnt_tii_heap >99) arr_cnt_tii_heap=0;
           i=arr_cnt_tii_heap; */
           
           modbus_mem1[AD_IND_DNS]++;
           if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}
           i=modbus_mem1[AD_IND_DNS];
          
           
           
           
           WrArrayToFlesh(A_IND_DNS,0,1,1,i);
           //WrArrayToFlesh(A_IND_DNS+1,0,1,1,0);
  
         
           
            end_zamer_tit(m);    // вычислить время окончания замера
           
       
           
                   
  } 


                   
void lock_arh(void)  // записать в архив значения 
 {
 char m;
 unsigned char i;

 unsigned int te; 
 
   
 te=tii_bit;   
 if (te==0) return; 
 
 
 if( te & 0x8000) { m=15;goto zap_tiix; }
 if( te & 0x4000) { m=14;goto zap_tiix; }
 
  if(sel_modul==1)
  {
    for (m=0; m < 5;m++)
          {
          if( te & 1)goto zap_tiix; 
                          
           te=te >> 1;               
          }
  }
  else
  {
 for (m=0; m < 8;m++)
          {
          if( te & 1)goto zap_tiix; 
                          
           te=te >> 1;               
          }
  }
zap_tiix:   
     

  
  
    te=  1<<m;
                          
tii_bit= tii_bit & ~te;

  //          send_info(sizeof(prov2),prov2,0,0);

            store_tii_lock(m);    // записать результат
           
           
             
            
            
            /*
            arr_cnt_tii_heap++; 
           if ( arr_cnt_tii_heap >99) arr_cnt_tii_heap=0;
           i=arr_cnt_tii_heap; */
           
           modbus_mem1[AD_IND_DNS]++;
           if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}
           i=modbus_mem1[AD_IND_DNS];
          
           
           
           
           WrArrayToFlesh(A_IND_DNS,0,1,1,i);
           //WrArrayToFlesh(A_IND_DNS+1,0,1,1,0);
  
         
           
            end_zamer(m);    // вычислить время окончания замера
           
 __disable_interrupt();
     start_ttii[m]=arr_tii_32[m ]; // начать новый  заме               
  __enable_interrupt();            
           
                   
  } 
 
// ***************************************************процедуры ГЗУ
  char test_nenull(void)
    { 
    char c,i,index;
    int t_i; 
   c=0;
   for (i=0;i<c1_byte.mb[1*2];i++)
                                                            {
                                                            index=6*2+i*2;
                                         t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256;
                                                             if ( t_i !=0) c++;
                                                                         
                                                            }
     return(c);                                                            
     }
     
                                                            
char test_switch_psm(void)
{
 if (c1_byte.mb[1*2]==8) 
                                       {                                   
                                       if ( pred_mask==8)
                                               { 
                                                  if( ((modbus_mem1[AD_TS]>> 4) &0xf) !=1 ) return(0);
                                                  return(1);
                                               }
                                       if ( (pred_mask+1) != ((modbus_mem1[AD_TS]>> 4) &0xf) ) return(0);        
                                       return(1);
                                                    
                                       } 
////////////////
  if (c1_byte.mb[1*2]==14) 
                                       {                                   
                                       if ( pred_mask==14)
                                               { 
                                                  if( ((modbus_mem1[AD_TS]>> 4) &0xf) !=1 ) return(0);
                                                  return(1);
                                               }
                                       if ( (pred_mask+1) != ((modbus_mem1[AD_TS]>> 4) &0xf) ) return(0);        
                                       return(1);
                                                    
                                       }
 return(0);                                                                             
}




  
  ////////////////////////////////////////
void store_ys_minut(void)  // записать в архив значения за минуту
 {
 char index;
unsigned int t_i;
union
   {     
   long int unix;
   char st[4]; 
   } tik;     
unsigned char zapis[14];  
       
           
           
           bit_zamer_psm=0;

                                        
                                        tik.unix=unix_psm;        
     
                                         zapis[0]=tik.st[0];
                                         zapis[1]=tik.st[1];
                                         zapis[2]=tik.st[2];
                                         zapis[3]=tik.st[3];
                                           
     __disable_interrupt();
  t_i=delta_ys+(arr_tii_32[c1_byte.mb[4*2] ]- start_tii_gzu)/2;
   __enable_interrupt(); 
  zapis[6]=t_i;
  zapis[7]=t_i >>8; 
  
                                        
  
//////    
 index=6*2+( tekpsm-1)*2;    
  
  t_i=  (( ( unsigned long int)c1_byte.mb[index] + ( unsigned long int)c1_byte.mb[index+1]*256)); // реальное время замера
  zapis[4]=t_i;
  zapis[5]=t_i >>8;       
  
  zapis[8]=tekpsm;
  zapis[9]=0; 
  
      
                          
                                                   
*(int*)&zapis[10]=crc_sum(&zapis[0],10);     
WrArrayToFlesh(STRUCT_US  + modbus_mem1[AD_IND_GZU]*12,&zapis[0],12,0,0);
  
 
            bit_zamer_psm=0; 
           
 }
///////////////////////////           
void store_ys_lock(void)  // записать в архив значения 
 {
 char index;
unsigned int t_i;
union
   {     
   long int unix;
   char st[4]; 
   } tik;     
unsigned char zapis[14]; 
  
                   
           bit_zamer_psm=0;

                                        
                                        tik.unix=unix;    //  unix_psm;   27 06 06     
     
                                         zapis[0]=tik.st[0];
                                         zapis[1]=tik.st[1];
                                         zapis[2]=tik.st[2];
                                         zapis[3]=tik.st[3];
                                           
    __disable_interrupt();
  t_i=delta_ys+(arr_tii_32[c1_byte.mb[4*2] ]- start_tii_gzu)/2;
    __enable_interrupt(); 
  zapis[6]=t_i;
  zapis[7]=t_i >>8; 
  
                                        
  
//////    
 index=6*2+( tekpsm-1)*2;    
 //  t_i=global_count_psm/60;
 //  if ( (global_count_psm%60) >20 ) t_i++;
   
  t_i=  (( ( unsigned long int)c1_byte.mb[index] + ( unsigned long int)c1_byte.mb[index+1]*256)); 
  t_i=t_i- (timer_psm/60);            // реальный интервал замера 07 02 2013
  if ( t_i==0) t_i=1;
  
  zapis[4]=t_i;
  zapis[5]=t_i >>8;  
  
   zapis[8]=tekpsm;
  zapis[9]=0; 
  
                                                     
*(int*)&zapis[10]=crc_sum(&zapis[0],10);     
WrArrayToFlesh(STRUCT_US  + modbus_mem1[AD_IND_GZU]*12,&zapis[0],12,0,0);
bit_zamer_psm=0;
             
           
 }
///////////////////////////   
   
   ////////////////////////////////////  
  
 void store_ys(void)  // записать в архив значения
 {
   char i;
 
           store_ys_lock();
           
      
           
           
            modbus_mem1[AD_IND_GZU]++;
           if ( modbus_mem1[AD_IND_GZU] >54 ) {modbus_mem1[AD_IND_GZU]=0;}
           i=modbus_mem1[AD_IND_GZU];
          
           
           
           
           WrArrayToFlesh(A_IND_GZU,0,1,1,i);
           
          
 }

////////////////////////// автомат состояний ГЗУ
void local_psm(void) 
{     
   char  c,index;
   unsigned int t_i;
   union
   {     
   long int unix;
   char st[4]; 
   } tik;  
  unsigned char zapis[14];
  
  
  
   if (c1_byte.mb[0]!=0x55){ err_psm=1;sost_psm=1;return;} 
  
               
   // переключение  до заданного усане зависимо от положения ручной автомат
   
   if (  (zadan_ys > 0) && (zadan_ys <15)  )
           {
           fin_ys=zadan_ys; 
           zadan_ys=0; 
           if ( tekpsm !=fin_ys)
                                {
                                if ( sost_psm==4) store_ys();  
                                global_try=3; 
                                count_sboj=15;
                                sost_psm=9;
                                return;
                                }
            global_try=3;
           }
           
           
           
  
                  
      
                        
               
switch (sost_psm) 
   {              
      case 1:
              {     
              
                  if (c1_byte.mb[1*2]==8) 
                                       {
                                        c=((modbus_mem1[AD_TS]>> 4) &0xf);
                                        if ( (c==0) | (c > 8) ) 
                                                        {
                                                        err_psm=1;
                                                        break;
                                                        } 
                                        tekpsm=c;  
                                        err_psm=0;               
                                        sost_psm=2;
                                        break;                
                                       }
                  if (c1_byte.mb[1*2]==14) 
                                       {
                                        c=((modbus_mem1[AD_TS]>> 4) &0xf);
                                        if ( (c==0) | (c > 14) ) 
                                                        {
                                                        err_psm=1;
                                                        break;
                                                        }
                                        sost_psm=2; 
                                        err_psm=0;
                                        tekpsm=c;   
                                        break;                
                                       } 
                  break;                      
              }         // end case 1
              
           case 2:               // переход на  коррекцию
                     {                                    
                     
                     
                                                      
                      if (test_nenull()==0)  {err_psm=1; sost_psm=1;break;} 
                      timer_psm=(unsigned long int)c1_byte.mb[2*2]*60;
                      index=6*2+( tekpsm-1)*2;
                                         t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256;
                                        
                                         
                                       if ( t_i==0) 
                                                    {
                                                   if  ( c1_byte.mb[5*2]==0) {err_psm=1;sost_psm=1;break;}  // ручной  
                                                    else {cnt_try_ys=3;sost_psm=5;break;}   // автомат
                                                    }
                                       
                                       
                      sost_psm=3;
                     break;
                     }   
           case 3:               // ожидание коррекции
                     {     
                      if (timer_psm==0) 
                                        {
                                         sost_psm=4;  // постановка на замер  
                                         
                                         start_tii_gzu=arr_tii_32[c1_byte.mb[4*2] ];// запомнить значение тии
                                        
////////////////////////
RdFromFleshToArr(STRUCT_US + modbus_mem1[AD_IND_GZU]*12,(unsigned char *)&zapis[0],12);                                         
 if((*(int*)&zapis[10])==crc_sum(&zapis[0],10))                                          

            
                                        {     
                                           tik.st[0]=zapis[0];
                                           tik.st[1]=zapis[1];
                                           tik.st[2]=zapis[2];
                                           tik.st[3]=zapis[3];
                                           if ( zapis[8] !=tekpsm ) goto new_zamer;
                                          if( unix >tik.unix) goto new_zamer;
                                          index=6*2+( tekpsm-1)*2;
                                          t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256; 
                                          if ( (tik.unix-unix) > ((unsigned long int)t_i*60) ) goto new_zamer; //..
                                          timer_psm= (tik.unix-unix);
                                          delta_ys=zapis[6]+(zapis[7]*256); // то что уже намеряно
                                          unix_psm=tik.unix; // время конец замера из архива
                                          
   
                                        } 
                       else        // перезаписать п о новой 
                                         {  
                              new_zamer:
                                          index=6*2+( tekpsm-1)*2;
                                          t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256;
                                          timer_psm= (unsigned long int)t_i*60;
                                          unix_psm=unix+((unsigned long int)t_i*60); // время конец замера   //..
                                          bit_zamer_psm=0;
                                          delta_ys=0;
                                        
                                         } 
////////////////////////                                         
                                         
                                        
                                        }
                      if( ((modbus_mem1[AD_TS]>> 4) &0xf) != tekpsm ) sost_psm=1;
                     break;
                     }       
              
           case 4:        // замер
                    {                   
                    if ( bit_zamer_psm==1)  store_ys_minut();
                    if ( bit_level_psm==1)  
                                               {
                                               bit_level_psm=0; 
                                         // store_ys();            // 26 06 06 начать замер по новой
                                          sost_psm=1; break;
                                               } 
                    if (unix >= unix_psm)        
                     //if (timer_psm==0)
                                          {  
                                          
                                          store_ys();
                                          if ( c1_byte.mb[5*2]==0) {sost_psm=1; break;}
                                          else 
                                               {
                     // если это единственный разрешенный ус                   
                                                       c=test_nenull();
                                                      
                                                     if (c==0)  { sost_psm=1;break;}   // нет ненулевых усов
                                                     if (c==1)
                                                                 {
                                                               
                                                                   // тот ус на котором я стою единственный
                                                            index=6*2+(tekpsm-1)*2;
                                         t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256;
                                                             if ( t_i !=0) { sost_psm=1;break;}
                                                                         
                                                             
                                                                 }           
                                                       
                                                      
                                                if (global_try==0) { err_psm=1;sost_psm=1;break;}
                                               cnt_try_ys=3;                                                     
                                               sost_psm=5; 
                                                 break;
                                               } // переключится
                                          }
                      if( ((modbus_mem1[AD_TS]>> 4) &0xf) != tekpsm ) {global_try=3;store_ys();sost_psm=1;}
                    break;
                    }   
           case 5:             // импульс ту
                    {
////////////////////                              
               c=test_nenull();
                                                      
                                                     if (c==0)  { sost_psm=1;break;}   // нет ненулевых усов
                                                     if (c==1)
                                                                 {
                                                               
                                                                   // тот ус на котором я стою единственный
                                                            index=6*2+(tekpsm-1)*2;
                                         t_i=  c1_byte.mb[index] + c1_byte.mb[index+1]*256;
                                                             if ( t_i !=0) { sost_psm=1;break;}
                                                                         
                                                             
                                                                 }    
////////////////////                  
                    if (global_try==0) { err_psm=1;sost_psm=1;break;} 
                    pred_mask=tekpsm;
                    TU1_ON;
                    timer_psm=(unsigned long int)c1_byte.mb[3*2]; 
                    sost_psm=6;
                    break;
                    }   
              
           case 6:
                   {     
                    if (timer_psm==0) 
                                    { 
                                    TU1_OFF; 
                                              if ( c1_byte.mb[5*2]==0) {sost_psm=1;break;}
                                     
                                     timer_psm=120;   // 2 минуты ждем
                                     sost_psm=7; 
                                    }
                   break;
                   }
          case 7:       // ожидание 2 минуты
                    {     
                     if (timer_psm==0) 
                                        {
                                        if( ((modbus_mem1[AD_TS]>> 4) &0xf) == pred_mask )
                                                                {
                                                                   timer_psm=60;
                                                                   sost_psm=8;  
                                                                   break;
                                                                } 
                                                                global_try=3;
                                           if (test_switch_psm()==1)
                                                            { 
                                                            err_switch=0;
                                                            sost_psm=1;
                                                            break; 
                                                            }            
                                                           
                                           err_switch=1;                 
                                           sost_psm=1;                     
                                           break;
                                        }
                    break;
                    }
           
         case 8:     // ожидание 60 секунд
                {     
                 if (timer_psm==0) 
                                        {
                                         if (test_switch_psm()==1)
                                                            { 
                                                            err_switch=0;
                                                            sost_psm=1;
                                                            break; 
                                                            } 
                                                            if( ((modbus_mem1[AD_TS]>> 4) &0xf) == pred_mask )
                                                                 { 
                                                                   if (cnt_try_ys ==0) 
                                                                               {
                                                                              err_switch=1;                 
                                                                              sost_psm=1;
                                                                              if (global_try !=0) global_try--;
                                                                              break;    
                                                                                }
                                                            else cnt_try_ys--;    
                                                           sost_psm=5;   // попытаться переключиться 3 раза      
                                                            break;                          
                                                                  }
                              // переключился неправильно                                    
                                            global_try=3;
                                            err_switch=1;
                                            sost_psm=1;
                                            break;                      
                                         
                                         
                                        }
                break;
                }

////////////////////////////////////////////////
// для преключения до заданного уса
case 9:
         { 
         pred_mask=tekpsm;
                    TU1_ON;
                    timer_psm=(unsigned long int)c1_byte.mb[3*2]; 
                    sost_psm=10; 
         break;
         }  
case 10:
         {
         if (timer_psm==0) 
                                    {
                                     TU1_OFF;
                                     timer_psm=120;
                                     sost_psm=11; 
                                    }
         break;
         } 
case 11:
         { 
         if (timer_psm==0) 
                                        { 
                                       tekpsm=((modbus_mem1[AD_TS]>> 4) &0xf); 
                                       if ( tekpsm==fin_ys) {sost_psm=1;err_switch=0;break;}
                                       count_sboj--;
                                       
                                       
                                       if (  count_sboj==0 )
                                      
                                                            { 
                                                            err_switch=1;
                                                            sost_psm=1;
                                                            break; 
                                                            }
                                                          
                                           sost_psm=9;               
                                           break;
                                        } 
         break;
         }                                
   default: {sost_psm=1;break;}
   }                     
}    


// процедуры ГЗУ 



/////////////////////////////////////////////***********************
// пытаемся найти архивную запись с заданными параметрами в стеке архивов 
char try_find_arhiv(unsigned long int matrix,char m)  
{
  
 unsigned char tek_ind,i;
  union
   {     
   unsigned long int unix;
   char st[4]; 
   } tik;
  
  unsigned char zapiska[14];
   
    tek_ind=modbus_mem1[AD_IND_DNS]; // текущий индекс архива
    if(tek_ind==0) tek_ind=99; // берем предыдущий
    else tek_ind--;
    
    for (i=0; i < 16;i++) {
                       /*
                       p_sys1=( char *) & zapiska[0];// point to arr zapis
                       sys1= STRUCT_TII_HEAP + (int)tek_ind*14;
                       
                       sys2=12;  
                       sys3=sys1+12; 
              
                       w_result(22);
                       */
      
                       RdFromFleshToArr(STRUCT_TII_HEAP + (int)tek_ind*14,&zapiska[0],14); 
                       if((*(int*)&zapiska[12])==crc_sum(&zapiska[0],12))
                       // if (ret_cod==0)
                                        {     
                                        tik.st[0]=zapiska[0];
                                          tik.st[1]=zapiska[1];
                                           tik.st[2]=zapiska[2];
                                           tik.st[3]=zapiska[3];
                                      
                                         if( ( matrix== tik.unix) && (zapiska[10]==m) )  return(1); // есть такая запись !
                                                                      
                                        }  
                                            
                                           
                        if(tek_ind==0) tek_ind=99; // берем предыдущий
                        else tek_ind--;               
                       
                       
                       
                       
                       
                       } // цикл for
                       
                 return(0);  // не нашли записи
}
  
// попытаться сформировать архивы, которые не сформировались по причине пропадания питания, по последним минутным замерам  
//////////////////
// прочитать сначала архивы на предмет действительности
 char try_form_arhiv(char m)  // 
 {
 

 
  union
   {     
   unsigned long int unix;
   char st[4]; 
   } tik;
  
  unsigned char zapis[14],i;
   
               /*
                       p_sys1=( char *) & zapis[0];// point to arr zapis
                sys1= STRUCT_TII + (int)m*0x100 + (int)arr_cnt_tii[m]*14;
                       
                       sys2=12;  
                       sys3=sys1+12; 
            // system_zap=22;   
                       w_result(22);
                 */      
                       RdFromFleshToArr(STRUCT_TII+m*14,&zapis[0],14);
                     
            //  если возвратило 0- значит правильно считали
            
                       // считали правидьно
                        if((*(int*)&zapis[12])==crc_sum(&zapis[0],12))   
                      // if (ret_cod==0)
                                        {     
                                        tik.st[0]=zapis[0];
                                          tik.st[1]=zapis[1];
                                           tik.st[2]=zapis[2];
                                           tik.st[3]=zapis[3];
                                      
                                          if( timer_tii[m] > tik.unix)   // это просроченный минутный архив
                                                                      {
                                                                 if( try_find_arhiv(tik.unix,m)==1) return(1);   // пытаюсь найти в стеке архивов запись с этим временем
                                                                      // не нашли архив,
                                                                      // последний минутный архив записать в общую кучу
                                                                      // это будет принудительно сфорировааный архив
                                                                 /*
                p_sys1=( char *) & zapis[0];// point to arr zapis
                sys1= STRUCT_TII_HEAP + (int)arr_cnt_tii_heap*14;
             sys2=12;  
             sys3=sys1+12; 
            // system_zap=24;  
            w_result(24);
            
            arr_cnt_tii_heap++; 
            if ( arr_cnt_tii_heap >99) {arr_cnt_tii_heap=0;arr_cnt_tii_heap_modbus=99;}
                                         else arr_cnt_tii_heap_modbus=arr_cnt_tii_heap-1;
                     
           
           
           write_ext_flesh( CNT_TII,arr_cnt_tii_heap);
           */
          ///////////////////////////
           WrArrayToFlesh(STRUCT_TII_HEAP + (int)modbus_mem1[AD_IND_DNS]*14,&zapis[0],14,0,0);      
     modbus_mem1[AD_IND_DNS]++;
     if ( modbus_mem1[AD_IND_DNS] >99 ) {modbus_mem1[AD_IND_DNS]=0;}          
     i=modbus_mem1[AD_IND_DNS];     
     WrArrayToFlesh(A_IND_DNS,0,1,1,i);
              
                                                                      
                                                                      
                                                                      }
                                                                                     
                                          
                                         
                                          }
                     
return(0);                                                       
/////////////////////////////////
}                  
 //////////        
 
////////////////////////////////////////////////////////////////////////////////////////

void start_a(void)  //YES!!!!
{  
char m;       

for (m=14; m < 16;m++) 
      {
      end_zamer(m); 
      start_ttii[m]=arr_tii_32[m]-read_arhiv_tii(m); // начать новый минутный заме   
      try_form_arhiv(m); // пытаемся сформировать утерянные архивы
     }
//при старте все времена окончания замеров установить

if(sel_modul==1)
  {
    for (m=0; m < 5;m++) 
      {
      end_zamer(m); 
      start_ttii[m]=arr_tii_32[m]-read_arhiv_tii(m); // начать новый минутный заме      
      try_form_arhiv(m); // пытаемся сформировать утерянные архивы
     }
  }
else
{
for (m=0; m < 8;m++) 
      {
      end_zamer(m); 
      start_ttii[m]=arr_tii_32[m]-read_arhiv_tii(m); // начать новый минутный заме 
      try_form_arhiv(m); // пытаемся сформировать утерянные архивы
     }
}
   lock_bit=0;  
time_sutok=set_time_day_zamer(real_time.r_date,real_time.r_month,real_time.r_year);
      if ( time_sutok <= unix) time_sutok+=86400; 
      for (m=0; m < 2;m++) end_zamer_tit(m);
} 




void refresh_a(char m)
{ 
  time_sutok=set_time_day_zamer(real_time.r_date,real_time.r_month,real_time.r_year);
      if ( time_sutok <= unix) time_sutok+=86400;   
  if ( m==0x20 ) { end_zamer_tit(0);return; }
  if ( m==0x21 ) { end_zamer_tit(1);return; }
 
  if( (m==0xe)  || (m==0xf) ) goto lk1j;
  
  if (sel_modul==1){if (m >=5) return;}
  else {if (m >=8) return;}
  
lk1j:
  
      end_zamer(m); 
     
     __disable_interrupt();
     start_ttii[m]=arr_tii_32[m ];    //    -read_arhiv_tii(m); // начать новый минутный заме 
     __enable_interrupt();
   
   
} 
                                        
 
 void start_aa(void)
{  
char m;       
time_sutok=set_time_day_zamer(real_time.r_date,real_time.r_month,real_time.r_year);
      if ( time_sutok <= unix) time_sutok+=86400; 
//при смене времени все времена окончания замеров установить но начальное значение тии не устанавливат
if(sel_modul==1){for (m=0; m < 5;m++) end_zamer(m);}
else{ for (m=0; m < 8;m++) end_zamer(m);}
for (m=0; m < 2;m++) end_zamer_tit(m);
for (m=14; m < 16;m++) end_zamer(m); 
      
      
}                                          


  
  /////////////////////////////////                     


          
   

/////////////////////////////////////////////////////////
/* 
void local_psm(void) 
{     


  if (c2_byte.mb[38]!=0xaa) return; 
  if (c2_byte.mb[39]!=0) return;
  
  if ( bit_level==1) {bit_level=0;start_aa();} 
  
  store_tii();
  lock_arh();
  
  
}



void energy_plus(void)
{
char k;

if(cnt_sec<DEF_CNT_SEC)return;
cnt_sec=0;

burst_ds_r();

  for (k=0;k<8;k++)
                        { 
                        if (timer_tii[k] ==0) continue;
                        if (lock_bit & (1<<k) ) continue;
                        if (unix >=timer_tii[k] )      // если ожидаемое время больше текущего
                                                  { 
                                                   tii_bit=tii_bit | (1<<k);
                                                   lock_bit=lock_bit | (1<<k);
                                                   }
                                           
                        }
     

tarhiv++;
           if (tarhiv>=60)
                   { 
                   tii_bit_minut=0xff;
                    tarhiv=0;
                   }
  
   local_psm();
   
      }
   */
 

void energy_plus(void)
{
char k,max;
union
  {  int data;
struct
   {     
     int psm_sost   :8 ;
     int err_switch     :4 ;
     int err_psm    :4 ;
   
   } sost;
  } mb; 

//if (c2_byte.mb[38]!=0xaa) return; 
//if (c2_byte.mb[39]!=0) return;



if(cnt_sec<DEF_CNT_SEC)return;
cnt_sec=0;


bpa_avtomat();

burst_ds_r();

if(sel_modul==1)max=5;else max=8;

if(timer_psm !=0) timer_psm--;    // время до переключения отсчитывается в секундах !!! // для ГЗУ

  for (k=0;k<max;k++)
                        { 
                        if (timer_tii[k] ==0) continue;  
                        if (lock_bit & (1<<k) ) continue;
                        // условный счет
                        tii_moto.config=*(unsigned int*)&c2_byte.mb[k<<1];
                        if (  (tii_moto.bit.type_zam==1)  && ((modbus_mem1[AD_TS] >> k) & 1 ) ) cnt_moto[k]++;
                             /*
                             {
                               if ( old_realsec[k] <= real_time.r_sec)   cnt_moto[k]+=real_time.r_sec-old_realsec[k];
                               else  cnt_moto[k]+=60 + real_time.r_sec-old_realsec[k];
                             }
                        old_realsec[k]=real_time.r_sec;
                        */
                        
                        // условный счет
                        if (unix >=timer_tii[k] )      // если ожидаемое время больше текущего
                                                  { 

                                                   tii_bit=tii_bit | (1<<k);
                                                   lock_bit=lock_bit | (1<<k);
                                                   }
                                           
                        }
  
  for (k=14;k<16;k++)
                        { 
                        if (timer_tii[k] ==0) continue;  
                        if (lock_bit & (1<<k) ) continue;
                        // условный счет
                        tii_moto.config=*(unsigned int*)&c2_byte.mb[k<<1];
                         if (  (tii_moto.bit.type_zam==1)  && ((modbus_mem1[AD_TS] >> k) & 1 ) ) cnt_moto[k]++;
                        /*
                        if (  (tii_moto.bit.enable==1)  &&
                              (((modbus_mem1[AD_TS] >> tii_moto.bit.nomer_tc) & 1 )==tii_moto.bit.level_tc ) ) 
                        
                       {
                               if ( old_realsec[k] <= real_time.r_sec)   cnt_moto[k]+=real_time.r_sec-old_realsec[k];
                               else  cnt_moto[k]+=60 + real_time.r_sec-old_realsec[k];
                             }
                        old_realsec[k]=real_time.r_sec;
                        */
                        
                        // условный счет
                        if (unix >=timer_tii[k] )      // если ожидаемое время больше текущего
                                                  { 

                                                   tii_bit=tii_bit | (1<<k);
                                                   lock_bit=lock_bit | (1<<k);
                                                   }
                                           
                        }
  
  
     
//
  // для замеров ТИТ
  //
  for (k=0;k<2;k++)
                        { 
                        if (timer_tit[k] ==0) continue;
                        if (lock_bit_tit & (1<<k) ) continue;
                        if (unix >=timer_tit[k] )      // если ожидаемое время больше текущего
                                                  { 
                                                   tit_bit=tit_bit | (1<<k);
                                                   lock_bit_tit=lock_bit_tit | (1<<k);
                                                   //timer_tii[k]=0;
                                                   }
                                           
                        }
  
  
  
tarhiv++;
           if (tarhiv>=60)
                   { 
                   if( sost_psm==4) bit_zamer_psm=1;  // для ГЗУ
                   tii_bit_minut=0xffff;
                   tarhiv=0;
                   }
 
          if( *(unsigned int*)&c2_byte.mb[38]==0xaa) 

          {
  if ( bit_level==1) {bit_level=0;start_aa();} 
  store_tii();
  lock_arh();  
  lock_arh_tit(); 
  
  if (*(unsigned int*)&c2_byte.mb[46]==1) day_zamer();
          }
  
  if(sel_modul!=1) {
                         local_psm();
                        
                        mb.sost.psm_sost=sost_psm;
                        mb.sost.err_switch=err_switch;
                        mb.sost.err_psm=err_psm;
                        modbus_mem1[AD_SOST_PSM]=mb.data;
                        modbus_mem1[AD_TIMER_PSM]=timer_psm;
                        
                        
     
                   } // только в режиме МТС
  
  


  }  

 
  
  
  
  
  



