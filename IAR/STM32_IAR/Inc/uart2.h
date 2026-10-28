#ifndef UART2_H
#define UART2_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart2;
extern uint8_t uart2_rx_byte;
extern uint8_t uart2_tx_byte;

void MX_UART2_Init(void);
HAL_StatusTypeDef UART2_Receive_IT(uint8_t *buffer, uint16_t size);
HAL_StatusTypeDef UART2_TransmitByte_IT(uint8_t byte);
void Uart2_ProcessRxCallback(void);
void Uart2_ProcessTxCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* UART2_H */
