#include "uart2.h"

UART_HandleTypeDef huart2;
uint8_t uart2_rx_byte;
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

HAL_StatusTypeDef UART2_Transmit_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Transmit_IT(&huart2, buffer, size);
}

void USART2_IRQHandler(void) {
  HAL_UART_IRQHandler(&huart2);
}


uint8_t  uart2_rx_byte;
void Uart2_ProcessRxCallback(void) {

    uint8_t data = uart2_rx_byte;

    S3_GR;

    Rs485_1.cnt_tm_rx_out = Rs485_1.vol_tm_rx_out;
    Rs485_1.cnt_tm_out = 0;

    if (Rs485_1.cnt_bt_rx_tx < MAX_BUF_RS485_1)
    {
        Rs485_1_buf_rx_tx[Rs485_1.cnt_bt_rx_tx] = data;
        Rs485_1.cnt_bt_rx_tx++;
    }
    else
    {
        fl_485_1.over = 1;
    }

    // Приём следующего байта
    HAL_UART_Receive_IT(&huart2, &uart2_rx_byte, 1);
}

uint8_t uart2_tx_byte;
void Uart2_ProcessTxCallback(void) {


    if (Rs485_1.cnt_bt_rx_tx != 0)
    {
        uart2_tx_byte = *Rs485_1.p_data485++;

        Rs485_1.cnt_bt_rx_tx--;

        HAL_UART_Transmit_IT(&huart2, &uart2_tx_byte, 1);

        return;
    }

    // Передача закончена
    Rs485_1.cnt_bt_rx_tx = 0;

    Rs485_1.cnt_tm_tx_out = Rs485_1.vol_tm_tx_out;

    if (Rs485_1.cnt_tm_tx_out == 0)
    {
        S3_OFF;

        Rs485_1.cnt_tm_out = Rs485_1.vol_tm_out;

        CLR_RTS1;

        // Переход в режим приема
        HAL_UART_Receive_IT(&huart2, &uart2_rx_byte, 1);
    }
}