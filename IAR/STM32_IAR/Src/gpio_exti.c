#include "gpio_exti.h"

void MX_GPIO_EXTI6_Init(void) {
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};
  gpio_init.Pin = GPIO_PIN_6;
  gpio_init.Mode = GPIO_MODE_IT_FALLING;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &gpio_init);

  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_6) {

  if (sel_modul != 1) {
    if (HAL_GPIO_ReadPin(IO1_PORT, IO1_PIN) == GPIO_PIN_SET)
  
      modbus_mem1[AD_TS] = modbus_mem1[AD_TS] & (~0x01);
    else {
      modbus_mem1[AD_TS] = modbus_mem1[AD_TS] | 0x01;
      // cnt_tii[0]++;
      // modbus_mem1[0]=cnt_tii[0];
      modbus_mem1[AD_TII1]++;
      arr_tii_32[0]++;
    }
  } else {
     if (HAL_GPIO_ReadPin(IO1_PORT, IO1_PIN) == GPIO_PIN_SET)
     return;
    bit_registr1 = bit_registr1 | DS_DETECT;
    // EIMSK=EIMSK & ~ 0x02;    // disable myself
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    
  }
    /* Empty callback: EXTI6 is enabled and ready for application logic. */
  }
}
