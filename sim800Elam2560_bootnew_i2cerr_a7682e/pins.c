#include <iom2560.h> 
#include "dfpin.h"
#include "dfproc.h"
#include <string.h> 
#include <inavr.h>
#include "dfcnst.h"
#include "map_mbus.h"
#include "ozu_map.h"
#include "def_at.h"
#include "map_ad.h"

#pragma location=A_DEBUG 
__eeprom __no_init unsigned char e_debug;

#pragma location=10
__eeprom __no_init unsigned char baud_port;
#pragma location=11
__eeprom __no_init unsigned char stop_port;
#pragma location=12
__eeprom __no_init unsigned char pariti_port;

void control_temperatura(void);
extern void long_delay(unsigned long int period);
 char At_buf[10];
extern unsigned char Regim;
extern unsigned char buf_tx_232[VOL_TX_PPP];
unsigned char point_GSM;
char cnt_rx1;
void lock_it(void);
void s_port( unsigned char vv);
extern struct
{
char buf[LN_BUF_AT];
unsigned char cnt_tx;
unsigned char cnt_rx;
unsigned char ln_buf;
unsigned char list_com[VOL_LIST];// перечень исполняемых команд
unsigned char ln_list;//длина перечня
unsigned char cnt_com;//счетчик команд
unsigned int cnt_tm_out; //счетчик времени ожидания ответа
unsigned int vol_tm_out; //предел времени ожидания ответа
unsigned int cnt_rx_out; //счетчик межбайтовый промежуток
unsigned int vol_rx_out; //предел межбайтового промежутка
}At_com;


extern struct
{
unsigned char ok :1;//требуемый ответ
unsigned char err :1;//ошибочный, нетребуемый ответ
unsigned char tm_out :1;//отсутствие ответа
unsigned char tx_en :1;//послать комманду
unsigned char rx_en :1;//принимать ответы
unsigned char rx_rec :1;//принят ответ
unsigned char greg_ereg :1;//пакетник GPRS or LTE
//unsigned char tm_out_en:1;//разрешение анализа по превышению времени ожидания ответа
}
fl_at_com;

void init_pins_hiz_to_out(void)
{
  //Конфигурирование ног порта №0 RS232-1
  PIN_OUT_PORT0;
  PIN_IN_PORT0;
  SET_DTR0;
  SET_RTS0;
  
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B | TXEN; 
  UCSR0B=UCSR0B | RXCIE;
  UCSR0A=UCSR0A | TXC;
  
  
}

void init_pins_out_to_hiz(void)
{
  //Конфигурирование ног порта №0 RS232-1

 
  PIN_HIZ_PORT0;
 
  UCSR0B=UCSR0B & ~RXEN;
  UCSR0B=UCSR0B & ~TXEN; 
  UCSR0B=UCSR0B & ~RXCIE;
  UCSR0A=UCSR0A & ~TXC;
  
}


void init_pins_ts_mod(void)
{
  PIN_IN_TS1_8;
}


void init_pins_mkd_mod(void)
{
PIN_IN_MKD1_5;
PIN_OUT_MKD;
}

void init_pins(void)
{
  /*
  //Конфигурирование ног порта №0 RS232-1
  PIN_OUT_PORT0;
  PIN_IN_PORT0;
  SET_DTR0;
  SET_RTS0;
  */
  
  //Конфигурирование ног порта №1 RS485-1
  PIN_OUT_PORT1;
  CLR_RTS1;
    
  //Конфигурирование ног порта №2 RS232-2
  PIN_OUT_PORT2;
  PIN_IN_PORT2;
  CLR_RTS2;
  
  //Конфигурирование ног порта №3 RS485-2
  PIN_OUT_PORT3;
  CLR_RTS3;

  // конфигурирование ног управления SIM
  PIN_OUT_SIM;
  SET_SIM1;
 
  // конфигурирование ноги включения питания модема
  PIN_OUT_PWR;
  CLR_PWR;
  
  // конфигурирование ноги включение модема
  PIN_OUT_PWRK;
  SET_PWRK;
   
   // конфигурирование ног ТС
  PIN_IN_TC1
  PIN_IN_TC2
  
  
    
 // PIN_IN_TS1_8;
  PIN_OUT_TU;  
    
    
  // конфигурирование ноги TEN  
  PIN_OUT_TEN;
  CLR_TEN;

  // конфигурация ног светодиодов
   PIN_OUT_S1;
   PIN_OUT_S2_S5;
   S1_OFF;
   S2_OFF;
   S3_OFF;
   S4_OFF;
   S5_OFF;
}




