#include "uart4.h"

UART_HandleTypeDef huart4;
uint8_t uart4_rx_byte;
uint8_t uart4_tx_byte;
void MX_UART4_Init(void)
{
    huart4.Instance = UART4;

    huart4.Init.BaudRate = 115200U;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }

    /* Enable UART4 interrupts */
    
HAL_StatusTypeDef UART4_Receive_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Receive_IT(&huart4, buffer, size);
}

HAL_StatusTypeDef UART4_TransmitByte_IT(uint8_t byte) {
  uart4_tx_byte = byte;
  return HAL_UART_Transmit_IT(&huart4, &uart4_tx_byte, 1U);
  
}
