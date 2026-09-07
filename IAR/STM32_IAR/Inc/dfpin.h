
//Ноги порта №0 (RS-232 №1) "коннтроллер - GSM-модем"/////////////////////////////////////////////////////////////////

#define RTS0 0x04  //RTS0  выход
#define DTR0 0x08  //DTR0  выход
#define CTS0 0x10  //CTS0  вход
#define DSR0 0x20  //DSR0  вход
#define DCD0 0x40  //DCD0  вход
#define RI0  0x80  //RI0   вход


//конфигурация ног на выход
#define PIN_OUT_PORT0\
{\
  DDRE=DDRE | RTS0 | DTR0;\
 }

#define PIN_HIZ_PORT0\
{\
  DDRE=DDRE & (~RTS0) & (~DTR0);\
 }

#define CLR_RTS0\
{\
  PORTE|=RTS0;\
 }

#define SET_RTS0\
{\
  PORTE&=~RTS0;\
 }

#define CLR_DTR0\
{\
  PORTE|=DTR0;\
 }

#define SET_DTR0\
{\
  PORTE&=~DTR0;\
 }

//конфигурация ног на вход
#define PIN_IN_PORT0\
{\
  PORTE=PORTE | CTS0 | DSR0 | DCD0 | RI0;\
 }

//////////////////////////////////////////////////////////////////////////////////////////////


//Ноги порта №1 (RS-485 №1)//////////////////////////////////////////////////////////
#define RTS1 0x04  //RTS1  выход
#define PIN_OUT_PORT1\
{\
  DDRJ=DDRJ | RTS1;\
 }

#define SET_RTS1\
{\
  PORTJ|=RTS1;\
 }
#define CLR_RTS1\
{\
  PORTJ&=~RTS1;\
 }
/////////////////////////////////////////////////////////////////////////



//Ноги порта №3 (RS-485 №2)///////////////////////////////////////////////////////////////
#define RTS3 0x10  //RTS3  выход
#define PIN_OUT_PORT3\
{\
  DDRD=DDRD | RTS3;\
 }

#define SET_RTS3\
{\
  PORTD|=RTS3;\
 }
#define CLR_RTS3\
{\
  PORTD&=~RTS3;\
 }
//////////////////////////////////////////////////////////////////////////////////////////



//Ноги порта №2 (RS-232 №2) /////////////////////////////////////////////////////////////

#define RTS2 0x04  //RTS0  выход
#define CTS2 0x02  //CTS0  вход

//конфигурация ног на выход
#define PIN_OUT_PORT2\
{\
  DDRH=DDRH | RTS2;\
 }

#define CLR_RTS2\
{\
  PORTH|=RTS2;\
 }

#define SET_RTS2\
{\
  PORTH&=~RTS2;\
 }

//конфигурация ног на вход
#define PIN_IN_PORT2\
{\
  PORTB=PORTB | CTS2;\
 }

/////////////////////////////////////////////////////////////////////////////////////////////////


//нога включения питания модема/////////////////////////////////
#define  PWR 0x40
//конфигурация ног на выход
#define PIN_OUT_PWR\
{\
  DDRJ=DDRJ | PWR;\
 }

#define SET_PWR\
{\
  PORTJ|=PWR;\
 }

#define CLR_PWR\
{\
  PORTJ&=~PWR;\
 }
/////////////////////////////////////////////////////////////////////////////////




//нога включения модема/////////////////////////////////
#define  PWRK 0x10
//конфигурация ног на выход
#define PIN_OUT_PWRK\
{\
  DDRB=DDRB | PWRK;\
 }

#define SET_PWRK\
{\
  PORTB|=PWRK;\
 }

#define CLR_PWRK\
{\
  PORTB&=~PWRK;\
 }
/////////////////////////////////////////////////////////////////////////////////


//ноги входов TC////////////////////////////////////////////////////////////////////
#define TCC1 0x04
#define TCC2 0x80

#define PIN_IN_TC1\
{\
  PORTG=PORTG | TCC1;\
 }

#define PIN_IN_TC2\
{\
  PORTA=PORTA | TCC2;\
 }
