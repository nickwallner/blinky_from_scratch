#include "core/include/interrupts.h"

/* increment system tick counter each time a systick
interrupt is generated */
void SysTick_Handler(void)
{
    SYSTICK_Inc_Ticks();
}

/* exti interrupt handler */
void EXTI15_10_IRQHandler(void)
{
    /* clear pending interrupt bits for exti
    lines 10-15 */
    EXTI->PR = BIT(10) | BIT(11) | BIT(12) | BIT(13) | BIT(14) | BIT(15);

    GPIO_Toggle(GPIOA, PIN5); // toggle led
    USART_Transmit(USART2, "led toggled\r\n", 13);
}

void USART2_IRQHandler(void)
{
    /* read byte from usart2 */
    char str[] = "";
    USART_Receive_Byte(USART2, str);

    /* check if byte received is 't', if so, toggle led and send message */
    if (*str == 't')
    {
        GPIO_Toggle(GPIOA, PIN5);
        USART_Transmit(USART2, "led toggled\r\n", 13);
    }
}