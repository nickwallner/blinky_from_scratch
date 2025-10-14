#include "core/include/main.h"

/* initialize clock signals for peripherals used */
static inline void Clock_Init(void)
{
    /* set bit 0 and 2 in the AHB1ENR register to enable 
    the system clock for the GPIOA and GPIOC peripherals */
    RCC->AHB1ENR |= (BIT(0) | BIT(2));
    /* set bit 14 to enable clock signal for SYSCFG peripheral */
    RCC->APB2ENR |= BIT(14);
    /* set bit17 to enable clock signal for USART2 peripheral */
    RCC->APB1ENR |= BIT(17);
}

/* initialize all gpio pins used */
static inline void GPIO_Pin_Init(void)
{
    /* LD2 is connected to PA5, so set PA5 to output mode */
    GPIO_Set_Mode(GPIOA, PIN5, GPIO_MODE_OUTPUT);

    /* button 1 is connected to PC13, set it to input mode */
    GPIO_Set_Mode(GPIOC, PIN13, GPIO_MODE_INPUT);

    /* PA2 and PA3 are connected to the ST-Link via USART */
    /* the ST-Link generates a virtual com port that can be connected
    to via a terminal emulator like PuTTY (windows) or cu (unix),
    no serial-to-usb adapter is required */

    /* set PA2 to alternate function 7 (usart2 tx) */
    GPIO_Set_Mode(GPIOA, PIN2, GPIO_MODE_AF);
    GPIO_Set_AF(GPIOA, PIN2, AF7);
    /* set PA3 to alternate function 7 (usart2 rx) */
    GPIO_Set_Mode(GPIOA, PIN3, GPIO_MODE_AF);
    GPIO_Set_AF(GPIOA, PIN3, AF7);
}

/* initialize exti interrupts */
static inline void EXTI_Init(void)
{
    /* enable exti line 13 to trigger on falling edges */
    EXTI_Line_Enable(EXTI_LINE_13, EXTI_RISING_EDGE_TRIGGER);
    /* enable gpio pin PC13 to trigger exti line 13 */
    SYSCFG_EXTI->EXTICR4 |= (0b0010U << 4UL);
    /* enable interrupt in nvic for exti line 13 */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void)
{
    Clock_Init();
    GPIO_Pin_Init();
    SYSTICK_Init(SYS_FREQ, SYSTICK_SEC); // set systick to seconds
    EXTI_Init();
    USART_Init(USART2, SYS_FREQ, 9600); // init usart2 to 9600bps baud rate
    NVIC_EnableIRQ(USART2_IRQn);

    while (1)
    {
        // do nothing! interrupts do all the work for us
    }
}