#include "drivers/include/exti.h"

/* exti line enable with trigger selection */
void EXTI_Line_Enable(EXTI_Line line, EXTI_Trigger trigger)
{
    /* unmask selected exti line */
    EXTI->IMR |= line;

    /* set trigger */
    if (trigger == EXTI_RISING_EDGE_TRIGGER)
    {
        EXTI->RTSR |= line;
    }
    else if (trigger == EXTI_FALLING_EDGE_TRIGGER)
    {
        EXTI->FTSR |= line;
    }
    else if (trigger == EXTI_RISING_FALLING_EDGE_TRIGGER)
    {
        EXTI->RTSR |= line; 
        EXTI->FTSR |= line; 
    }
}