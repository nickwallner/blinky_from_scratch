#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "main.h"

void SysTick_Handler(void);
void EXTI15_10_IRQHandler(void);
void USART2_IRQHandler(void);

#endif // INTERRUPTS_H_