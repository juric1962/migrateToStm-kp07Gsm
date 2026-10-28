#include "uart2.h"

UART_HandleTypeDef huart2;
uint8_t uart2_rx_byte;
uint8_t uart2_tx_byte;
void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;

    huart2.Init.BaudRate = 115200U;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

HAL_StatusTypeDef UART2_Receive_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Receive_IT(&huart2, buffer, size);
}

HAL_StatusTypeDef UART2_TransmitByte_IT(uint8_t byte) {
  uart2_tx_byte = byte;
  return HAL_UART_Transmit_IT(&huart2, &uart2_tx_byte, 1U);
}



