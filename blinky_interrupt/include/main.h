#ifndef MAIN_H_
#define MAIN_H_

#include "common.h"
#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "exti.h"
#include "nvic.h"

#define SYS_FREQ 16000000 // system operating frequency in hz

int main(void);

#endif // MAIN_H_