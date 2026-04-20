
#include <iom2560.h>
#include <inavr.h>
#include "dfcnst.h"
#include "map_ef.h"
#include "map_mbus.h"
#include "arh_zap.h"

#include "dfpin.h"





 void com_disp_mbus (void);

void WrArrayToFlesh(unsigned int adres_flesh,  unsigned char *adres_ozu, unsigned int num,unsigned char flag,unsigned char znach);  //запись конфигурации кп во флеш
void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,unsigned int num);

void insert_zapis_kd(unsigned char nomer,unsigned char type);

unsigned char crc_485(unsigned char num, unsigned char *p);

unsigned long int burst_ds_r(void);

extern unsigned char sel_modul;

extern unsigned int modbus_mem1[SEG1];


extern char flag_arhiv_prihod;

extern unsigned char flag_set_akb;


//extern unsigned char zapis[14];



extern unsigned char prihod_uhod;

unsigned char cnt_snd;

unsigned int glutch;
unsigned int sost;

extern char dabl;

unsigned int c_config_kd[2];

unsigned char crc_485(unsigned char num, unsigned char *p);

extern unsigned char fl_akb;




unsigned char bufer[2];

unsigned char query_full,obmen_full,point_head,point_tail;

extern char bit_registr1;


extern unsigned char nomer_tab;

unsigned char trevoga,iz_vzat;

unsigned char glob_tabletka;

unsigned int ust_vhod, timer_vhod,timer_autovzat;





void wr_uk (void)
{
unsigned char buf[4];
      buf[0]=point_head;
      buf[1]=point_tail;
      buf[2]=query_full;
      buf[3]=crc_485(3,&buf[0]);
      WrArrayToFlesh(A_UK_OHR,&buf[0],4,0,0);
      WrArrayToFlesh(A_UK1_OHR,&buf[0],4,0,0);
 
}


void wr_sost (void)
{
unsigned char buf[2];
      buf[0]=sost;
      buf[1]=crc_485(1,&buf[0]);
      WrArrayToFlesh(A_SOST_OHR,&buf[0],2,0,0);
      WrArrayToFlesh(A_SOST1_OHR,&buf[0],2,0,0); 
}



void morg( void)
{
  
   switch(prihod_uhod)
                                {
                                case 0:  break;
                                case 1: {SOST_ON; SOUND_ON; return;}   // приход
                                case 2:
                                         {
                                         if(glutch<170)
                                                  {
                                                  // indicator=indicator & ~ST_VZ;
                                                    SOST_OFF;
                                                    SOUND_OFF;
                                                  }
                                         if(glutch>170)
                                                   {
                                                  // indicator=indicator | ST_VZ; 
                                                   SOST_ON;
                                                   SOUND_ON;
                                                  }
                                                     if (glutch >340) glutch=0;
                                         return;            
                                         }
                                  case 4:
                                         {
                                         if(glutch<34)
                                                  {
                                                //   indicator=indicator & ~ST_VZ;
                                                   SOST_OFF;
                                                   SOUND_OFF;
                                                  }
                                         if(glutch>34)
                                                   {
                                                   //indicator=indicator | ST_VZ; 
                                                   SOST_ON;
                                                   SOUND_ON;
                                                  }
                                                     if (glutch >68) glutch=0;
                                         return;            
                                         }       
                                         
                                         
                                  case 3: {SOST_OFF; SOUND_OFF; return;}      
                                          
                                   default: {prihod_uhod=0;break;}
                                  }
   
   
    
  
                 switch(sost)
                                {
                                case 0: //ST_ON; 
                                       // indicator=indicator | ST_VZ;
                                        SOST_ON;
                                        SOUND_OFF;
                                 
                                        break;
                                
                                case 1: //ST_OFF; 
                                        //indicator=indicator & ~ST_VZ;
                                  SOST_OFF;      
                                  SOUND_OFF;
                                      
                                        break;
                                case 2:
                                         {
                                         
                                           
                                           
                                         if(glutch< 1000)
                                                  {
                                                  //ST_OFF;
                                                 // indicator=indicator & ~ST_VZ; 
                                                    SOST_OFF;
                                                  SOUND_OFF;
                                                  }
                                         if(glutch> 1000)
                                                   {
                                                   //ST_ON;
                                                   
                                                     
                                                  // indicator=indicator | ST_VZ; 
                                                     SOST_ON;
                                                    if(cnt_snd<OVER_SND)SOUND_ON;
                                                  }
                                         
                                                  if (glutch >2000) glutch=0;
                                         }
                                          break;


                                case 3:
                                         {
                                            if(glutch< 2000)
                                                  {

                                                      //ST_OFF;
                                                      //indicator=indicator & ~ST_VZ;  
                                                      SOST_OFF;
                                                      SOUND_OFF;

                                                  }
                                         if(glutch> 2000)
                                                  {
                                                    //ST_ON;
                                                   //indicator=indicator | ST_VZ;   
                                                    SOST_ON;
                                                   SOUND_ON;
                                                   }

                                          
                                         }
                                         
                                         break;

                                case 4:
                                           {
                                            if(glutch< 2000)
                                                  {

                                                     //ST_OFF;
                                                   //  indicator=indicator & ~ST_VZ; 
                                                     SOST_OFF;
                                                    SOUND_OFF;

                                                  }
                                         if(glutch> 2000)
                                                   {
                                                   //ST_ON;
                                                   //  indicator=indicator | ST_VZ; 
                                                     SOST_ON;
                                                     SOUND_ON;

                                                  }

                                          
                                         }
                                      
                                         break;



                                   default: break;
                                 }
 }










