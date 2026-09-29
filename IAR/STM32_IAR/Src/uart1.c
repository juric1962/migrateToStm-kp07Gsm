#include "uart1.h"

UART_HandleTypeDef huart1;
uint8_t uart1_rx_byte;
void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200U;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

HAL_StatusTypeDef UART1_Receive_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Receive_IT(&huart1, buffer, size);
}

HAL_StatusTypeDef UART1_Transmit_IT(uint8_t *buffer, uint16_t size) {
  return HAL_UART_Transmit_IT(&huart1, buffer, size);
}

void USART1_IRQHandler(void) {
  HAL_UART_IRQHandler(&huart1);
}


uint8_t  uart1_rx_byte;
void Uart1_ProcessRxCallback(void) {
   uint8_t data;


    data = uart1_rx_byte;

    if ((Regim == MODEM_ONLY) || (Regim == MODEM_ONLY_R))
    {
        HAL_UART_Transmit(&huart4, &data, 1, 10);

        HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
        return;
    }

    if (Regim == RG_DEBAG)
    {
        HAL_UART_Transmit(&huart3, &data, 1, 10);
    }

    if (fl_at_mom_232 == 1)
    {
        Appl_seq_buf[point_Head] = data;

        point_Head++;
        point_Head &= 0x3F;

        HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
        return;
    }

    if (command_AT == TRUE)
    {
        if (fl_at_com.rx_en == 0)
        {
            HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
            return;
        }

        S2_GR;

        At_com.cnt_rx_out = At_com.vol_rx_out;
        At_com.cnt_tm_out = 0;

        if (At_com.cnt_rx < LN_BUF_AT)
        {
            At_com.buf[At_com.cnt_rx] = data;
            At_com.cnt_rx++;
        }

        HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
        return;
    }

    cnt_incom++;

    if (fl_rx_ppp.switcher == 0)
        recive_buf1(data);
    else
        recive_buf2(data);
      if (Buf2_rx_ppp.check_busy == TRUE) return;
      if (Buf1_rx_ppp.check_busy == TRUE) return;

    HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
}

void Uart1_ProcessTxCallback(void) {
  
  if (command_AT == TRUE)
    sending_at_pac();

  else {

    if (fl_cts_232_ignor == TRUE) {
      sending_ppp_pac();
      return;
    }

    cnt_outcom++;

    if (check_cts() == 1)
      return;
    sending_ppp_pac();
  }
}
