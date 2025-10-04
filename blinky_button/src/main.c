#include "include/main.h"

/* choose which blinking method to use, only one
define should be uncommented at a time! */
#define HOLD_CONSTANT
//#define TOGGLE_CONSTANT
//#define HOLD_BLINKING
//#define TOGGLE_BLINKING

int main(void)
{
    __attribute__((unused)) uint8_t toggled_flag = 0; // flag for toggling led blinking

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

        #ifdef HOLD_CONSTANT

        /* ------------------------------------------------ */
        /* hold the button to toggle the led to constant on */
        /* ------------------------------------------------ */

        while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET)
        {
            GPIO_Write(GPIOA, PIN5, GPIO_PIN_SET);
        }

        GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET);

        #endif // HOLD_CONSTANT
        #ifdef TOGGLE_CONSTANT

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

        #endif // TOGGLE_CONSTANT
        #ifdef HOLD_BLINKING

        /* --------------------------------------------- */
        /* hold the button to stop the led from blinking */
        /* --------------------------------------------- */

        /* while button is pressed, turn the led off and wait for the button to be released */
        while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET)
        {
            GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET);
        }

        /* blink the led on 0.5 second intervals when the button isnt pressed */
        GPIO_Toggle(GPIOA, PIN5);
        SYSTICK_Delay(500);

        #endif // HOLD_BLINKING
        #ifdef TOGGLE_BLINKING

        /* ------------------------------------------*/
        /* use the button to toggle the led blinking */
        /* ------------------------------------------*/

        /* this method of toggling the led is not perfect, you might press
        the button but the led doesnt toggle. this is because the button
        is only polled once per loop iteration, so if you press the button
        after the button has been polled for a given iteration, nothing will
        happen. it would be better to use an external interrupt for the 
        button press to guarantee the led is toggled every button press */

        /* poll the button to check if its pressed */
        if (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET)
        {
            toggled_flag = !toggled_flag; // invert toggled flag
            GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET); // turn off led

            /* do nothing while the buttons remains pressed, to ensure
            we don't repeatedly toggle the flag */
            while (GPIO_Read(GPIOC, PIN13) == GPIO_PIN_RESET) {}
        }

        /* if toggled_flag is not set, blink the led */
        if (toggled_flag == 0)
        {
            GPIO_Toggle(GPIOA, PIN5);
            SYSTICK_Delay(500);
        }

        #endif // TOGGLE_BLINKING
    }
}