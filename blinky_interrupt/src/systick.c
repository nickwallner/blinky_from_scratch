#include "include/systick.h"

volatile uint32_t System_Ticks = 0;

/* systick handler just increments system ticks */
void SYSTICK_Handler(void)
{
    /* rollover so that the systick current value
    register doesn't overflow */
    if (System_Ticks == (0xffffff - 1))
    {
        System_Ticks = 0;
        return;
    }

    System_Ticks++;
}

/* initialize the systick timer to the specified tickrate */
void SYSTICK_Init(uint32_t freq, SYSTICK_Time_Interval interval)
{
    uint32_t tickrate = freq / interval;

    if ((tickrate - 1) > 0xffffff) return; // the systick timer is 24-bit

    SYSTICK->SYST_RVR = tickrate - 1; // set the reload value
    SYSTICK->SYST_CVR = 0; // clear the current value

    SYSTICK->SYST_CSR = BIT(0) | BIT(1) | BIT(2); // enable the systick timer
}

/* delay for delay_period, length of delay depends on how systick was initialized */
void SYSTICK_Delay(uint32_t delay_period)
{
    uint32_t until = System_Ticks + delay_period;
    while (System_Ticks < until) {}
}