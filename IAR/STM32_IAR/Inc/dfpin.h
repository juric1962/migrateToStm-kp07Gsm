#include "stm32f4xx_hal.h"

//���� ����� �0 (RS-232 �1) "����������� - GSM-�����"/////////////////////////////////////////////////////////////////



#define RTS0_PORT GPIOA
#define RTS0_PIN  GPIO_PIN_12
#define CTS0_PORT GPIOA
#define CTS0_PIN  GPIO_PIN_11
#define DTR0_PORT GPIOA
#define DTR0_PIN  GPIO_PIN_8
#define DCD0_PORT GPIOC
#define DCD0_PIN  GPIO_PIN_8

#define RTS1_PORT GPIOB
#define RTS1_PIN  GPIO_PIN_12
#define RTS3_PORT GPIOA
#define RTS3_PIN  GPIO_PIN_4

#define RTS2_PORT GPIOC
#define RTS2_PIN  GPIO_PIN_2
#define CTS2_PORT GPIOC
#define CTS2_PIN  GPIO_PIN_3

#define C_SIM1_PORT GPIOC
#define C_SIM1_PIN  GPIO_PIN_10
#define C_SIM2_PORT GPIOC
#define C_SIM2_PIN  GPIO_PIN_11
#define TEN_PORT    GPIOC
#define TEN_PIN     GPIO_PIN_12
#define PWR_PORT    GPIOD
#define PWR_PIN     GPIO_PIN_2

#define S1_R_PORT GPIOB
#define S1_R_PIN  GPIO_PIN_7
#define S1_G_PORT GPIOB
#define S1_G_PIN  GPIO_PIN_6
#define S2_R_PORT GPIOB
#define S2_R_PIN  GPIO_PIN_9
#define S2_G_PORT GPIOB
#define S2_G_PIN  GPIO_PIN_8
#define PWRK_PORT GPIOC
#define PWRK_PIN  GPIO_PIN_13




#define CLR_RTS0\
{\
  HAL_GPIO_WritePin(RTS0_PORT, RTS0_PIN, GPIO_PIN_SET);\
 }

#define SET_RTS0\
{\
  HAL_GPIO_WritePin(RTS0_PORT, RTS0_PIN, GPIO_PIN_RESET);\
 }

#define CLR_DTR0\
{\
  HAL_GPIO_WritePin(DTR0_PORT, DTR0_PIN, GPIO_PIN_SET);\
 }

#define SET_DTR0\
{\
  HAL_GPIO_WritePin(DTR0_PORT, DTR0_PIN, GPIO_PIN_RESET);\
 }



//////////////////////////////////////////////////////////////////////////////////////////////


//���� ����� �1 (RS-485 �1)//////////////////////////////////////////////////////////

#define PIN_OUT_PORT1\
{\
  gpio_init.Pin = RTS1_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(RTS1_PORT, &gpio_init);\
 }

#define SET_RTS1\
{\
  HAL_GPIO_WritePin(RTS1_PORT, RTS1_PIN, GPIO_PIN_SET);\
 }
#define CLR_RTS1\
{\
  HAL_GPIO_WritePin(RTS1_PORT, RTS1_PIN, GPIO_PIN_RESET);\
 }
/////////////////////////////////////////////////////////////////////////



//���� ����� �3 (RS-485 �2)///////////////////////////////////////////////////////////////

#define PIN_OUT_PORT3\
{\
  gpio_init.Pin = RTS3_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(RTS3_PORT, &gpio_init);\
 }

#define SET_RTS3\
{\
  HAL_GPIO_WritePin(RTS3_PORT, RTS3_PIN, GPIO_PIN_SET);\
 }
#define CLR_RTS3\
{\
  HAL_GPIO_WritePin(RTS3_PORT, RTS3_PIN, GPIO_PIN_RESET);\
 }
//////////////////////////////////////////////////////////////////////////////////////////



//���� ����� �2 (RS-232 �2) /////////////////////////////////////////////////////////////



//������������ ��� �� �����
#define PIN_OUT_PORT2\
{\
  gpio_init.Pin = RTS2_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(RTS2_PORT, &gpio_init);\
 }

#define CLR_RTS2\
{\
  HAL_GPIO_WritePin(RTS2_PORT, RTS2_PIN, GPIO_PIN_SET);\
 }

#define SET_RTS2\
{\
  HAL_GPIO_WritePin(RTS2_PORT, RTS2_PIN, GPIO_PIN_RESET);\
 }


/////////////////////////////////////////////////////////////////////////////////////////////////


//���� ��������� ������� ������/////////////////////////////////
//������������ ��� �� �����
#define PIN_OUT_PWR\
{\
  gpio_init.Pin = PWR_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(PWR_PORT, &gpio_init);\
 }

