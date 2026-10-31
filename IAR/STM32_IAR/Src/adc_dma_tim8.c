
#include "adc_dma_tim8.h"
#include "main.h"

/* ---------------------------------------------------------
 * Global handles
 * --------------------------------------------------------- */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim8;

/* ---------------------------------------------------------
 * ADC data
 * --------------------------------------------------------- */

 uint16_t adc_buffer[3] = {0};

volatile uint16_t Channel1 = 0;
volatile uint16_t Channel2 = 0;


/* ---------------------------------------------------------
 * DMA initialization
 * --------------------------------------------------------- */

void MX_DMA_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();

    hdma_adc1.Instance = DMA2_Stream0;

    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;

    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;

    hdma_adc1.Init.PeriphDataAlignment =
        DMA_PDATAALIGN_HALFWORD;

    hdma_adc1.Init.MemDataAlignment =
        DMA_MDATAALIGN_HALFWORD;

    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;

    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/* ---------------------------------------------------------
 * TIM8 initialization
 *
 * TIM8 -> TRGO -> ADC1
 * Trigger frequency: 10 kHz
 * --------------------------------------------------------- */

void MX_TIM8_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    uint32_t pclk2;
    uint32_t timerclk;
    uint32_t prescaler;

    __HAL_RCC_TIM8_CLK_ENABLE();

    pclk2 = HAL_RCC_GetPCLK2Freq();

    /*
     * Standard STM32F405 clock configuration:
     * APB2 prescaler = 1: TIM8CLK = PCLK2
     * APB2 prescaler > 1: TIM8CLK = 2 * PCLK2
     *
     * Assumes standard TIMPRE configuration.
     */
    timerclk = (pclk2 == SystemCoreClock)
             ? pclk2
             : (pclk2 * 2U);

    /*
     * Timer counter frequency = 1 MHz
     * TRGO frequency = 1 MHz / 100 = 10 kHz
     */
    prescaler = (timerclk / 1000000U) - 1U;

    htim8.Instance = TIM8;

    htim8.Init.Prescaler = prescaler;
    htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim8.Init.Period = 99U;

    htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;

    htim8.Init.AutoReloadPreload =
        TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;

    sMasterConfig.MasterSlaveMode =
        TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(
            &htim8,
            &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/* ---------------------------------------------------------
 * ADC1 initialization
 *
 * Rank 1: PC5  ADC1_IN15
 * Rank 2: PB0  ADC1_IN8
 * Rank 3: Internal temperature sensor
 * --------------------------------------------------------- */

void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* GPIO PC5 */

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIO PB0 */

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ADC configuration */

    hadc1.Instance = ADC1;

    hadc1.Init.ClockPrescaler =
        ADC_CLOCK_SYNC_PCLK_DIV4;

    hadc1.Init.Resolution = ADC_RESOLUTION_10B;

    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;

    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 0;

    hadc1.Init.ExternalTrigConvEdge =
        ADC_EXTERNALTRIGCONVEDGE_RISING;

    hadc1.Init.ExternalTrigConv =
        ADC_EXTERNALTRIGCONV_T8_TRGO;

    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;

    hadc1.Init.NbrOfConversion = 3;

    hadc1.Init.DMAContinuousRequests = ENABLE;

    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

    /*
     * Link ADC and DMA before ADC initialization.
     */
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /* Rank 1: ADC1_IN15, PC5 */

    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Rank 2: ADC1_IN8, PB0 */

    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 2;
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* Rank 3: internal temperature sensor */

    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = 3;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/* ---------------------------------------------------------
 * Start ADC + DMA + TIM8
 * --------------------------------------------------------- */

HAL_StatusTypeDef ADC1_DMA_Start(void)
{
    HAL_StatusTypeDef status;

    status = HAL_ADC_Start_DMA(
        &hadc1,
        (uint32_t *)(void *)adc_buffer,
        3U
    );

    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_TIM_Base_Start(&htim8);

    if (status != HAL_OK)
    {
        HAL_ADC_Stop_DMA(&hadc1);
    }

    return status;
}

/* ---------------------------------------------------------
 * Stop ADC + DMA + TIM8
 * --------------------------------------------------------- */

HAL_StatusTypeDef ADC1_DMA_Stop(void)
{
    HAL_StatusTypeDef status;

    status = HAL_TIM_Base_Stop(&htim8);

    if (status != HAL_OK)
    {
        return status;
    }

    return HAL_ADC_Stop_DMA(&hadc1);
}
