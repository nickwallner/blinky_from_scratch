#include "include/main.h"

static inline void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

int main(void)
{
    /* set bit 0 in the AHB1ENR register to enable 
    the system clock for the GPIOA peripheral */
    RCC->AHB1ENR |= BIT(0);

    /* LD2 is connected to PA5, so set PA5 to output mode */
    GPIO_Set_Mode(GPIOA, PIN5, GPIO_MODE_OUTPUT);

    while (1)
    {
        GPIO_Write(GPIOA, PIN5, GPIO_PIN_SET); // turn led on
        spin(999999); // execute 999999 nop instructions
        GPIO_Write(GPIOA, PIN5, GPIO_PIN_RESET); // turn led off
        spin(999999); // execute 999999 more nop instructions
    }
}