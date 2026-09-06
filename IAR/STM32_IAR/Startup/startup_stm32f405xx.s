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

    AREA |.text|, CODE, READONLY
Reset_Handler
    IMPORT SystemInit
    IMPORT __iar_program_start
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