#define SET_PWR\
{\
  HAL_GPIO_WritePin(PWR_PORT, PWR_PIN, GPIO_PIN_SET);\
 }

#define CLR_PWR\
{\
  HAL_GPIO_WritePin(PWR_PORT, PWR_PIN, GPIO_PIN_RESET);\
 }
/////////////////////////////////////////////////////////////////////////////////




//���� ��������� ������/////////////////////////////////

//������������ ��� �� �����
#define PIN_OUT_PWRK\
{\
  gpio_init.Pin = PWRK_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(PWRK_PORT, &gpio_init);\
 }

#define SET_PWRK\
{\
  HAL_GPIO_WritePin(PWRK_PORT, PWRK_PIN, GPIO_PIN_SET);\
 }

#define CLR_PWRK\
{\
  HAL_GPIO_WritePin(PWRK_PORT, PWRK_PIN, GPIO_PIN_RESET);\
 }
/////////////////////////////////////////////////////////////////////////////////


//���� ������ TC////////////////////////////////////////////////////////////////////



//���� IO1-10]
#define IO1  GPIO_PIN_6
#define IO2  GPIO_PIN_15
#define IO3  GPIO_PIN_14
#define IO4  GPIO_PIN_4
#define IO5  GPIO_PIN_13
#define IO6  GPIO_PIN_5
#define IO7  GPIO_PIN_6
#define IO8  GPIO_PIN_7
#define IO9  GPIO_PIN_5
#define IO10 GPIO_PIN_0
#define IO11 GPIO_PIN_1
#define IO12 GPIO_PIN_2

#define IO1_PORT GPIOC
#define IO1_PIN  GPIO_PIN_6

#define IO2_PORT GPIOB
#define IO2_PIN  GPIO_PIN_15

#define IO3_PORT GPIOB
#define IO3_PIN  GPIO_PIN_14

#define IO4_PORT GPIOB
#define IO4_PIN  GPIO_PIN_13

#define IO5_PORT GPIOA
#define IO5_PIN  GPIO_PIN_5

#define IO6_PORT GPIOA
#define IO6_PIN  GPIO_PIN_6

#define IO7_PORT GPIOA
#define IO7_PIN  GPIO_PIN_7

#define IO8_PORT GPIOC
#define IO8_PIN  GPIO_PIN_4

#define IO9_PORT GPIOC
#define IO9_PIN  GPIO_PIN_5

#define IO10_PORT GPIOB
#define IO10_PIN  GPIO_PIN_0

#define IO11_PORT GPIOB
#define IO11_PIN  GPIO_PIN_1

#define IO12_PORT GPIOB
#define IO12_PIN  GPIO_PIN_2

#define TCH_I  GPIO_PIN_6

#define PIN_IN_TS1_8\
{\
  GPIO_InitTypeDef gpio_init = {0};\
  gpio_init.Mode = GPIO_MODE_INPUT;\
  gpio_init.Pull = GPIO_PULLUP;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  gpio_init.Pin = IO1_PIN; HAL_GPIO_Init(IO1_PORT, &gpio_init);\
  gpio_init.Pin = IO2_PIN; HAL_GPIO_Init(IO2_PORT, &gpio_init);\
  gpio_init.Pin = IO6_PIN; HAL_GPIO_Init(IO6_PORT, &gpio_init);\
  gpio_init.Pin = IO8_PIN; HAL_GPIO_Init(IO8_PORT, &gpio_init);\
  gpio_init.Pin = IO3_PIN; HAL_GPIO_Init(IO3_PORT, &gpio_init);\
  gpio_init.Pin = IO5_PIN; HAL_GPIO_Init(IO5_PORT, &gpio_init);\
  gpio_init.Pin = IO7_PIN; HAL_GPIO_Init(IO7_PORT, &gpio_init);\
  gpio_init.Pin = IO4_PIN; HAL_GPIO_Init(IO4_PORT, &gpio_init);\
  
}

#define PIN_IN_MKD1_5\
{\
  GPIO_InitTypeDef gpio_init = {0};\
  gpio_init.Mode = GPIO_MODE_INPUT;\
  gpio_init.Pull = GPIO_PULLUP;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  gpio_init.Pin = IO6_PIN; HAL_GPIO_Init(IO6_PORT, &gpio_init);\
  gpio_init.Pin = IO8_PIN; HAL_GPIO_Init(IO8_PORT, &gpio_init);\
  gpio_init.Pin = IO3_PIN; HAL_GPIO_Init(IO3_PORT, &gpio_init);\
  gpio_init.Pin = IO5_PIN; HAL_GPIO_Init(IO5_PORT, &gpio_init);\
  gpio_init.Pin = IO4_PIN; HAL_GPIO_Init(IO4_PORT, &gpio_init);\
  gpio_init.Pin = IO2_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(IO2_PORT, &gpio_init);\
  gpio_init.Pin = IO7_PIN;\
  HAL_GPIO_Init(IO7_PORT, &gpio_init);\
}

