#ifndef NVIC_H_
#define NVIC_H_

#include "common.h"

/* base address for the nvic peripheral */
#define NVIC_PERIPH_BASE_ADDR 0xE000E100

/* nvic peripheral */
#define NVIC ((NVIC_Peripheral *) NVIC_PERIPH_BASE_ADDR)

/* nvic peripheral typedef */
/* the cortex-m4 nvic is capable of handling up to 496 different
interrupts. however, ST only implements 96 interrupts on STM32F446RE.
the remaining 400 interrupts in the nvic are unused. each 32-bit
register handles 32 interrupts, so we only need to use the first 3
of each register, the rest remain reserved (unused) */
typedef struct
{
    volatile uint32_t ISER[3];       // NVIC interrupt set enable register
             uint32_t RESERVED0[29];
    volatile uint32_t ICER[3];       // NVIC interrupt clear enable register
             uint32_t RESERVED1[29];
    volatile uint32_t ISPR[3];       // NVIC interrupt set pending register
             uint32_t RESERVED2[29];
    volatile uint32_t ICPR[3];       // NVIC interrupt clear pending register
             uint32_t RESERVED3[29];
    volatile uint32_t IABR[3];       // NVIC interrupt active bit register
             uint32_t RESERVED4[29];
             uint32_t RESERVED5[32];
    volatile uint32_t IPR[3];        // NVIC interrupt priority register
} NVIC_Peripheral;

/* enum for each interrupt that can be used */
typedef enum
{
    /* ------ Cortex-M4 processer exceptions ------ */
    NonMaskableInt_IRQ   = -14,
    HardFault_IRQ        = -13,
    MemoryManagement_IRQ = -12,
    BusFault_IRQ         = -11,
    UsageFault_IRQ       = -10,
    SVCall_IRQ           = -5,
    DebugMonitor_IRQ     = -4,
    PendSV_IRQ           = -2,
    SysTick_IRQ          = -1,

    /* --------- STM32F446RE interrupts --------- */
    /* these are ordered according to the STM32F446x vector table */
    WWDG_IRQHandler               = 0,
    PVD_IRQHandler                = 1,
    TAMP_STAMP_IRQHandler         = 2,
    RTC_WKUP_IRQHandler           = 3,
    FLASH_IRQHandler              = 4,
    RCC_IRQHandler                = 5,
    EXTI0_IRQHandler              = 6,
    EXTI1_IRQHandler              = 7,
    EXTI2_IRQHandler              = 8,
    EXTI3_IRQHandler              = 9,
    EXTI4_IRQHandler              = 10,
    DMA1_Stream0_IRQHandler       = 11,
    DMA1_Stream1_IRQHandler       = 12,
    DMA1_Stream2_IRQHandler       = 13,
    DMA1_Stream3_IRQHandler       = 14,
    DMA1_Stream4_IRQHandler       = 15,
    DMA1_Stream5_IRQHandler       = 16,
    DMA1_Stream6_IRQHandler       = 17,
    ADC_IRQHandler                = 18,
    CAN1_TX_IRQHandler            = 19,
    CAN1_RX0_IRQHandler           = 20,
    CAN1_RX1_IRQHandler           = 21,
    CAN1_SCE_IRQHandler           = 22,
    EXTI9_5_IRQHandler            = 23,
    TIM1_BRK_TIM9_IRQHandler      = 24,
    TIM1_UP_TIM10_IRQHandler      = 25,
    TIM1_TRG_COM_TIM11_IRQHandler = 26,
    TIM1_CC_IRQHandler            = 27,
    TIM2_IRQHandler               = 28,
    TIM3_IRQHandler               = 29,
    TIM4_IRQHandler               = 30,
    I2C1_EV_IRQHandler            = 31,
    I2C1_ER_IRQHandler            = 32,
    I2C2_EV_IRQHandler            = 33,
    I2C2_ER_IRQHandler            = 34,
    SPI1_IRQHandler               = 35,
    SPI2_IRQHandler               = 36,
    USART1_IRQHandler             = 37,
    USART2_IRQHandler             = 38,
    USART3_IRQHandler             = 39,
    EXTI15_10_IRQHandler          = 40,
    RTC_Alarm_IRQHandler          = 41,
    OTG_FS_WKUP_IRQHandler        = 42,
    TIM8_BRK_TIM12_IRQHandler     = 43,
    TIM8_UP_TIM13_IRQHandler      = 44,
    TIM8_TRG_COM_TIM14_IRQHandler = 45,
    TIM8_CC_IRQHandler            = 46,
    DMA1_Stream7_IRQHandler       = 47,
    FMC_IRQHandler                = 48,
    SDIO_IRQHandler               = 49,
    TIM5_IRQHandler               = 50,
    SPI3_IRQHandler               = 51,
    UART4_IRQHandler              = 52,
    UART5_IRQHandler              = 53,
    TIM6_DAC_IRQHandler           = 54,
    TIM7_IRQHandler               = 55,
    DMA2_Stream0_IRQHandler       = 56,
    DMA2_Stream1_IRQHandler       = 57,
    DMA2_Stream2_IRQHandler       = 58,
    DMA2_Stream3_IRQHandler       = 59,
    DMA2_Stream4_IRQHandler       = 60,
    CAN2_TX_IRQHandler            = 63,
    CAN2_RX0_IRQHandler           = 64,
    CAN2_RX1_IRQHandler           = 65,
    CAN2_SCE_IRQHandler           = 66,
    OTG_FS_IRQHandler             = 67,
    DMA2_Stream5_IRQHandler       = 68,
    DMA2_Stream6_IRQHandler       = 69,
    DMA2_Stream7_IRQHandler       = 70,
    USART6_IRQHandler             = 71,
    I2C3_EV_IRQHandler            = 72,
    I2C3_ER_IRQHandler            = 73,
    OTG_HS_EP1_OUT_IRQHandler     = 74,
    OTG_HS_EP1_IN_IRQHandler      = 75,
    OTG_HS_WKUP_IRQHandler        = 76,
    OTG_HS_IRQHandler             = 77,
    DCMI_IRQHandler               = 78,
    FPU_IRQHandler                = 81,
    SPI4_IRQHandler               = 84,
    SAI1_IRQHandler               = 87,
    SAI2_IRQHandler               = 91,
    QUADSPI_IRQHandler            = 92,
    CEC_IRQHandler                = 93,
    SPDIF_RX_IRQHandler           = 94,
    FMPI2C1_EV_IRQHandler         = 95,
    FMPI2C1_ER_IRQHandler         = 96
} IRQ_Type;

/* enable an interrupt in the nvic */
void NVIC_EnableIRQ(IRQ_Type);

#endif // NVIC_H_