//////////////////////////////////////////////////////////////////////////////////////////

//ноги IO1-10]
#define IO1  0x80
#define IO2  0x40
#define IO3  0x01
#define IO4  0x80
#define IO5  0x04
#define IO6  0x10
#define IO7  0x02
#define IO8  0x04
#define IO9  0x04
#define IO10 0x02
#define IO11 0x10
#define IO12 0x08

#define TCH_I  0x80

#define PIN_IN_TS1_8\
{\
  DDRK=DDRK & (~IO1) & (~IO2) & (~IO6) & (~IO8);\
  DDRA=DDRA & (~IO3) & (~IO5) & (~IO7);\
  DDRJ=DDRJ & (~IO4);\
  PORTK=PORTK | IO1 | IO2 | IO6 | IO8;\
  PORTA=PORTA | IO3 | IO5 | IO7;\
  PORTJ=PORTJ | IO4;\
}

#define PIN_IN_MKD1_5\
{\
  DDRK=DDRK & (~IO6) & (~IO8);\
  DDRA=DDRA & (~IO3) & (~IO5);\
  DDRJ=DDRJ & (~IO4);\
  PORTK=PORTK | IO6 | IO8;\
  PORTA=PORTA | IO3 | IO5;\
  PORTJ=PORTJ | IO4;\
}

#define PIN_OUT_TU\
{\
  DDRA=DDRA | IO11 | IO12;\
}


#define TU2_ON\
{\
  PORTA=PORTA | IO12;\
}

#define TU2_OFF\
{\
  PORTA=PORTA & (~IO12);\
}

#define SOUND_ON\
{\
  PORTA=PORTA | IO12;\
}

#define SOUND_OFF\
{\
  PORTA=PORTA & (~IO12);\
}




#define TU1_ON\
{\
  PORTA=PORTA | IO11;\
}

#define TU1_OFF\
{\
  PORTA=PORTA & (~IO11);\
}



#define ST_SHL_ON\
{\
  PORTA=PORTA | IO11;\
}

#define ST_SHL_OFF\
{\
  PORTA=PORTA & (~IO11);\
}


#define PIN_OUT_MKD\
{\
  DDRK=DDRK | IO2;\
  DDRA=DDRA | IO7;\
}


#define TCH_O_ONE\
{\
  PORTK=PORTK | IO2;\
}

#define TCH_O_ZERO\
{\
  PORTK=PORTK & (~IO2);\
}



#define SOST_ON\
{\
  PORTA=PORTA | IO7;\
}

#define SOST_OFF\
{\
  PORTA=PORTA & (~IO7);\
}

//ноги управления SIM////////////////////////////////////////////////////////////////////
#define C_SIM1 0x20
#define C_SIM2 0x10
#define PIN_OUT_SIM\
{\
  DDRJ=DDRJ | C_SIM1 | C_SIM2;\
 }

#define SET_SIM1\
{\
  PORTJ=PORTJ & ~C_SIM2;\
  PORTJ=PORTJ | C_SIM1;\
 }

#define SET_SIM2\
{\
  PORTJ=PORTJ & ~C_SIM1;\
  PORTJ=PORTJ | C_SIM2;\
 }
/////////////////////////////////////////////////////////




//нога включения TEN /////////////////////////////////
#define  TEN 0x08
//конфигурация ног на выход
#define PIN_OUT_TEN\
{\
  DDRJ=DDRJ | TEN;\
 }

#define SET_TEN\
{\
  PORTJ|=TEN;\
 }

