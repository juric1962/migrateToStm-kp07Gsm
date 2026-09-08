#ifndef TIM_H
#define TIM_H

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM5_Init(void);

#endif
