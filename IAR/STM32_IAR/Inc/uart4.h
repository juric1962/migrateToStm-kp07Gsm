#ifndef UART4_H
#define UART4_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart4;
extern uint8_t uart4_rx_byte;
extern uint8_t uart4_tx_byte;

void MX_UART4_Init(void);
HAL_StatusTypeDef UART4_Receive_IT(uint8_t *buffer, uint16_t size);
HAL_StatusTypeDef UART4_TransmitByte_IT(uint8_t byte);
void Uart4_ProcessRxCallback(void);
void Uart4_ProcessTxCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* UART4_H */
