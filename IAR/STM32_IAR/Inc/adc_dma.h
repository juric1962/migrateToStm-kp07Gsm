#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;

/* ADC buffer: adc_buffer[0] = ADC1_IN15, adc_buffer[1] = ADC1_IN8 */
extern uint16_t adc_buffer[2];

/* Latest channel values updated from DMA complete callback */
extern volatile uint16_t Channel1;
extern volatile uint16_t Channel2;

void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_TIM1_Init(void);

/* Start ADC in DMA circular mode and start TIM1 counter */
void ADC1_DMA_Start(void);

#endif