#define PIN_OUT_TU\
{\
  GPIO_InitTypeDef gpio_init = {0};\
  gpio_init.Pin = IO11_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(IO11_PORT, &gpio_init);\
  gpio_init.Pin = IO12_PIN;\
  HAL_GPIO_Init(IO12_PORT, &gpio_init);\
}


#define TU2_ON\
{\
  HAL_GPIO_WritePin(IO12_PORT, IO12_PIN, GPIO_PIN_SET);\
}

#define TU2_OFF\
{\
  HAL_GPIO_WritePin(IO12_PORT, IO12_PIN, GPIO_PIN_RESET);\
}

#define SOUND_ON\
{\
  HAL_GPIO_WritePin(IO12_PORT, IO12_PIN, GPIO_PIN_SET);\
}

#define SOUND_OFF\
{\
  HAL_GPIO_WritePin(IO12_PORT, IO12_PIN, GPIO_PIN_RESET);\
}

#define TU1_ON\
{\
  HAL_GPIO_WritePin(IO11_PORT, IO11_PIN, GPIO_PIN_SET);\
}

#define TU1_OFF\
{\
  HAL_GPIO_WritePin(IO11_PORT, IO11_PIN, GPIO_PIN_RESET);\
}

#define ST_SHL_ON\
{\
  HAL_GPIO_WritePin(IO11_PORT, IO11_PIN, GPIO_PIN_SET);\
}

#define ST_SHL_OFF\
{\
  HAL_GPIO_WritePin(IO11_PORT, IO11_PIN, GPIO_PIN_RESET);\
}

#define PIN_OUT_MKD\
{\
  GPIO_InitTypeDef gpio_init = {0};\
  gpio_init.Pin = IO2_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(IO2_PORT, &gpio_init);\
\
  gpio_init.Pin = IO7_PIN;\
  HAL_GPIO_Init(IO7_PORT, &gpio_init);\
}

#define TCH_O_ONE\
{\
  HAL_GPIO_WritePin(IO2_PORT, IO2_PIN, GPIO_PIN_SET);\
}

#define TCH_O_ZERO\
{\
  HAL_GPIO_WritePin(IO2_PORT, IO2_PIN, GPIO_PIN_RESET);\
}

#define SOST_ON\
{\
  HAL_GPIO_WritePin(IO7_PORT, IO7_PIN, GPIO_PIN_SET);\
}

#define SOST_OFF\
{\
  HAL_GPIO_WritePin(IO7_PORT, IO7_PIN, GPIO_PIN_RESET);\
}

//���� ���������� SIM////////////////////////////////////////////////////////////////////


#define PIN_OUT_SIM\
{\
  gpio_init.Pin = C_SIM1_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(C_SIM1_PORT, &gpio_init);\
  gpio_init.Pin = C_SIM2_PIN;\
  HAL_GPIO_Init(C_SIM2_PORT, &gpio_init);\
 }

