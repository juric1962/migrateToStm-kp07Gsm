#include "gpio_inputs.h"

void MX_GPIO_TCs_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};

  gpio_init.Mode = GPIO_MODE_INPUT;
  gpio_init.Pull = GPIO_PULLUP;


  gpio_init.Pin = TSS1_PIN;
  HAL_GPIO_Init(TSS1_PORT, &gpio_init);

  gpio_init.Pin = TSS2_PIN;
  HAL_GPIO_Init(TSS2_PORT, &gpio_init);

  gpio_init.Pin = CTS2_PIN;
  HAL_GPIO_Init(CTS2_PORT, &gpio_init);

  gpio_init.Pin = CTS0_PIN;
  HAL_GPIO_Init(CTS0_PORT, &gpio_init);

  gpio_init.Pin = DSR0_PIN;
  HAL_GPIO_Init(DSR0_PORT, &gpio_init);

  gpio_init.Pin = DCD0_PIN;
  HAL_GPIO_Init(DCD0_PORT, &gpio_init);

///// общие для двух мод выходные ноги


  HAL_GPIO_WritePin(TU1_PORT, TU1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TU2_PORT, TU2_PIN, GPIO_PIN_RESET);

  gpio_init.Pin = TU1_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TU1_PORT, &gpio_init);

  gpio_init.Pin = TU2_PIN;
  HAL_GPIO_Init(TU2_PORT, &gpio_init);



  // Конфигурирование ног порта №1 RS485-1
  //PIN_OUT_PORT1;
  CLR_RTS1;
  gpio_init.Pin = RTS1_PIN;
  HAL_GPIO_Init(RTS1_PORT, &gpio_init);

  // Конфигурирование ног порта №2 RS232-2
  //PIN_OUT_PORT2;
  
  CLR_RTS2;
  gpio_init.Pin = RTS2_PIN;
  HAL_GPIO_Init(RTS2_PORT, &gpio_init);

  // Конфигурирование ног порта №3 RS485-2
  //PIN_OUT_PORT3;
  CLR_RTS3;
  gpio_init.Pin = RTS3_PIN;
  HAL_GPIO_Init(RTS3_PORT, &gpio_init);

  // конфигурирование ног управления SIM
  //PIN_OUT_SIM;
  SET_SIM1;
  gpio_init.Pin = C_SIM1_PIN;
  HAL_GPIO_Init(C_SIM1_PORT, &gpio_init);
  gpio_init.Pin = C_SIM2_PIN;
  HAL_GPIO_Init(C_SIM2_PORT, &gpio_init);

  // конфигурирование ноги включения питания модема
 // PIN_OUT_PWR;
  CLR_PWR;
  gpio_init.Pin = PWR_PIN;
  HAL_GPIO_Init(PWR_PORT, &gpio_init);

  // конфигурирование ноги включение модема
  //PIN_OUT_PWRK;
  //SET_PWRK;
  gpio_init.Pin = PWRK_PIN;
  HAL_GPIO_Init(PWRK_PORT, &gpio_init);



  // конфигурирование ноги TEN
  //PIN_OUT_TEN;
  CLR_TEN;
  gpio_init.Pin = TEN_PIN;
  HAL_GPIO_Init(TEN_PORT, &gpio_init);

  // конфигурация ног светодиодов
  //PIN_OUT_S1;
  gpio_init.Pin = S1_R_PIN;
  HAL_GPIO_Init(S1_R_PORT, &gpio_init);
  gpio_init.Pin = S1_G_PIN;
  HAL_GPIO_Init(S1_G_PORT, &gpio_init);


  //PIN_OUT_S2_S5;
  gpio_init.Pin = S2_R_PIN;
  HAL_GPIO_Init(S2_R_PORT, &gpio_init);
  gpio_init.Pin = S2_G_PIN;
  HAL_GPIO_Init(S2_G_PORT, &gpio_init);
  S1_OFF;
  S2_OFF;
  S3_OFF;
  S4_OFF;
  S5_OFF;

}
