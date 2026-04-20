
#define A_VER_MAP 0
#define L_VER_MAP 16
#define A_VER_CRC L_VER_MAP-2
#define L_VER_MAP_MAX 20 //max длина области 


#define A_C_GPRS  A_VER_MAP+L_VER_MAP_MAX
#define L_C_GPRS_MAX 60//max длина области
#define A_C_GPRS_CRC A_C_GPRS+L_C_GPRS_MAX-2


#define A_CR_GPRS  A_C_GPRS+L_C_GPRS_MAX
#define L_CR_GPRS_MAX 60//max длина области
#define A_CR_GPRS_CRC A_CR_GPRS+L_CR_GPRS_MAX-2


#define A_IP_PAR  A_CR_GPRS+L_CR_GPRS_MAX
#define L_IP_PAR 10
//карта внутри области
      #define OFS_IP          0
      #define A_IP            A_IP_PAR+OFS_IP
      #define OFS_PORT        4
      #define A_PORT          A_IP_PAR+OFS_PORT
      #define OFS_NUM         6
      #define A_NUM           A_IP_PAR+OFS_NUM
      #define OFS_IP_PAR_CRC  8   
      #define A_IP_PAR_CRC    A_IP_PAR+OFS_IP_PAR_CRC





#define A_C_PAR  A_IP_PAR+L_IP_PAR
#define L_C_PAR  14                                      //!!!!!!!!!!!!!!!!dobavka
//карта внутри области
      #define OFS_NAT          0
      #define A_NAT            A_C_PAR+OFS_NAT
      #define OFS_CCH          2     
      #define A_CCH            A_C_PAR+OFS_CCH
      #define OFS_TM_CH        4   
      #define A_TM_CH          A_C_PAR+OFS_TM_CH
     
      #define OFS_TM_NO_LINK   6                         //!!!!!!!!!!!!!!!!dobavka
      #define A_TM_NO_LINK     A_C_PAR+OFS_TM_NO_LINK
      #define OFS_TM_LINK_RES  8   
      #define A_TM_LINK_RES    A_C_PAR+OFS_TM_LINK_RES   //EEEEEEEEEEEEEEE dobavka

      #define OFS_NAT_R        10
      #define A_NAT_R          A_C_PAR+OFS_NAT_R

      #define OFS_C_PAR_CRC    12                          //!!!!!!!!!!!!!!!!dobavka
      #define A_C_PAR_CRC      A_C_PAR+OFS_C_PAR_CRC



#define A_SEQ_PAR  A_C_PAR+L_C_PAR
#define L_SEQ_PAR  11 
//карта внутри области
      #define OFS_DES_SEQ        0
      #define A_DES_SEQ           A_SEQ_PAR+OFS_DES_SEQ
      #define OFS_NUM_CL         1
      #define A_NUM_CL           A_SEQ_PAR+OFS_NUM_CL
      #define OFS_TM_VZ          3
      #define A_TM_VZ            A_SEQ_PAR+OFS_TM_VZ
      #define OFS_TM_CL          5
      #define A_TM_CL            A_SEQ_PAR+OFS_TM_CL 
      #define OFS_TP_TS          7
      #define A_TP_TS            A_SEQ_PAR+OFS_TP_TS
      #define OFS_SEQ_PAR_CRC    9
      #define A_SEQ_PAR_CRC      A_SEQ_PAR+OFS_SEQ_PAR_CRC
 

/*
#define A_PORT_GPRS  A_SEQ_PAR+L_SEQ_PAR
#define L_PORT_GPRS  7
//карта внутри области
      #define OFS_GPRS_DES          0
      #define A_GPRS_DES            A_PORT_GPRS+OFS_GPRS_DES
      #define OFS_GPRS_S            1
      #define A_GPRS_S              A_PORT_GPRS+OFS_GPRS_S
      #define OFS_PORT_GPRS_CRC     5
      #define A_PORT_GPRS_CRC       A_PORT_GPRS+OFS_PORT_GPRS_CRC
*/ 
  
#define A_KOD_SIM1  A_SEQ_PAR+L_SEQ_PAR
#define L_KOD_SIM1  6
//карта внутри области
      #define OFS_PIN_SIM1       0
      #define A_PIN_SIM1         A_KOD_SIM1+OFS_PIN_SIM1
      #define OFS_KOD_SIM1_CRC   4
      #define A_KOD_SIM1_CRC     A_PIN_SIM1+OFS_KOD_SIM1_CRC


#define A_KOD_SIM2  A_KOD_SIM1+L_KOD_SIM1
#define L_KOD_SIM2  6
//карта внутри области
      #define OFS_PIN_SIM2       0
      #define A_PIN_SIM2         A_KOD_SIM2+OFS_PIN_SIM2
      #define OFS_KOD_SIM2_CRC   4
      #define A_KOD_SIM2_CRC     A_PIN_SIM2+OFS_KOD_SIM2_CRC



#define A_SEL_MODUL 246
#define A_C1_GPRS          250
#define  L_C1_GPRS_MAX      60//max длина области
#define A_C1_GPRS_CRC      A_C1_GPRS+L_C1_GPRS_MAX-2 

#define A_C2_GPRS          A_C1_GPRS+L_C1_GPRS_MAX 
#define L_C2_GPRS_MAX      60//max длина области
#define A_C2_GPRS_CRC      A_C2_GPRS+L_C2_GPRS_MAX-2 

#define A_C3_GPRS          A_C2_GPRS+L_C2_GPRS_MAX 
#define L_C3_GPRS_MAX      60//max длина области
#define A_C3_GPRS_CRC      A_C3_GPRS+L_C3_GPRS_MAX-2 







#define ABUF_CR_GPRS  9800          //буферизированные параметры
#define ABUF_C_GPRS  10000          //буферизированные параметры
//#define ABUF_IP_PAR  10100          //буферизированные параметры
//карта внутри области
//#define ABUF_IP            ABUF_IP_PAR+OFS_IP
//#define ABUF_PORT          ABUF_IP_PAR+OFS_PORT
//#define ABUF_NUM           ABUF_IP_PAR+OFS_NUM



#define BEG_BUF_LOG 500
#define L_LOG 40


#define A_SOB_OHR 850

#define A_UK_OHR  950
#define A_UK1_OHR 958

#define A_SOST_OHR  964
#define A_SOST1_OHR 966


#define A_IND_DNS  0x3F0
#define A_IND_GZU  0x3F2

#define A_SEG3  0x400

#define A_SEG8  0x440

#define A_SEG9  0x470

#define A_SEG99  0x480    // параметры ГЗУ

#define A_SEG5  0x6000

#define STRUCT_US 0x2000  // архивы ГЗУ

#define A_SEG7  0x2fc0
#define A_SEG6  0x3000

#define STRUCT_TII 0x7000
//#define STRUCT_TII_HEAP 0x7080
#define STRUCT_TII_HEAP A_SEG5


//трафик
#define A_TRAF  29950
#define L_TRAF  14       // длина области без CRC
#define A_TRAF_CRC A_TRAF+L_TRAF-2

//Ключи шифрования
#define A_KEYS  30000
#define L_KEYS  10       // длина области без CRC
#define A_KEYS_CRC A_KEYS+L_KEYS-2

#define A_FIRST_ON   32760