void insert_query(char tabletka,char sobit)
{

      unsigned char buf[4];
      unsigned char ukaz;
      //if(query_full) return;
      
       wr_sost();
      
      
       if(query_full) 
       { 
         
         if(sobit<2)
         {
         buf[0]=sobit;
         buf[1]=tabletka;
         
         if(point_head==0)ukaz=0x1e;else ukaz=point_head-2;
         
         WrArrayToFlesh(A_SOB_OHR+ukaz, &buf[0],2,0,0);
         }
         return;
       }
       
       buf[0]=sobit;
       buf[1]=tabletka;
      
      point_head=point_head & 0x1f;
      
      WrArrayToFlesh(A_SOB_OHR+point_head, &buf[0],2,0,0);
      
      
      point_head=( (point_head+2) & 0x1f);
      
      if(point_head==point_tail) query_full=1;
      
     wr_uk();
      /*
      buf[0]=point_head;
      buf[1]=point_tail;
      buf[2]=query_full;
      buf[3]=crc_485(3,&buf[0]);
      WrArrayToFlesh(A_UK_OHR,&buf[0],4,0,0);
      WrArrayToFlesh(A_UK1_OHR,&buf[0],4,0,0);  */
      
    //  modbus_mem1[0x155-BEGIN_SEG1]=point_head;
    //  modbus_mem1[0x156-BEGIN_SEG1]=point_tail;
    //  modbus_mem1[0x157-BEGIN_SEG1]=query_full;
}


char read_ohr(void)
{
char cc;
//unsigned char buf[4];
//
//  ЎгдҐа ®Ў¬Ґ­  Їгбв Ё«Ё ­Ґв
//
  if(obmen_full)
      {
       cc=bufer[0];
       glob_tabletka=bufer[1];
  return(cc);

      }


  if(point_head==point_tail)
                                      {
                                      if(query_full) goto extract_it;
                                      
                                      //if(fl_akb==0)bufer[0]=0x74;else bufer[0]=0x64;
                                      //bufer[1]=0;
                                      
                                     // if(fl_akb==0)bufer[0]=0x44;else bufer[0]=0x54;                                    
                                      if(flag_set_akb==0) bufer[0]=0x44;else bufer[0]=0x54; 
                                      if(modbus_mem1[AD_IND_DNS]==0)bufer[1]=99;else bufer[1]=modbus_mem1[AD_IND_DNS]-1;
                                      
                                       return(0x74);
                                      }

extract_it:
                 
        obmen_full=1; 
//  bit_registr3=bit_registr3 | OBMEN_FULL;


      // cc=adr_soob[point_tail];
      // glob_tabletka=adr_soob[point_tail+1];
       RdFromFleshToArr(A_SOB_OHR+point_tail, &bufer[0],2);          
       

       
       point_tail=((point_tail+2) & 0x1f);

       cc=bufer[0];
       glob_tabletka=bufer[1];
       
      query_full=0;
      
      wr_uk();
      
      /*
      buf[0]=point_head;
      buf[1]=point_tail;
      buf[2]=query_full;
      buf[3]=crc_485(3,&buf[0]);
      WrArrayToFlesh(A_UK_OHR,&buf[0],4,0,0);
      WrArrayToFlesh(A_UK1_OHR,&buf[0],4,0,0);  */
     

 //     modbus_mem1[0x155-BEGIN_SEG1]=point_head;
 //     modbus_mem1[0x156-BEGIN_SEG1]=point_tail;
  //    modbus_mem1[0x157-BEGIN_SEG1]=query_full;
      
       return(cc);

}

