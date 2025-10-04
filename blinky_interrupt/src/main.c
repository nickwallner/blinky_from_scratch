#include "include/main.h"

int main(void)
{
    /* set bit 0 and 2 in the AHB1ENR register to enable 
    the system clock for the GPIOA and GPIOC peripherals */
    RCC->AHB1ENR |= (BIT(0) | BIT(2));
    /* set bit 14 to enable clock signal for SYSCFG peripheral */
    RCC->APB2ENR |= BIT(14);

    /* LD2 is connected to PA5, so set PA5 to output mode */
    GPIO_Set_Mode(GPIOA, PIN5, GPIO_MODE_OUTPUT);
    /* button 1 is connected to PC13, set it to input mode */
    GPIO_Set_Mode(GPIOC, PIN13, GPIO_MODE_INPUT);

    /* initialize the systick timer to milliseconds */
    SYSTICK_Init(SYS_FREQ, SYSTICK_MS);

    /* enable exti line 13 to trigger on falling edges */
    EXTI_Line_Enable(EXTI_LINE_13, EXTI_FALLING_EDGE_TRIGGER);
    /* enable gpio port c to trigger exti line 13 */
    SYSCFG_EXTI->EXTICR4 |= (0b0010U << 4UL);
    /* enable interrupt in nvic for exti line 13 */
    NVIC_EnableIRQ(EXTI15_10_IRQHandler);

    while (1)
    {
        // do nothing
    }
}

void EXTI_Callback(void)
{
    GPIO_Toggle(GPIOA, PIN5); // toggle led
}