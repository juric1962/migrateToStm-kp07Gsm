#define DCD       (0x20)   // наличие несущей             

#define RTS_232   (0x01)   //  
#define CTS_232   (0x02)   //        вход 
#define RTS_485_1   (0x04)   // 
#define RTS_485_2   (0x01)   //  выход



#define LED_SOST_S (0x08)   //  светодиод состояния 
#define LED_OHRN_S (0x10)   //  светодиод охраны


#define SVD1_2 (0x08)   //  светодиод 
#define SVD1_1 (0x10)   //  

#define SVD2_1 (0x20)   //  светодиод 
#define SVD2_2 (0x40)   // 

#define DSR       (0x02)   //           вход    
#define DTR       (0x01)   // терминал                выход
#define ON_OFF    (0x80)   // включение выключение модема          выход  
 
#define PIN_AKB_SET (0x40)
 
 
# define ON_LUFKIN (0x04)
 
  
#define RXCIE 0x80
#define TXCIE 0x40
#define UDRIE 0x20
#define RXEN 0x10
#define TXEN 0x08
#define TXC  0x40

#define OUT_BIT485_2 0x40
#define IN_BIT485_2  0x02

#define OUT_BIT232_2 0x10
#define IN_BIT232_2  0x20

#define SDA      0x08
#define SCL      0x04   


#define SPEED_232  9600
#define R115200_H  0x00
#define R115200_L  0x08


#define R9600_H  0x00
#define R9600_L  103

#define R4800_H  0x00
#define R4800_L  207

#define R2400_H  0x01
#define R2400_L  0x9f

#define R19200_H  0x00
#define R19200_L  51


#define lufkin_port_on\
{\
  PORTG&=~ON_LUFKIN;\
 }

#define lufkin_port_off\
{\
  PORTG|=ON_LUFKIN;\
 }

#define set_rts_232\
{\
  PORTB&=~RTS_232;\
 }

#define clr_rts_232\
{\
  PORTB|=RTS_232;\
 }
 
#define clr_rts_485_1\
{\
  PORTA&=~RTS_485_1;\
 }

#define set_rts_485_1\
{\
  PORTA|=RTS_485_1;\
 }


#define clr_rts_485_2\
{\
  PORTD&=~RTS_485_2;\
 }

#define set_rts_485_2\
{\
  PORTD|=RTS_485_2;\
 }



#define led_sost_s_off\
{\
  PORTB&=~LED_SOST_S;\
 }

#define led_sost_s_on\
{\
  PORTB|=LED_SOST_S;\
 }
 
#define led_ohrn_s_off\
{\
  PORTB&=~LED_OHRN_S;\
 }

#define led_ohrn_s_on\
{\
  PORTB|=LED_OHRN_S;\
 } 

 //  SVD1_1  зеленый ближе к процессору    
 //  SVD2_1  красный  
        
#define SVD1_1_OFF\
{\
  PORTA&=~SVD1_1;\
 }

#define SVD1_1_ON\
{\
  PORTA|=SVD1_1;\
 } 

#define SVD1_2_OFF\
{\
  PORTA&=~SVD1_2;\
 }

#define SVD1_2_ON\
{\
  PORTA|=SVD1_2;\
 } 


#define SVD2_1_OFF\
{\
  PORTA&=~SVD2_1;\
 }

#define SVD2_1_ON\
{\
  PORTA|=SVD2_1;\
 } 

#define SVD2_2_OFF\
{\
  PORTA&=~SVD2_2;\
 }

#define SVD2_2_ON\
{\
  PORTA|=SVD2_2;\
 }

#define CLR485_2\
{\
  PORTE&=~OUT_BIT485_2;\
 }

#define SETB485_2\
{\
  PORTE|=OUT_BIT485_2;\
 }   
 
#define CLR232_2\
{\
  PORTE&=~OUT_BIT232_2;\
 }

#define SETB232_2\
{\
  PORTE|=OUT_BIT232_2;\
 }  
 
 
 #define modem_off\
{\
  PORTA&=~ON_OFF;\
 }

#define modem_on\
{\
  PORTA|=ON_OFF;\
 }        
 
 
    