/*
void read_ohr_bt(void)
{
       
       obmen_full=0;
     
       if(point_head==point_tail)return;

       obmen_full=1;
       
       RdFromFleshToArr(A_SOB_OHR+point_tail, &bufer[0],2);          
       point_tail=((point_tail+2) & 0x1f);

      
       glob_tabletka=bufer[1];
       
       query_full=0;
      
      wr_uk();  
      modbus_mem1[0x155-BEGIN_SEG1]=point_head;
      modbus_mem1[0x156-BEGIN_SEG1]=point_tail;
      modbus_mem1[0x157-BEGIN_SEG1]=query_full;
     
      
}

*/

char test_query(void)
{
  if(query_full) return(1);
  else return(1);
}


                 void test_snat(void)
{
                                                 
                        if((iz_vzat==1)&&(timer_autovzat==0))
                        {
                          iz_vzat=0;
                          if(trevoga==0)
                          {
                            if(test_query()==0) return;
                            sost=VZAT;
                            insert_query(nomer_tab,sost);
                            insert_zapis_kd(nomer_tab,ZP_VZAT);
                            return;
                          }
                        }  
                        
                        
                          if((iz_vzat==1)&&(timer_autovzat!=0)&&(trevoga==1))
                        {
                          iz_vzat=0;                          
                        }  
  
                         if(( bit_registr1 & TABLETKA)==0) return;
                          //  semafor_clr(&bit_registr1 ,TABLETKA);
                         bit_registr1=bit_registr1 & ~TABLETKA;
                         
                         if( dabl==1) { dabl=0;return;}
                         
                         /*
                         if(trevoga==1) return;
                                               // shleif
//
//               if shleif norma & tabletka
//
//
                         if(trevoga==0)

                                 {
                                      if(test_query()==0) return;
                                 timer_vhod=ust_vhod;
                                 sost=VIHOD;
                          //       insert_query();
                                 }
                         */
                         
                                 if(test_query()==0) return;
                                 timer_vhod=ust_vhod;
                                 sost=VIHOD;


}


//
//               function sostojanij
//
//
                 void test_t_vhod(void)
{

                         if(timer_vhod==0)
                                             {
                                             if(test_query()==0) return;
                                             sost=ALARM;
                                             insert_query(nomer_tab,sost);
                                             insert_zapis_kd(nomer_tab,ZP_ALARM);
                                             }
                         if(timer_vhod !=0)
                                 {
                         if(( bit_registr1 & TABLETKA)==0) return;


                        //    semafor_clr(&bit_registr1 ,TABLETKA);
                          bit_registr1=bit_registr1 & ~TABLETKA;
                          
                         if( dabl==2) { dabl=0;return;}
                          
                            if(test_query()==0) return;
                         sost=SNAT;
                        if(sel_modul==1) PORTB=PORTB & ~0x04; //  PORTE=PORTE & ~4;  //ravil
                         insert_query(nomer_tab, sost);
                         
                         insert_zapis_kd(nomer_tab,ZP_SNAT);




                                  }



}

