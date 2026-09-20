#include "uart4.h"

UART_HandleTypeDef huart4;

void MX_UART4_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_UART4_CLK_ENABLE();

  GPIO_InitTypeDef gpio = {0};

  gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &gpio);

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);

  __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart4, UART_IT_TXE);

  HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

uint8_t uart4_rx_byte;

void Uart4_ProcessRxCallback(void)
{
    uint8_t data;


    data = uart4_rx_byte;

    if ((Regim == MODEM_ONLY) || (Regim == MODEM_ONLY_R))
    {
        HAL_UART_Transmit(&huart1, &data, 1, 10);
    }
    else if (fl_at_mom_232 == 1)
    {
        if (vol_tx_ppp >= VOL_TX_PPP)
            vol_tx_ppp = 0;

        buf_tx_232[vol_tx_ppp] = data;
        vol_tx_ppp++;
    }
    else
    {
        S5_GR;

        Rs232_2.cnt_tm_rx_out = Rs232_2.vol_tm_rx_out;
        Rs232_2.cnt_tm_out = 0;

        if (Rs232_2.cnt_bt_rx_tx < MAX_BUF_RS232_2)
        {
            Rs232_2_buf_rx_tx[Rs232_2.cnt_bt_rx_tx] = data;
            Rs232_2.cnt_bt_rx_tx++;
        }
        else
        {
            fl_232_2.over = 1;
        }
    }

    HAL_UART_Receive_IT(&huart4, &uart4_rx_byte, 1);
}

uint8_t uart4_tx_byte;
void Uart4_ProcessTxCallback()
{
    

    if (Rs232_2.cnt_bt_rx_tx == 0)
    {
        // Передача закончена
        Rs232_2.cnt_bt_rx_tx = 0;

        Rs232_2.cnt_tm_tx_out = Rs232_2.vol_tm_tx_out;

        if (Rs232_2.cnt_tm_tx_out == 0)
        {
            S5_OFF;

            Rs232_2.cnt_tm_out = Rs232_2.vol_tm_out;

            CLR_RTS2;

            // Переход в режим приема
            HAL_UART_Receive_IT(&huart4, &uart4_rx_byte, 1);
        }

        return;
    }

    // Передаем следующий байт
    uart4_tx_byte = *Rs232_2.p_data485++;

    Rs232_2.cnt_bt_rx_tx--;

    HAL_UART_Transmit_IT(&huart4, &uart4_tx_byte, 1);
}



void HAL_UART4_IRQHandler(void) {
  HAL_UART_IRQHandler(&huart4);
}
