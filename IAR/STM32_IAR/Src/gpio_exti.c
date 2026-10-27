#include "gpio_exti.h"

void MX_GPIO_EXTI6_Init(void) {
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};
  gpio_init.Pin = GPIO_PIN_6;
  //gpio_init.Mode = GPIO_MODE_IT_FALLING;
  gpio_init.Mode = GPIO_MODE_IT_RISING_FALLING;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &gpio_init);

  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