//
//               function sostojanij
//
//
                 void test_t_vihod(void)
{

                         if(timer_vhod==0)
                                         {
                                          if(test_query()==0) return;
                         if(trevoga==0)

        {
         sost=VZAT;
        if(sel_modul==1) PORTB=PORTB & ~0x04; //  PORTE=PORTE & ~4;          //ravil
        insert_query(nomer_tab,sost);
        insert_zapis_kd(nomer_tab,ZP_VZAT);
        }
                         if(trevoga==1)
      {
      sost=ALARM;
      insert_query(nomer_tab,sost);
      insert_zapis_kd(nomer_tab,ZP_ALARM);
      }
                                         }

                         if(timer_vhod !=0)
                                 {
                         if(( bit_registr1 & TABLETKA)==0) return;
        
                         //  semafor_clr(&bit_registr1 ,TABLETKA);
                         bit_registr1=bit_registr1 & ~TABLETKA;
                         
                         if( dabl==1) { dabl=0;return;}
                         
                         if(test_query()==0) return;

                        sost=SNAT;
                       if(sel_modul==1)  PORTB=PORTB & ~0x04; //  PORTE=PORTE & ~4;  //ravil
                        
                        timer_autovzat=ust_vhod;
                        iz_vzat=1;
                        
                         //insert_query(nomer_tab,sost);
                         //insert_zapis_kd(nomer_tab,ZP_SNAT);


                                  }


}



//
//               function sostojanij
//
//
                 void test_alarm(void)
{

                         if(( bit_registr1 & TABLETKA)==0) return;


                        //    semafor_clr(&bit_registr1 ,TABLETKA);
                         bit_registr1=bit_registr1 & ~TABLETKA;
                        
                         if( dabl==2) { dabl=0;return;}
                         
                         if(test_query()==0) return;
                         sost=SNAT;
                     if(sel_modul==1)  PORTB=PORTB & ~4;  //  только в режиме МКД пищать
                         insert_query(nomer_tab,sost);

                       insert_zapis_kd(nomer_tab,ZP_SNAT);



}



//
//               function sostojanij
//
//
                 void test_vzat(void)
{
     
                 if(trevoga==1)
                                               // shleif
                                 {
                                     if(test_query()==0) return;

                                 timer_vhod=ust_vhod;
                                 sost=VHOD;
                            //     insert_query();
                                 }
                 else

                 {
                         if(( bit_registr1 & TABLETKA)==0) return;
                        // semafor_clr(&bit_registr1 ,TABLETKA);
                        bit_registr1=bit_registr1 & ~TABLETKA;
                     
                         if( dabl==2) { dabl=0;return;}
                        
                         if(test_query()==0) return;
                         sost=SNAT;
                         timer_autovzat=ust_vhod;
                         iz_vzat=1;
                         insert_query(nomer_tab,sost);
                         insert_zapis_kd(nomer_tab,ZP_SNAT);
                   }


}

/*
void begin_kd (void)
{
  unsigned char buf[4];
  
  sost=VZAT;
  
  
  RdFromFleshToArr(A_UK_OHR,&buf[0],4);
  if(buf[3]==crc_485(3,&buf[0]))
    {
      if((buf[0]<32) && (buf[1]<32) && (buf[2]<2))
      {
        point_head=buf[0];
        point_tail=buf[1];
        query_full=buf[2];
        goto met1;
      }
    }
  
  RdFromFleshToArr(A_UK1_OHR,&buf[0],4);
  if(buf[3]==crc_485(3,&buf[0]))
    {
      if((buf[0]<32) && (buf[1]<32) && (buf[2]<2))
      {
        point_head=buf[0];
        point_tail=buf[1];
        query_full=buf[2];
        goto met1;
      }
    }
  
          point_head=0;
          point_tail=0;
          query_full=0;
          
          buf[0]=point_head;
          buf[1]=point_tail;
          buf[2]=query_full;
          buf[3]=crc_485(3,&buf[0]);
          WrArrayToFlesh(A_UK_OHR,&buf[0],4,0,0);
          WrArrayToFlesh(A_UK1_OHR,&buf[0],4,0,0);  
  
     
  
          
met1:
  if(test_query()==0) return;
  insert_query(0,sost); 
  insert_zapis_kd(0,ZP_VZAT);
  
   modbus_mem1[0x180-BEGIN_SEG1]=point_head;
   modbus_mem1[0x181-BEGIN_SEG1]=point_tail;
   modbus_mem1[0x182-BEGIN_SEG1]=query_full; 
     
  
  
}

*/

