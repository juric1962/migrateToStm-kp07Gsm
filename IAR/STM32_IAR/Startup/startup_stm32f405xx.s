; Minimal startup for STM32F405 (IAR)
; Replace with ST-provided startup for production use

    PUBLIC __iar_program_start
    EXPORT __vector_table

    AREA |.intvec|, NOINIT, READONLY
__initial_sp   EQU 0x20030000
    DCD __initial_sp
    DCD Reset_Handler
    DCD NMI_Handler
    DCD HardFault_Handler
    DCD MemManage_Handler
    DCD BusFault_Handler
    DCD UsageFault_Handler
    DCD 0
    DCD 0
    DCD 0
    DCD 0
    DCD SVC_Handler
    DCD DebugMon_Handler
    DCD 0
    DCD PendSV_Handler
    DCD SysTick_Handler
    DCD Default_Handler ; WWDG
    DCD Default_Handler ; PVD
    DCD Default_Handler ; TAMP_STAMP
    DCD Default_Handler ; RTC_WKUP
    DCD Default_Handler ; FLASH
    DCD Default_Handler ; RCC
    DCD Default_Handler ; EXTI0
    DCD Default_Handler ; EXTI1
    DCD Default_Handler ; EXTI2
    DCD Default_Handler ; EXTI3
    DCD Default_Handler ; EXTI4
    DCD Default_Handler ; DMA1_Stream0
    DCD Default_Handler ; DMA1_Stream1
    DCD Default_Handler ; DMA1_Stream2
    DCD Default_Handler ; DMA1_Stream3
    DCD Default_Handler ; DMA1_Stream4
    DCD Default_Handler ; DMA1_Stream5
    DCD Default_Handler ; DMA1_Stream6
    DCD Default_Handler ; ADC
    DCD Default_Handler ; CAN1_TX
    DCD Default_Handler ; CAN1_RX0
    DCD Default_Handler ; CAN1_RX1
    DCD Default_Handler ; CAN1_SCE
    DCD Default_Handler ; EXTI9_5
    DCD Default_Handler ; TIM1_BRK_TIM9
    DCD Default_Handler ; TIM1_UP_TIM10
    DCD Default_Handler ; TIM1_TRG_COM_TIM11
    DCD Default_Handler ; TIM1_CC
    DCD Default_Handler ; TIM2
    DCD Default_Handler ; TIM3
    DCD Default_Handler ; TIM4
    DCD Default_Handler ; I2C1_EV
    DCD Default_Handler ; I2C1_ER
    DCD Default_Handler ; I2C2_EV
    DCD Default_Handler ; I2C2_ER
    DCD Default_Handler ; SPI1
    DCD Default_Handler ; SPI2
    DCD USART1_IRQHandler
    DCD Default_Handler ; USART2
    DCD USART3_IRQHandler

    AREA |.text|, CODE, READONLY
Reset_Handler
    IMPORT SystemInit
    IMPORT __iar_program_start
    IMPORT USART1_IRQHandler
    IMPORT USART3_IRQHandler
    BL SystemInit
    BL __iar_program_start
    B .

NMI_Handler        B Default_Handler
HardFault_Handler  B Default_Handler
MemManage_Handler  B Default_Handler
BusFault_Handler   B Default_Handler
UsageFault_Handler B Default_Handler
SVC_Handler        B Default_Handler
DebugMon_Handler   B Default_Handler
PendSV_Handler     B Default_Handler
SysTick_Handler    B Default_Handler

Default_Handler
    B Default_Handler
