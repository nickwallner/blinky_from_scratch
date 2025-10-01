#include "include/main.h"

int main(void)
{
    /* set bit 0 and 2 in the AHB1ENR register to enable 
    the system clock for the GPIOA and GPIOC peripherals */
    RCC->AHB1ENR |= (BIT(0) | BIT(2));

    /* LD2 is connected to PA5, so set PA5 to output mode */
    GPIO_Set_Mode(GPIOA, PIN5, GPIO_MODE_OUTPUT);
    /* button 1 is connected to PC13, set it to input mode */
    GPIO_Set_Mode(GPIOC, PIN13, GPIO_MODE_INPUT);

    /* initialize the systick timer to milliseconds */
    SYSTICK_Init(SYS_FREQ, SYSTICK_MS);

    while (1)
    {
        /* ST uses a Schmitt trigger on each GPIO pin to implement
        hardware debouncing, so we don't need to worry about
        debouncing the button input with software */

        /* ------------------------------------------------------ */
        /* use the button to toggle the led to constant on or off */
        /* ------------------------------------------------------ */

        /* while button is not pressed, do nothing */
        while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_SET) {}

        /* if the button is ever pressed, the above loop exits, and 
        the led is toggled */
        GPIO_Toggle(GPIOA, PIN5);

        /* loop infinitely while the button remains pressed, 
        so we don't repeatedly toggle the led */
        while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET) {}

        /* --------------------------------------------- */
        /* hold the button to stop the led from blinking */
        /* --------------------------------------------- */

        /* while button is pressed, turn the led off and wait for the button to be released */
        //while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET) { GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET); }

        /* blink the led on 0.5 second intervals when the button isnt pressed */
        //GPIO_Toggle(GPIOA, PIN5);
        //SYSTICK_Delay(500);
    }
}