void begin_kd_2 (void)
{
  unsigned char buf[4];
  
 // if(sel_modul!=1)return;               21 06 2017
  
  RdFromFleshToArr(A_SOST_OHR,&buf[0],2);
  if(buf[1]==crc_485(1,&buf[0]))
  {
    sost=buf[0];
    goto met2;
  }
  
  RdFromFleshToArr(A_SOST1_OHR,&buf[0],2);
  if(buf[1]==crc_485(1,&buf[0]))
    {
    sost=buf[0];
    goto met2;
    }
    
    sost=VZAT;
      buf[0]=sost;
      buf[1]=crc_485(1,&buf[0]);
      WrArrayToFlesh(A_SOST_OHR,&buf[0],2,0,0);
      WrArrayToFlesh(A_SOST1_OHR,&buf[0],2,0,0); 
    
met2:
  
  RdFromFleshToArr(A_UK_OHR,&buf[0],4);
  if(buf[3]==crc_485(3,&buf[0]))
    {
      if((buf[0]<32) && (buf[1]<32) && (buf[2]<2))
      {
        point_head=buf[0];
        point_tail=buf[1];
        query_full=buf[2];
        goto met3;
      }
    }
  
  RdFromFleshToArr(A_UK1_OHR,&buf[0],4);
  if(buf[3]==crc_485(3,&buf[0]))
    {
      if((buf[0]<32) && (buf[1]<32) && (buf[2]<2))
      {
        point_head=buf[0];
        point_tail=buf[1];
        query_full=buf[2];
        goto met3;
      }
    }
  
          point_head=0;
          point_tail=0;
          query_full=0;
          
          //buf[0]=point_head;
         // buf[1]=point_tail;
         // buf[2]=query_full;
         // buf[3]=crc_485(3,&buf[0]);
         // WrArrayToFlesh(A_UK_OHR,&buf[0],4,0,0);
         // WrArrayToFlesh(A_UK1_OHR,&buf[0],4,0,0);  
          
          wr_uk();
         
          
          
          met3:
         
        //    modbus_mem1[0x155-BEGIN_SEG1]=point_head;
        //    modbus_mem1[0x156-BEGIN_SEG1]=point_tail;
        //    modbus_mem1[0x157-BEGIN_SEG1]=query_full; 
         
                 
         /*        
               if(test_query()==0) return;
               
               point_head=point_head & 0x1f;
               point_head=( (point_head+2) & 0x1f);
               if(point_head==point_tail) query_full=1;
               wr_uk();
  
                 modbus_mem1[0x180-BEGIN_SEG1]=point_head;
                 modbus_mem1[0x181-BEGIN_SEG1]=point_tail;
                 modbus_mem1[0x182-BEGIN_SEG1]=query_full; 
         */
  
return;
  
}

void zp_prihod_uhod(void)
{
  
switch (flag_arhiv_prihod)
{
case 1:insert_zapis_kd(nomer_tab,ZP_PRIHOD); flag_arhiv_prihod=0; break;
case 2:insert_zapis_kd(nomer_tab,ZP_UHOD);flag_arhiv_prihod=0;break;
default: break;
}
        
}


 void com_disp_mbus (void)
{
//unsigned int pred_sost;  

      bit_registr1=bit_registr1 | TABLETKA; 
      nomer_tab=0;  
 //     pred_sost=sost; 
      switch(sost)
                                {
                                case 0: test_vzat();  break;
                                case 1: test_snat();  break;
                                case 2: test_alarm(); break;
                                case 3: test_t_vhod(); break;
                                case 4: test_t_vihod(); break;
                                   default: break;
                                 } 
                
 //               if(pred_sost==sost) Rs485_buf_rx_tx[TX_BUF_RS485]=192;

}

void monitori(void)
                  {
                   
              //    if(sel_modul!=1)return;
                    if(sel_modul==1) {                   // только в режиме МКД
                   morg();
                   if(trevoga==1) {ST_SHL_ON;} else { ST_SHL_OFF;}
                    }
                    
                  zp_prihod_uhod();
                               
               //   if(trevoga==1){ SL_ON; } else {SL_OFF; }  
                  //!!! выдавать светодиод тревоги !!!!!!!!!!!!!!!!!!
                 
                // if(trevoga==1) indicator=indicator | ST_SHL; else indicator=indicator & ~ST_SHL;
                  
                 
                  
                 switch(sost)
                                {
                                case 0: cnt_snd=0;test_vzat();  break;
                                case 1: cnt_snd=0;test_snat();  break;
                                case 2: test_alarm(); break;
                                case 3: cnt_snd=0;test_t_vhod(); break;
                                case 4: cnt_snd=0;test_t_vihod(); break;
                                   default: sost=0;break;
                                 }
                   }

