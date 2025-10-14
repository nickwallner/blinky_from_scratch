#include "drivers/include/gpio.h"

/* calculate number of a gpio pin (pinno)
which is a whole value in [0, 15] */
/* for example, if pin = PIN5, then pinno = 5 */
static inline uint8_t calc_pinno(GPIO_Pin pin)
{
    uint8_t pinno = 0;

    for (uint8_t i = 0; i < 16; i++)
    {
        if ((pin & (1 << i)) == pin)
        {
            pinno = i;
            break;
        }
    }

    return pinno;
}

/* set the mode of a gpio pin */
/* this function can only set the mode of ONE PIN AT A TIME!!! */
void GPIO_Set_Mode(GPIO_Peripheral *gpiox, GPIO_Pin pin, GPIO_Pin_Mode mode)
{
    uint8_t pinno = calc_pinno(pin);

    gpiox->MODER &= ~(3U << (pinno * 2)); // clear bits before setting them
    gpiox->MODER |= (mode << (pinno * 2)); // set bits to proper mode
}

/* sets the alternate function for a given GPIO port and pin */
/* this function assumes the pin is already placed in AF mode */
void GPIO_Set_AF(GPIO_Peripheral *gpiox, GPIO_Pin pin, GPIO_AF af)
{
    uint8_t pinno = calc_pinno(pin);

    if (pinno <= 7)
    {
        gpiox->AFRL &= ~(15U << (pinno * 4)); // clear bits before setting them
        gpiox->AFRL |= (af << (pinno * 4));
    }
    else
    {
        gpiox->AFRH &= ~(15U << (pinno * 4)); // clear bits before setting them
        gpiox->AFRH |= (af << (pinno * 4));
    }
}

/* write a 1 (set) or a 0 (reset) to a gpio pin */
/* this function assumes the pin is already placed in output mode */
void GPIO_Write(GPIO_Peripheral *gpiox, GPIO_Pin pin, GPIO_Pin_State pinstate)
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

/* read gpio pin(s) */
/* this function assumes the pin is already placed in input mode */
/* returns GPIO_PIN_SET or GPIO_PIN_RESET depending on the state of the
pin */
GPIO_Pin_State GPIO_Read(GPIO_Peripheral *gpiox, GPIO_Pin pin)
{
    if ((gpiox->IDR & pin) == pin)
    {
        return GPIO_PIN_SET;
    }
    else
    {
        return GPIO_PIN_RESET;
    }
}

/* toggles a single gpio output pin */
void GPIO_Toggle(GPIO_Peripheral *gpiox, GPIO_Pin pin)
{
    if ((gpiox->ODR & pin) == pin)
    {
        gpiox->BSRR = ((uint32_t)pin << 16);
    }
    else
    {
        gpiox->BSRR = pin;
    }
}