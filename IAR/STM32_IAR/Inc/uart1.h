#ifndef UART1_H
#define UART1_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart1;
extern uint8_t uart1_rx_byte;

void MX_UART1_Init(void);
HAL_StatusTypeDef UART1_Receive_IT(uint8_t *buffer, uint16_t size);
HAL_StatusTypeDef UART1_Transmit_IT(uint8_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* UART1_H */
