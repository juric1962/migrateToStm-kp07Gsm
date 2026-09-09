#include "adc_callback.h"
#include "stm32f4xx_hal.h"

/* Buffer and channel variables are defined in adc_dma.c */
extern uint16_t adc_buffer[2];
extern volatile uint16_t Channel1;
extern volatile uint16_t Channel2;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc == NULL)
    return;

  if (hadc->Instance == ADC1) {
    /* adc_buffer[0] -> ADC1_IN15, adc_buffer[1] -> ADC1_IN8 */
    Channel1 = adc_buffer[0];
    Channel2 = adc_buffer[1];
    summa_adc[0] = summa_adc[0] + Channel1;
    summa_adc[1] = summa_adc[1] + Channel2;
    count_summa_adc[0]++;
    if (count_summa_adc[0] == 10) {
      count_summa_adc[0] = 0;
      modbus_mem1[AD_TIT1] = summa_adc[0] / 10;
      modbus_mem1[AD_TIT1 + 1] = summa_adc[1] / 10;
    }
  }
}
