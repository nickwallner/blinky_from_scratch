#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "common.h"

/* base address for the systick timer */
#define SYSTICK_BASE_ADDR 0xE000E010

/* systick timer peripheral */
#define SYSTICK ((SYSTICK_Peripheral *) SYSTICK_BASE_ADDR)

/* systick peripheral struct */
typedef struct
{
    volatile uint32_t SYST_CSR;   // SYSTICK control and status register
    volatile uint32_t SYST_RVR;   // SYSTICK reset value register
    volatile uint32_t SYST_CVR;   // SYSTICK current value register
    volatile uint32_t SYST_CALIB; // SYSTICK calibration register
} SYSTICK_Peripheral;

/* systick timer intervals */
/* this value determines the frequency at which the systick timer
will generate systick exceptions */
typedef enum
{
    SYSTICK_US  = 1000000U, // systick increments System_Ticks each microsecond
    SYSTICK_MS  = 1000U,    // systick increments System_Ticks each millisecond
    SYSTICK_SEC = 1U,       // systick increments System_Ticks each second
} SYSTICK_Time_Interval;

/* handler for systick interrupts */
void SYSTICK_Handler(void);
/* systick timer initializer */
void SYSTICK_Init(uint32_t, SYSTICK_Time_Interval);
/* systick execution delay */
void SYSTICK_Delay(uint32_t);

#endif // SYSTICK_H_