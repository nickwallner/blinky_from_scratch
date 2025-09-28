#include "include/main.h"

int main(void)
{
    /* set bit 0 in the AHB1ENR register to enable 
    the system clock for the GPIOA peripheral */
    RCC->AHB1ENR |= BIT(0);

    /* set bit 14 in the APB2ENR register to enable
    the system clock for the SYSCFG peripheral */
    RCC->APB2ENR |= BIT(14);

    /* LD2 is connected to PA5, so set PA5 to output mode */
    GPIO_Set_Mode(GPIOA, PIN5, GPIO_MODE_OUTPUT);

    /* initialize the systick timer to milliseconds */
    SYSTICK_Init(SYS_FREQ, SYSTICK_MS);

    while (1)
    {
        GPIO_Write(GPIOA, PIN5, GPIO_PIN_SET); // turn led on
        SYSTICK_Delay(80); // delay for 80ms
        GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET); // turn led off
        SYSTICK_Delay(1000); // delay for 1000ms (1 sec)
    }
}