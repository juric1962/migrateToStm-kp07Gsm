/* Includes -------------------------------------------------*/
#include "stm32f4xx_hal.h"

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    if (uartHandle->Instance == USART1)
    {
        /* USART1 clock */
        __HAL_RCC_USART1_CLK_ENABLE();

        /* GPIOA clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /*
         * USART1
         * PA9  = TX
         * PA10 = RX
         * AF7
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt */
        HAL_NVIC_SetPriority(USART1_IRQn, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    } else if (uartHandle->Instance == USART2)
    {
        /* Peripheral clock */
        __HAL_RCC_USART2_CLK_ENABLE();

        /* GPIO clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /*
         * USART2
         * PA2 = TX
         * PA3 = RX
         * AF7
         */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 interrupt */
        HAL_NVIC_SetPriority(USART2_IRQn, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }else if (uartHandle->Instance == UART4)
    {
        /* UART4 clock */
        __HAL_RCC_UART4_CLK_ENABLE();

        /* GPIOA clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /*
         * UART4
         * PA0 = TX
         * PA1 = RX
         * AF8
         */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART4 interrupt */
        HAL_NVIC_SetPriority(UART4_IRQn, 0U, 0U);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
    }else if (uartHandle->Instance == USART3)
    {
        /* USART3 clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        /* GPIOB clock enable */
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /*
         * USART3:
         * PB10 = TX
         * PB11 = RX
         * AF7
         */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 interrupt */
        HAL_NVIC_SetPriority(USART3_IRQn, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }

}


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

      if (uartHandle->Instance == USART1)
    {
        /* USART1 clock */
        __HAL_RCC_USART1_CLK_DISABLE();

        /* GPIO */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USART1 interrupt */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    } else if (uartHandle->Instance == USART2)
    {
        /* Peripheral clock */
        __HAL_RCC_USART2_CLK_DISABLE();

        /* GPIO */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

        /* USART2 interrupt */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }else if (uartHandle->Instance == UART4)
    {
        /* UART4 clock */
        __HAL_RCC_UART4_CLK_DISABLE();

        /* GPIO */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1);

        /* UART4 interrupt */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }else if (uartHandle->Instance == USART3)
    {
        /* USART3 clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /* GPIOB de-init */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

        /* USART3 interrupt disable */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }
}