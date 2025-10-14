#ifndef HAL_H_
#define HAL_H_

#define BIT(x) (1UL << (x)) // convenience macro

/* driver includes */
#include "drivers/include/exti.h"
#include "drivers/include/gpio.h"
#include "drivers/include/nvic.h"
#include "drivers/include/rcc.h"
#include "drivers/include/systick.h"
#include "drivers/include/usart.h"

#endif // HAL_H_