
#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim8;

extern  uint16_t adc_buffer[3];

extern volatile uint16_t Channel1;
extern volatile uint16_t Channel2;


void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_TIM8_Init(void);

HAL_StatusTypeDef ADC1_DMA_Start(void);
HAL_StatusTypeDef ADC1_DMA_Stop(void);

float ADC_GetTemperature(void);

#endif