void init_proc_state(void)
{
  // Инициализация порта №0
  
  UCSR0B=0;
 // UBRR0H=R9600_H;
 // UBRR0L=R9600_L;  
  
   // A7682
 UBRR0H=R115200_H;
 UBRR0L=R115200_L;
 
/*
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B | TXEN; 
  UCSR0B=UCSR0B | RXCIE;
  UCSR0A=UCSR0A | TXC;
 */
   
  // Инициализация порта №1
  UCSR1B=0;
  UCSR1B=UCSR1B | TXEN;
  
  // Инициализация порта №2 RS-232 №2
  UCSR2B=0;
  CLR_RTS2;
   UBRR2H=R9600_H;
   UBRR2L=R9600_L; 
  UCSR2B=UCSR2B | TXEN;
  
  // Инициализация порта №3
  UCSR3B=0;
  UCSR3B=UCSR3B | TXEN;
  
 //таймер 0 
 OCR0A=85;// 340mks
 TCCR0A=0x02;
 TCCR0B=0x03; //прескалер 64
 TCNT0=0;
 TIMSK0=TIMSK0 | 0x02;
  
    //таймер2 
  OCR2A=64;           //1,024 мсек
  TCCR2A=0x02;
  TCCR2B=0x06;  //прескалер 256
  TIMSK2=TIMSK2 | 0x02;  
 
 //таймер 3`
  OCR3AH=0x3d; //15625
  OCR3AL=0x09; 
  TCCR3B=0x0d;       //преск 1024            // 1 сек
  TIMSK3=TIMSK3 | 0x02;  

  SCL_OUT;// i2C
  
  ADCSRA=0xcf;
  ADMUX=0x40;
  
  
  PCMSK2=0x80;
  PCICR=0x04;
  
}


void init_modem_only(void)
{
  // Инициализация порта №0
 
  
  UCSR0B=0;
 // UBRR0H=R9600_H;
 // UBRR0L=R9600_L; 
  
 UBRR0H=R115200_H;
 UBRR0L=R115200_L;
  
 

  // Инициализация порта №1
  UCSR1B=0;

  // Инициализация порта №2
  SET_RTS2;
  UCSR2B=0;
  UBRR2H=R9600_H;
  UBRR2L=R9600_L;  
  //UBRR2H=R115200_H;
 // UBRR2L=R115200_L;
  UCSR2B=UCSR2B | RXEN;
  UCSR2B=UCSR2B | TXEN;
  UCSR2B=UCSR2B | RXCIE;
  // Инициализация порта №3
  
  UCSR3B=0;
   
  
   if(Regim==MODEM_ONLY_R){SET_SIM2;}else {SET_SIM1;} //dobavka
   
  
  
 //////////////////////////////////////////////////////////
   PIN_OUT_PORT0;
  PIN_IN_PORT0;
  
  
  SET_DTR0;
  SET_RTS0;
   
   CLR_PWRK;
   SET_PWR;  
   
   __watchdog_reset();
   long_delay(10000000);//
   
   
   SET_PWRK;
   S3_RD;
   __watchdog_reset();
   long_delay(10000000);//>3 sek
   
   CLR_PWRK;
   S3_GR;
  
   
 __watchdog_reset();
   long_delay(10000000);//>3 sek
  long_delay(10000000);//>3 sek
  
  
  
  
  
  UCSR0B=UCSR0B | TXEN;
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B & ~TXCIE;
    
  UDR0='A';
   
   
   
   
   
   
   
   
  if(Regim==MODEM_ONLY_R){S1_RD;}else {S1_GR;} //dobavka 
  
  
  
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B | TXEN; 
  UCSR0B=UCSR0B | RXCIE;
 // UCSR0A=UCSR0A | TXC;
  
  S3_RD; 
  S4_RD;
  S5_GR;
  

  
  __enable_interrupt();
     while(1){ __watchdog_reset();control_temperatura();}
}