#define CLR_TEN\
{\
  PORTJ&=~TEN;\
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//ноги светодиодов /////////////////////////////////

#define S1_R 0x01  //светодиод статуса модема
#define S1_G 0x02
#define S2_R 0x01  //светодиод статуса порта №0 RS232-1
#define S2_G 0x02
#define S4_R 0x04  //светодиод статуса порта RS485-1
#define S4_G 0x08
#define S3_R 0x10  //светодиод статуса порта RS485-2
#define S3_G 0x20
#define S5_R 0x40  //светодиод статуса порта №3 RS232-2
#define S5_G 0x80

//конфигурация ног на выход
#define PIN_OUT_S1\
{\
  DDRG=DDRG | S1_R | S1_G;\
 }

#define PIN_OUT_S2_S5\
{\
  DDRC=DDRC | S2_R | S2_G | S3_R | S3_G | S4_R | S4_G | S5_R | S5_G;\
 }
///S1
#define S1_OFF\
{\
  PORTG&=~S1_R;\
  PORTG&=~S1_G;\
 }
#define S1_YL\
{\
  PORTG|=S1_R;\
  PORTG|=S1_G;\
 }
#define S1_RD\
{\
  PORTG|=S1_R;\
  PORTG&=~S1_G;\
 }
#define S1_GR\
{\
  PORTG&=~S1_R;\
  PORTG|=S1_G;\
 }
#define S1_CH\
{\
  PORTG^=S1_R;\
  PORTG^=S1_G;\
 }


///S2
#define S2_OFF\
{\
  PORTC&=~S2_R;\
  PORTC&=~S2_G;\
 }
#define S2_YL\
{\
  PORTC|=S2_R;\
  PORTC|=S2_G;\
 }
#define S2_RD\
{\
  PORTC|=S2_R;\
  PORTC&=~S2_G;\
 }
#define S2_GR\
{\
  PORTC&=~S2_R;\
  PORTC|=S2_G;\
 }
#define S2_CH\
{\
  PORTC^=S2_R;\
  PORTC^=S2_G;\
 }


///S3
#define S3_OFF\
{\
  PORTC&=~S3_R;\
  PORTC&=~S3_G;\
 }
#define S3_YL\
{\
  PORTC|=S3_R;\
  PORTC|=S3_G;\
 }
#define S3_RD\
{\
  PORTC|=S3_R;\
  PORTC&=~S3_G;\
 }
#define S3_GR\
{\
  PORTC&=~S3_R;\
  PORTC|=S3_G;\
 }
#define S3_CH\
{\
  PORTC^=S3_R;\
  PORTC^=S3_G;\
 }


///S4
#define S4_OFF\
{\
  PORTC&=~S4_R;\
  PORTC&=~S4_G;\
 }
#define S4_YL\
{\
  PORTC|=S4_R;\
  PORTC|=S4_G;\
 }
#define S4_RD\
{\
  PORTC|=S4_R;\
  PORTC&=~S4_G;\
 }
#define S4_GR\
{\
  PORTC&=~S4_R;\
  PORTC|=S4_G;\
 }
#define S4_CH\
{\
  PORTC^=S4_R;\
  PORTC^=S4_G;\
 }

///S5
#define S5_OFF\
{\
  PORTC&=~S5_R;\
  PORTC&=~S5_G;\
 }
#define S5_YL\
{\
  PORTC|=S5_R;\
  PORTC|=S5_G;\
 }
#define S5_RD\
{\
  PORTC|=S5_R;\
  PORTC&=~S5_G;\
 }
#define S5_GR\
{\
  PORTC&=~S5_R;\
  PORTC|=S5_G;\
 }
#define S5_CH\
{\
  PORTC^=S5_R;\
  PORTC^=S5_G;\
 }

//I2C
#define SDA 0x02
#define SCL 0x01



#define SDA_OUT\
{\
  DDRD|=SDA;\
 }

#define SDA_IN\
{\
  DDRD&=~SDA;\
 }

#define SET_SDA\
{\
  PORTD|=SDA;\
 }

#define CLR_SDA\
{\
  PORTD&=~SDA;\
 }




// определить по новой
/*
#define SDA_OUT\
{\
  __no_operation();\
 }

#define SDA_IN\
{\
  DDRD&=~SDA;\
 }

#define SET_SDA\
{\
    DDRD&=~SDA;\
    PORTD|=SDA;\
 }

#define CLR_SDA\
{\
  PORTD&=~SDA;\
    DDRD|=SDA;\
 }
*/
///////////////////////////////////////////////////////////


#define SCL_OUT\
{\
  DDRD|=SCL;\
 }
#define SET_SCL\
{\
  PORTD|=SCL;\
 }

#define CLR_SCL\
{\
  PORTD&=~SCL;\
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




