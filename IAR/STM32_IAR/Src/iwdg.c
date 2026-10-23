#include "stm32f4xx_hal.h"
IWDG_HandleTypeDef hiwdg;
// IWDG init function
void MX_IWDG_Init (void){
hiwdg.Instance = IWDG;
hiwdg.Init.Prescaler =IWDG_PRESCALER_32;
hiwdg.Init.Reload = 3000;
if (HAL_IWDG_Init (&hiwdg) != HAL_OK){
while (1) ;
}
}