#define SET_SIM1\
{\
  HAL_GPIO_WritePin(C_SIM2_PORT, C_SIM2_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(C_SIM1_PORT, C_SIM1_PIN, GPIO_PIN_SET);\
 }

#define SET_SIM2\
{\
  HAL_GPIO_WritePin(C_SIM1_PORT, C_SIM1_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(C_SIM2_PORT, C_SIM2_PIN, GPIO_PIN_SET);\
 }
/////////////////////////////////////////////////////////




//���� ��������� TEN /////////////////////////////////

//������������ ��� �� �����
#define PIN_OUT_TEN\
{\
  gpio_init.Pin = TEN_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(TEN_PORT, &gpio_init);\
 }

#define SET_TEN\
{\
  HAL_GPIO_WritePin(TEN_PORT, TEN_PIN, GPIO_PIN_SET);\
 }

#define CLR_TEN\
{\
  HAL_GPIO_WritePin(TEN_PORT, TEN_PIN, GPIO_PIN_RESET);\
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//���� ����������� /////////////////////////////////


//������������ ��� �� �����
#define PIN_OUT_S1\
{\
  gpio_init.Pin = S1_R_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(S1_R_PORT, &gpio_init);\
  gpio_init.Pin = S1_G_PIN;\
  HAL_GPIO_Init(S1_G_PORT, &gpio_init);\
 }

#define PIN_OUT_S2_S5\
{\
  gpio_init.Pin = S2_R_PIN;\
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;\
  gpio_init.Pull = GPIO_NOPULL;\
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;\
  HAL_GPIO_Init(S2_R_PORT, &gpio_init);\
  gpio_init.Pin = S2_G_PIN;\
  HAL_GPIO_Init(S2_G_PORT, &gpio_init);\

 }
///S1
#define S1_OFF\
{\
  HAL_GPIO_WritePin(S1_R_PORT, S1_R_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(S1_G_PORT, S1_G_PIN, GPIO_PIN_RESET);\
 }
#define S1_YL\
{\
  HAL_GPIO_WritePin(S1_R_PORT, S1_R_PIN, GPIO_PIN_SET);\
  HAL_GPIO_WritePin(S1_G_PORT, S1_G_PIN, GPIO_PIN_SET);\
 }
#define S1_RD\
{\
  HAL_GPIO_WritePin(S1_R_PORT, S1_R_PIN, GPIO_PIN_SET);\
  HAL_GPIO_WritePin(S1_G_PORT, S1_G_PIN, GPIO_PIN_RESET);\
 }
#define S1_GR\
{\
  HAL_GPIO_WritePin(S1_R_PORT, S1_R_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(S1_G_PORT, S1_G_PIN, GPIO_PIN_SET);\
 }
#define S1_CH\
{\
  HAL_GPIO_TogglePin(S1_R_PORT, S1_R_PIN);\
  HAL_GPIO_TogglePin(S1_G_PORT, S1_G_PIN);\
 }


///S2
#define S2_OFF\
{\
  HAL_GPIO_WritePin(S2_R_PORT, S2_R_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(S2_G_PORT, S2_G_PIN, GPIO_PIN_RESET);\
 }
#define S2_YL\
{\
  HAL_GPIO_WritePin(S2_R_PORT, S2_R_PIN, GPIO_PIN_SET);\
  HAL_GPIO_WritePin(S2_G_PORT, S2_G_PIN, GPIO_PIN_SET);\
 }
#define S2_RD\
{\
  HAL_GPIO_WritePin(S2_R_PORT, S2_R_PIN, GPIO_PIN_SET);\
  HAL_GPIO_WritePin(S2_G_PORT, S2_G_PIN, GPIO_PIN_RESET);\
 }
#define S2_GR\
{\
  HAL_GPIO_WritePin(S2_R_PORT, S2_R_PIN, GPIO_PIN_RESET);\
  HAL_GPIO_WritePin(S2_G_PORT, S2_G_PIN, GPIO_PIN_SET);\
 }
#define S2_CH\
{\
  HAL_GPIO_TogglePin(S2_R_PORT, S2_R_PIN);\
  HAL_GPIO_TogglePin(S2_G_PORT, S2_G_PIN);\
 }


///S3
#define S3_OFF\
{\
  __NOP();\
   }
#define S3_YL\
{\
  __NOP();\
   }
#define S3_RD\
{\
  __NOP();\
   }
#define S3_GR\
{\
  __NOP();\
   }
#define S3_CH\
{\
  __NOP();\
  }


///S4
#define S4_OFF\
{\
  __NOP();\
  }
#define S4_YL\
{\
  __NOP();\
  }
#define S4_RD\
{\
  __NOP();\
  }
#define S4_GR\
{\
  __NOP();\
  }
#define S4_CH\
{\
  __NOP();\
  }

///S5
#define S5_OFF\
{\
  __NOP();\
  }
#define S5_YL\
{\
  __NOP();\
  }
#define S5_RD\
{\
  __NOP();\
  }
#define S5_GR\
{\
  __NOP();\
   }
#define S5_CH\
{\
  __NOP();\
  }





///////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// stm32 tc define
#define TS1_PORT   GPIOC
#define TS1_PIN    GPIO_PIN_6

#define TS2_PORT   GPIOB
#define TS2_PIN    GPIO_PIN_15

#define TS3_PORT   GPIOB
#define TS3_PIN    GPIO_PIN_14

#define TS4_PORT   GPIOC
#define TS4_PIN    GPIO_PIN_4

#define TS5_PORT   GPIOB
#define TS5_PIN    GPIO_PIN_13

#define TS6_PORT   GPIOA
#define TS6_PIN    GPIO_PIN_5

#define TS7_PORT   GPIOA
#define TS7_PIN    GPIO_PIN_6

#define TS8_PORT   GPIOA
#define TS8_PIN    GPIO_PIN_7


#define TSS1_PORT   GPIOC
#define TSS1_PIN    GPIO_PIN_7

#define TSS2_PORT   GPIOC
#define TSS2_PIN    GPIO_PIN_1

#define TU1_PORT   GPIOB
#define TU1_PIN    GPIO_PIN_1

#define TU2_PORT   GPIOB
#define TU2_PIN    GPIO_PIN_2


