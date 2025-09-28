#include "include/gpio.h"

/* set the mode of a gpio pin */
/* this function can only set the mode of ONE PIN AT A TIME!!! */
void GPIO_Set_Mode(GPIO_Peripheral *gpiox, uint16_t pin, GPIO_Mode mode)
{
    uint16_t pinno = 0;

    /* calculate number of the pin that is being set (pinno)
    which is a decimal value in [0, 15] */
    /* for example, if we are setting the mode for pin 5, then pinno = 5 */
    for (uint16_t i = 0; i < 16; i++)
    {
        if ((pin & (1 << i)) == pin)
        {
            pinno = i;
            break;
        }
    }

    gpiox->MODER &= ~(3U << (pinno * 2)); // clear bits before setting them
    gpiox->MODER |= (mode << (pinno * 2)); // set bits to proper mode
}

/* write a 1 (set) or a 0 (reset) to a gpio pin */
/* this function assumes the pin is already placed in output mode */
void GPIO_Write(GPIO_Peripheral *gpiox, uint16_t pin, GPIO_PinState pinstate)
{
    if (pinstate == GPIO_PIN_SET)
    {
        gpiox->BSRR = pin;
    }
    else
    {
        gpiox->BSRR = (uint32_t)pin << 16;
    }
}