void long_delay_wait_answer(unsigned long int period)
{
  char i;
  bad1:
      if ( UCSR0A & 0x80) {
        // накапливаем данные с порта UDR0 в массив
           At_com.cnt_rx_out=100;
            
                    if(cnt_rx1 < 100)
                          {
                           buf_tx_232[cnt_rx1]=UDR0;
                           cnt_rx1++;
                           }
      }
   
      
  period--;
  __watchdog_reset();
  if(period !=0) goto bad1;
  
/////////////////////////////////////////////////////////

 
          
          long_delay(1000);
        
          UDR2=buf_tx_232[0];
          UCSR2A = UCSR2A | 0x40;
          
          
          for ( i=1;i< (cnt_rx1+1);i++) {
      
wait_ready22:
    if ( (UCSR2A & 0x40) ==0) goto wait_ready22;
    UCSR2A = UCSR2A | 0x40;
    if(i==cnt_rx1) goto exit_from1;
    UDR2=buf_tx_232[i];
      /////////////////
                                       }
                               
  
exit_from1:
  cnt_rx1=0;  
  
  
}





void send_at(void)
{
   char At_ln_buf,i;
    At_ln_buf=strlen(At_buf)+1;
    At_buf[At_ln_buf-1]=0x0d; 
    for ( i=0;i< At_ln_buf;i++) {
      
wait_ready:
    if ( (UCSR0A & 0x20) ==0) goto wait_ready;
    
    UDR0=At_buf[i];
                                 }
    long_delay(1000000);
  //  i=UDR0;  // мусор выгрести
}




void send_to_sim900(char num)
{
   char i;
   UCSR0B=UCSR0B | TXEN;
   UCSR0B=UCSR0B | RXEN;
   UCSR0B=UCSR0B & ~TXCIE;
//   UCSR0B=UCSR0B & ~RXCIE;
    UCSR0B=UCSR0B | RXCIE;     // разрешить прием от SIM модуля
    for ( i=0;i< num;i++) {
      
wait_ready:
    if ( (UCSR0A & 0x20) ==0) goto wait_ready;
    
    UDR0=buf_tx_232[i];
                                 }
    cnt_rx1=0;
 //   long_delay_wait_answer(500000); // УВЕЛИЧИЛ В 5 РАЗ
  
}

void send_atp(void)
{
   char At_ln_buf,i;
    At_ln_buf=strlen(At_buf);
    
    for ( i=0;i< At_ln_buf;i++) {
      
wait_ready:
    if ( (UCSR0A & 0x20) ==0) goto wait_ready;
    
    UDR0=At_buf[i];
                                 }
    long_delay(1000000);
  //  i=UDR0;  // мусор выгрести
}



void fun_init_sim900(void)
{
   UCSR0B=0;
  //UBRR0H=R9600_H;
  //UBRR0L=R9600_L; 
  
 // A7682
 UBRR0H=R115200_H;
 UBRR0L=R115200_L; 
  
   SET_SIM1; //dobavka
   s_port('.');
   CLR_PWR;                              // выключить питание
   SET_PWRK;
    __watchdog_reset();
   long_delay(10000000/4);//
   
   
   SET_PWR; 
   CLR_PWRK;
   
   __watchdog_reset();
   long_delay(10000000/4);//
   s_port('.');
   long_delay(10000000/4);//>3 sek
   SET_PWRK;
   __watchdog_reset();
   long_delay(10000000);//>3 sek
   s_port('.');
   CLR_PWRK;
  
 
  
  PIN_OUT_PORT0;
  PIN_IN_PORT0;
  
  CLR_DTR0; 
  CLR_RTS0;
   __watchdog_reset();
   long_delay(1000000/4);//>1.5 sek 
   s_port('.');
  SET_DTR0;
  SET_RTS0;
  
  
  
  
  
  
  
  UCSR0B=UCSR0B | TXEN;
  UCSR0B=UCSR0B | RXEN;
  
  UCSR0B=UCSR0B & ~TXCIE;
  /*  
  UCSR0B=UCSR0B | RXCIE;  //
  
   UCSR2B=UCSR2B | RXEN;  //
   UCSR2B=UCSR2B | TXEN;  //
   UCSR2B=UCSR2B & ~TXCIE;  //
   UCSR2B=UCSR2B | RXCIE;  //
   
   Regim=MODEM_ONLY;  //
   __enable_interrupt();
   
   strcpy(At_buf, "AT+CBST=0,0,1");      // АВТО моду 
   send_at(); 
    
     UBRR0H=R4800_H;
     UBRR0L=R4800_L;
    
  //  strcpy(At_buf, "atv0");           
  //  send_at();      
    */
    
    __enable_interrupt();
}



