#include "uart3.h"

#include "dfpin.h"
#include "rs485_state.h"

UART_HandleTypeDef huart3;
uint8_t uart3_rx_byte;
static uint8_t uart3_tx_byte;
void MX_USART3_UART_Init(void)
{
    huart3.Instance = USART3;

    huart3.Init.BaudRate = 115200U;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}

HAL_StatusTypeDef UART3_Receive_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Receive_IT(&huart3, buffer, size);
}

HAL_StatusTypeDef UART3_TransmitByte_IT(uint8_t byte) {
  uart3_tx_byte = byte;
  return HAL_UART_Transmit_IT(&huart3, &uart3_tx_byte, 1U);
}

void USART3_IRQHandler(void) {
  HAL_UART_IRQHandler(&huart3);
}

void Uart3_ProcessRxCallback(void) {
  unsigned char data = uart3_rx_byte;

  S4_GR;
  Rs485_2.cnt_tm_rx_out = Rs485_2.vol_tm_rx_out;
  Rs485_2.cnt_tm_out = 0;

  if (Rs485_2.cnt_bt_rx_tx < MAX_BUF_RS485_2) {
    Rs485_2_buf_rx_tx[Rs485_2.cnt_bt_rx_tx] = data;
    Rs485_2.cnt_bt_rx_tx++;
  } else {
    fl_485_2.over = 1;
  }

  HAL_UART_Receive_IT(&huart3, &uart3_rx_byte, 1U);
}

void Uart3_ProcessTxCallback(void) {
  if (Rs485_2.cnt_bt_rx_tx == 0U) {
    Rs485_2.cnt_bt_rx_tx = 0U;
    Rs485_2.cnt_tm_tx_out = Rs485_2.vol_tm_tx_out;

    if (Rs485_2.cnt_tm_tx_out == 0U) {
      S4_OFF;
      Rs485_2.cnt_tm_out = Rs485_2.vol_tm_out;
      CLR_RTS3;
      HAL_UART_Receive_IT(&huart3, &uart3_rx_byte, 1U);
    }
    return;
  }

  UART3_TransmitByte_IT(*Rs485_2.p_data485++);
  Rs485_2.cnt_bt_rx_tx--;
}
