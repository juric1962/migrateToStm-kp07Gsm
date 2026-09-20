#include "gpio_inputs.h"

void MX_GPIO_TCs_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};

  gpio_init.Mode = GPIO_MODE_INPUT;
  gpio_init.Pull = GPIO_NOPULL;

  gpio_init.Pin = TS1_PIN;
  HAL_GPIO_Init(TS1_PORT, &gpio_init);

  gpio_init.Pin = TS2_PIN;
  HAL_GPIO_Init(TS2_PORT, &gpio_init);

  gpio_init.Pin = TS3_PIN;
  HAL_GPIO_Init(TS3_PORT, &gpio_init);

  gpio_init.Pin = TS4_PIN;
  HAL_GPIO_Init(TS4_PORT, &gpio_init);

  gpio_init.Pin = TS5_PIN;
  HAL_GPIO_Init(TS5_PORT, &gpio_init);

  gpio_init.Pin = TS6_PIN;
  HAL_GPIO_Init(TS6_PORT, &gpio_init);

  gpio_init.Pin = TS7_PIN;
  HAL_GPIO_Init(TS7_PORT, &gpio_init);

  gpio_init.Pin = TS8_PIN;
  HAL_GPIO_Init(TS8_PORT, &gpio_init);

  gpio_init.Pin = TSS1_PIN;
  HAL_GPIO_Init(TSS1_PORT, &gpio_init);

  gpio_init.Pin = TSS2_PIN;
  HAL_GPIO_Init(TSS2_PORT, &gpio_init);
}

void MX_GPIO_TU_Init(void) {
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {0};
  gpio_init.Pin = TU1_PIN;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TU1_PORT, &gpio_init);

  gpio_init.Pin = TU2_PIN;
  HAL_GPIO_Init(TU2_PORT, &gpio_init);

  HAL_GPIO_WritePin(TU1_PORT, TU1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TU2_PORT, TU2_PIN, GPIO_PIN_RESET);
}
