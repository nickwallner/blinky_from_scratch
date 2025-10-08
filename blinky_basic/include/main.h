#ifndef MAIN_H_
#define MAIN_H_

/* cstdlib includes */
#include <stdint.h>

/* driver includes */
#include "include/rcc.h"
#include "include/gpio.h"

#define BIT(x) (1UL << (x)) // convenience macro

int main(void);

#endif // MAIN_H_