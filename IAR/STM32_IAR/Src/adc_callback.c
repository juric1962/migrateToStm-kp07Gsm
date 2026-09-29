#include "adc_callback.h"
#include "stm32f4xx_hal.h"

/* Buffer and channel variables are defined in adc_dma.c */
extern uint16_t adc_buffer[3];
extern volatile uint16_t Channel1;
extern volatile uint16_t Channel2;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  float vsense;
  uint16_t adc_raw;
  if (hadc == NULL)
    return;

  if (hadc->Instance == ADC1) {
    /* adc_buffer[0] -> ADC1_IN15, adc_buffer[1] -> ADC1_IN8 */
    Channel1 = adc_buffer[0];
    Channel2 = adc_buffer[1];
    adc_raw = adc_buffer[2]; // Internal temperature sensor value
    modbus_mem1[AD_TEMP] = adc_raw;
    // Для Vref = 3.3 В и ADC 10 бит
     vsense = ((float)adc_raw * 3.3f) / 1024.0f; 
     temperatura = (((vsense - 0.76f) / 0.0025f) +25.0f);
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