void init_modem_call(void)
{
  char sost_call,a,str_cmp[6];
  int cnt_rx,i;
  __watchdog_reset();
  __disable_interrupt();
  sost_call=0;
  UCSR0B=0;
//  UBRR0H=R9600_H;
//  UBRR0L=R9600_L; 
  
  
   // A7682
 UBRR0H=R115200_H;
 UBRR0L=R115200_L;
  
 //UBRR0H=R115200_H;
 //UBRR0L=R115200_L;
  
 

  

  // Инициализация порта №2
  SET_RTS2;
  UCSR2B=0;
  UBRR2H=R9600_H;
  UBRR2L=R9600_L;  
  
 
  
  CLR_RTS1;
  UCSR1B=0;
  UBRR1H=R9600_H;
  UBRR1L=R9600_L; 
  ///////////////////////////////////////////////////////
   CLR_RTS3;
   UCSR3B=0;
   UBRR3H=R9600_H;
   UBRR3L=R9600_L; 
  
   SET_SIM1; //dobavka
   
   SET_PWR;  
   __watchdog_reset();
   long_delay(10000000);//>3 sek
   SET_PWRK;
   __watchdog_reset();
   long_delay(10000000);//>3 sek
   CLR_PWRK;
  
 
  
  PIN_OUT_PORT0;
  PIN_IN_PORT0;
  
  CLR_DTR0; 
  CLR_RTS0;
   __watchdog_reset();
   long_delay(1000000);//>3 sek 
  SET_DTR0;
  SET_RTS0;
  
  
  
  
  
  
  
  UCSR0B=UCSR0B | TXEN;
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B & ~TXCIE;
    
   UCSR2B=UCSR2B | RXEN;
   UCSR2B=UCSR2B | TXEN;
   UCSR2B=UCSR2B & ~TXCIE;
   
    UCSR3B=UCSR3B | RXEN;
    UCSR3B=UCSR3B | TXEN;
    UCSR3B=UCSR3B & ~TXCIE;
    
    UCSR1B=UCSR1B | RXEN;
    UCSR1B=UCSR1B | TXEN;
    UCSR1B=UCSR1B & ~TXCIE;
    
    strcpy(At_buf, "atv0");           
    send_at();      
    S1_RD;
    S2_OFF;
    S3_OFF;
    S4_OFF;
    S5_OFF;
    
    __enable_interrupt();
    
    while(1) {
       __watchdog_reset();
    switch(sost_call)
    {
    case 0:
      {
        S2_RD;
        if (PINE & RI0)  { sost_call=1; break;}     // call
        if (PINE & DCD0) { sost_call=2; cnt_rx=0; break;}   // connect
        break;
      }
      
    case 1:
      {
       S2_GR;
       strcpy(At_buf, "ata");     // поднять трубку и ждать connect      
       send_at();  
       sost_call=0;
       break;
      }
    
    case 2:
      {
        S2_YL;
    //    if ( UCSR0A & 0x80) UDR2=UDR0;  // перекидываеи данные из порта GSM в СОМ порт 232 и наоборот дуплексный режим
    //    if ( UCSR2A & 0x80) UDR0=UDR2;
       
        // обратный канал простой все что пришло по портам, то и толкаю в модем
        if ( UCSR2A & 0x80) UDR0=UDR2;
        if ( UCSR1A & 0x80) UDR0=UDR1;
        if ( UCSR3A & 0x80) UDR0=UDR3;
        
        
     /////////////////////////////////  
        if ( UCSR0A & 0x80) {
        // накапливаем данные с порта UDR0 в массив
           At_com.cnt_rx_out=100;
           a= UDR0;       
                    if(cnt_rx < 1000)
                          {
                           buf_tx_232[cnt_rx]=a;
                           cnt_rx++;
                           }
                    
                    str_cmp[0]=str_cmp[1];
                    str_cmp[1]=str_cmp[2];
                    str_cmp[2]=str_cmp[3];
                    str_cmp[3]=str_cmp[4];
                    str_cmp[4]=a & ~0x20;     // убить регистр верхний нижний
                    
                    if ( str_cmp[0]=='A'  && str_cmp[1]=='T' && str_cmp[2]=='D' && str_cmp[3]=='O' && str_cmp[4]=='N' ) {e_debug=4; 
                                 strcpy(At_buf, "OK");      //        
                                 send_at(); }
                    
                    
                    if ( str_cmp[0]=='A'  && str_cmp[1]=='T' && str_cmp[2]=='D' && str_cmp[3]=='O' && str_cmp[4]=='F' ) {e_debug=0; 
                    strcpy(At_buf, "OK");      //        
                    send_at();
                    lock_it();}  // перезагрузиться прейти в GPRS моду
                    
        }
  /////////////////////////////////////////////
        
        if (fl_at_com.rx_rec==1)  {
          fl_at_com.rx_rec=0;
          SET_RTS1;
          SET_RTS3;
          long_delay(10000);
          __disable_interrupt();
          UDR2=UDR1=UDR3=buf_tx_232[0];
          UCSR2A = UCSR2A | 0x40;
          UCSR1A = UCSR1A | 0x40;
          UCSR3A = UCSR3A | 0x40;
          
          for ( i=1;i< (cnt_rx+1);i++) {
      
wait_ready2:
    if ( (UCSR2A & 0x40) ==0) goto wait_ready2;
    UCSR2A = UCSR2A | 0x40;
    if(i==cnt_rx) goto exit_from;
    UDR2=buf_tx_232[i];
    
wait_ready1:
    if ( (UCSR1A & 0x40) ==0) goto wait_ready1;
    UCSR1A = UCSR1A | 0x40;
    UDR1=buf_tx_232[i];
    
    wait_ready3:
    if ( (UCSR3A & 0x40) ==0) goto wait_ready3;
    UCSR3A = UCSR3A | 0x40;
    UDR3=buf_tx_232[i];
    
    
                                 }
          
exit_from:          
          __enable_interrupt();
        //  long_delay(100);
       //  ожидаем ухода последннего бита по порту 485 и снимаем RTS   
         
        CLR_RTS3;
               
        CLR_RTS1;
        
        cnt_rx=0;
        long_delay(100);
        a=UDR1;
        a=UDR2;
        a=UDR3;
        UCSR0B=UCSR0B | RXEN;  
        }
 //////////////////////////////////////////////////////////////////////
        
        if ( (PINE & DCD0)==0 ) { sost_call=0; break;}   // потеря соединения
        break;
      }
      
    default: { sost_call=0; break;}
    }
    }
   
}


