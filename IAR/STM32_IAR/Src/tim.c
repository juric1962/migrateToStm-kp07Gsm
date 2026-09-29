#include "tim.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

void MX_TIM2_Init(void) {
  __HAL_RCC_TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84U - 1U;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 340U - 1U;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_Base_Init(&htim2);
  HAL_TIM_Base_Start_IT(&htim2);
}

void MX_TIM3_Init(void) {
  __HAL_RCC_TIM3_CLK_ENABLE();

  /*
  1 sec
   */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400 -1 ;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000 - 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_Base_Start_IT(&htim3);
}
// 1 mls 
void MX_TIM5_Init(void) {
  __HAL_RCC_TIM5_CLK_ENABLE();

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84U - 1U;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000U - 1U;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_Base_Init(&htim5);
  HAL_TIM_Base_Start_IT(&htim5);
}

/* TIM10 init function free run for delay function*/
void MX_TIM10_Init(void)
{
__HAL_RCC_TIM10_CLK_ENABLE();
  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 167;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */
HAL_TIM_Base_Start(&htim10);
  /* USER CODE END TIM10_Init 2 */

}
