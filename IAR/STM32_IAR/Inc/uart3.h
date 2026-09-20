#ifndef UART3_H
#define UART3_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart3;
extern uint8_t uart3_rx_byte;

void MX_UART3_Init(void);
HAL_StatusTypeDef UART3_Receive_IT(uint8_t *buffer, uint16_t size);
HAL_StatusTypeDef UART3_TransmitByte_IT(uint8_t byte);
void Uart3_ProcessRxCallback(void);
void Uart3_ProcessTxCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* UART3_H */
