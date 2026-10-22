#include "stm32f4xx_hal.h"
IWDG_HandleTypeDef hiwdg;
// IWDG init function
static void MX_IWDG_Init (void){
hiwdg.Instance = IWDG;
hiwdg.Init.Prescaler =IWDG_PRESCALER_32:
hiwdg.Init.Reload = 3000;
iF (HAL_IWDG_Init (&hiwdg) != HAL OK){
while (1) ;
}
}