void init_modem_call_inside(void)
{
  char sost_call,a;
  int cnt_rx,i;
  char str_cmp[6],str_cmp1[12];
  __watchdog_reset();
  __disable_interrupt();
  sost_call=0;
  UCSR0B=0;
 // UBRR0H=R9600_H;
 // UBRR0L=R9600_L; 
  
   // A7682
 UBRR0H=R115200_H;
 UBRR0L=R115200_L;
  
 //UBRR0H=R115200_H;
 //UBRR0L=R115200_L;
  
 

  

  // Инициализация порта №2
  SET_RTS2;
  UCSR2B=0;
  UBRR2H=R9600_H;
  UBRR2L=R9600_L;  
  
 
  
  CLR_RTS1;
  UCSR1B=0;
  UBRR1H=R9600_H;
  UBRR1L=R9600_L; 
  ///////////////////////////////////////////////////////
   CLR_RTS3;
   UCSR3B=0;
   UBRR3H=R9600_H;
   UBRR3L=R9600_L; 
 
   PIN_OUT_PORT0;
  PIN_IN_PORT0;
  
  CLR_DTR0; 
  CLR_RTS0;
   __watchdog_reset();
   long_delay(1000000);//>3 sek 
  SET_DTR0;
  SET_RTS0;
  
  
  
  
  
  
  UCSR0B=UCSR0B | TXEN;
  UCSR0B=UCSR0B | RXEN;
  UCSR0B=UCSR0B & ~TXCIE;
//  UCSR0B=UCSR0B | RXCIE;      // посмотреть 
//  Regim=MODEM_ONLY;
    
   UCSR2B=UCSR2B | RXEN;
   UCSR2B=UCSR2B | TXEN;
   UCSR2B=UCSR2B & ~TXCIE;
   
    UCSR3B=UCSR3B | RXEN;
    UCSR3B=UCSR3B | TXEN;
    UCSR3B=UCSR3B & ~TXCIE;
    
    UCSR1B=UCSR1B | RXEN;
    UCSR1B=UCSR1B | TXEN;
    UCSR1B=UCSR1B & ~TXCIE;
    
     __enable_interrupt();
    strcpy(At_buf, "+++");       // перейти в онлайн командную моду      
    send_atp();
   __watchdog_reset();
   long_delay(1000000);//>3 sek
  __watchdog_reset();
   long_delay(1000000);//>3 sek
    
    
    strcpy(At_buf, "ath");      // повесить трубку       
    send_at();  
    long_delay(1000000);//>3 sek
    
 //   strcpy(At_buf, "at+cbst=12,0,1");      // пперейти в моду 9600
 //   send_at(); 
    
    
 //    strcpy(At_buf, "atv1");      // повесить трубку       
 //   send_at();
    
    S1_RD;
    S2_OFF;
    S3_OFF;
    S4_OFF;
    S5_OFF;
    
   
    
    sost_call=1;    //    вход в процедуру при состоянии call
    
    while(1) {
       __watchdog_reset();
    switch(sost_call)
    {
    case 0:
      {
        S2_RD;
        if (PINE & RI0)  { sost_call=1; break;}     // call
        if (PINE & DCD0) { sost_call=2; cnt_rx=0; break;}   // connect
        break;
      }
      
    case 1:
      {
       S2_GR;
       strcpy(At_buf, "ata");     // поднять трубку и ждать connect      
       send_at();  
       sost_call=0;
       break;
      }
    
    case 2:
      {
        S2_YL;
    //    if ( UCSR0A & 0x80) UDR2=UDR0;  // перекидываеи данные из порта GSM в СОМ порт 232 и наоборот дуплексный режим
    //    if ( UCSR2A & 0x80) UDR0=UDR2;
       
        // обратный канал простой все что пришло по портам, то и толкаю в модем
        if ( UCSR2A & 0x80) UDR0=UDR2;
        if ( UCSR1A & 0x80) UDR0=UDR1;
        if ( UCSR3A & 0x80) UDR0=UDR3;
        
     /////////////////////////////////  
        if ( UCSR0A & 0x80) {
          a=UDR0;
        // накапливаем данные с порта UDR0 в массив
           At_com.cnt_rx_out=100;
                    
                    if(cnt_rx < 1000)
                          {
                           buf_tx_232[cnt_rx]=a;
                           cnt_rx++;
                           }
                    
                    
                    str_cmp[0]=str_cmp[1];
                    str_cmp[1]=str_cmp[2];
                    str_cmp[2]=str_cmp[3];
                    str_cmp[3]=str_cmp[4];
                    str_cmp[4]=a & ~0x20;     // убить регистр верхний нижний
                    
                    if ( str_cmp[0]=='A'  && str_cmp[1]=='T' && str_cmp[2]=='D' && str_cmp[3]=='O' && str_cmp[4]=='N' ) {e_debug=4; 
                                 strcpy(At_buf, "OK");      //        
                                 send_at(); }
                    
                    
                    if ( str_cmp[0]=='A'  && str_cmp[1]=='T' && str_cmp[2]=='D' && str_cmp[3]=='O' && str_cmp[4]=='F' ) {e_debug=0; 
                    strcpy(At_buf, "OK");      //        
                    send_at();
                    lock_it();}  // перезагрузиться прейти в GPRS моду
                    
                    //  запись контекста  в 12 разрядный регистр
                    str_cmp1[0]=str_cmp1[1];
                    str_cmp1[1]=str_cmp1[2];
                    str_cmp1[2]=str_cmp1[3];
                    str_cmp1[3]=str_cmp1[4];
                    str_cmp1[4]=str_cmp1[5];
                    str_cmp1[5]=str_cmp1[6];
                    str_cmp1[6]=str_cmp1[7];
                    str_cmp1[7]=str_cmp1[8];
                    str_cmp1[8]=str_cmp1[9];
                    str_cmp1[9]=str_cmp1[10];
                    str_cmp1[10]=str_cmp1[11];
                    str_cmp1[11]=a;
                    
                    if ( str_cmp1[0]=='A'  &&
                         str_cmp1[1]=='T'  &&
                         str_cmp1[2]=='I'  &&
                         str_cmp1[3]=='F'  &&
                         str_cmp1[4]=='C'  &&
                      //   str_cmp1[5]=='D'  &&
                         str_cmp1[6] =='='  && 
                         str_cmp1[7] <=0x33  && str_cmp1[7] >=0x30 &&
                         str_cmp1[8] ==','  &&  
                         str_cmp1[9] <=0x33  &&  str_cmp1[9] >=0x30 &&
                         str_cmp1[10]==','  &&   
                         str_cmp1[11] <=0x32  &&  str_cmp1[11] >=0x30  )      {
                                                                               baud_port=str_cmp1[7];
                                                                               stop_port=str_cmp1[9];
                                                                               pariti_port=str_cmp1[11];
                                                                               strcpy(At_buf, "OK");             
                                                                               send_at(); 
                           }
                    
                   
                    
                    
        }
  /////////////////////////////////////////////
        
        if (fl_at_com.rx_rec==1)  {
          fl_at_com.rx_rec=0;
          
          SET_RTS1;
          SET_RTS3;
          long_delay(10000);
          __disable_interrupt();
          UDR2=UDR1=UDR3=buf_tx_232[0];
          UCSR2A = UCSR2A | 0x40;
          UCSR1A = UCSR1A | 0x40;
          UCSR3A = UCSR3A | 0x40;
          
          for ( i=1;i< (cnt_rx+1);i++) {
      
wait_ready2:
    if ( (UCSR2A & 0x40) ==0) goto wait_ready2;
    UCSR2A = UCSR2A | 0x40;
    if(i==cnt_rx) goto exit_from;
    UDR2=buf_tx_232[i];
    
wait_ready1:
    if ( (UCSR1A & 0x40) ==0) goto wait_ready1;
    UCSR1A = UCSR1A | 0x40;
    UDR1=buf_tx_232[i];
    
    wait_ready3:
    if ( (UCSR3A & 0x40) ==0) goto wait_ready3;
    UCSR3A = UCSR3A | 0x40;
    UDR3=buf_tx_232[i];
    
    
                                 }
          
exit_from:          
          __enable_interrupt();
       //   long_delay(100);
       //  ожидаем ухода последннего бита по порту 485 и снимаем RTS   
         
        CLR_RTS3;
               
        CLR_RTS1;
        
        cnt_rx=0;
        long_delay(100);
        a=UDR1;
        a=UDR2;
        a=UDR3;
        UCSR0B=UCSR0B | RXEN;  
        }
 //////////////////////////////////////////////////////////////////////
        
        if ( (PINE & DCD0)==0 ) { sost_call=0; break;}   // потеря соединения
        break;
      }
      
    default: { sost_call=0; break;}
    }
    }
   
}






void init_ports_debug(void)
{
   // Инициализация порта №1
  UCSR1B=0;
  UBRR1H=R9600_H;
  UBRR1L=R9600_L;
  UCSR1B=UCSR1B | TXEN;
  SET_RTS3;
  
  // Инициализация порта №2 RS-232 №2
  UCSR2B=0;
  UBRR2H=R115200_H;
  UBRR2L=R115200_L;
  CLR_RTS2;
  UCSR2B=UCSR2B | TXEN;
  
  // Инициализация порта №3
  UCSR3B=0;
  UBRR3H=R9600_H;
  UBRR3L=R9600_L;
  UCSR3B=UCSR3B | TXEN;
  SET_RTS1;  
  
  S3_YL;
  S4_YL;
 // S5_YL;
}




