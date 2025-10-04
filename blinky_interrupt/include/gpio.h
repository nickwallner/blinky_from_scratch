#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "common.h"

/* base address for gpio peripherals */
#define GPIO_PERIPH_BASE_ADDR 0x40020000

/* STM32F446RE has 3 i/o ports: A, B, and C */
/* each i/o port is controlled by a corresponding GPIO peripheral
and each GPIO peripheral is 0x0400 large, which is 1024 bytes (1KiB) */
#define GPIOA_BASE_ADDR (GPIO_PERIPH_BASE_ADDR + 0x0000UL)
#define GPIOB_BASE_ADDR (GPIO_PERIPH_BASE_ADDR + 0x0400UL)
#define GPIOC_BASE_ADDR (GPIO_PERIPH_BASE_ADDR + 0x0800UL)

/* gpio peripherals */
/* each peripheral (GPIOA, GPIOB, GPIOC) is simply a pointer to a GPIO_Peripheral instance,
where each instance is placed at the base address for its respective peripheral */
#define GPIOA ((GPIO_Peripheral *) GPIOA_BASE_ADDR)
#define GPIOB ((GPIO_Peripheral *) GPIOB_BASE_ADDR)
#define GPIOC ((GPIO_Peripheral *) GPIOC_BASE_ADDR)

/* gpio peripheral struct, holds all the gpio peripheral registers */
typedef struct
{
    volatile uint32_t MODER;   // GPIO port mode register
    volatile uint32_t OTYPER;  // GPIO port output type register
    volatile uint32_t OSPEEDR; // GPIO port output speed register
    volatile uint32_t PUPDR;   // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;     // GPIO port input data register
    volatile uint32_t ODR;     // GPIO port output data register
    volatile uint32_t BSRR;    // GPIO port bit set/reset register
    volatile uint32_t LCKR;    // GPIO port configuration lock register
    volatile uint32_t AFR[2];  // GPIO alternate function registers
} GPIO_Peripheral;

/* gpio pin mode enum, these are the 4 possible modes a
gpio pin can be set too */
typedef enum
{
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
} GPIO_Mode;

/* gpio pin state enum, a pin is either high (set) or low (reset) */
typedef enum
{
    GPIO_PIN_RESET,
    GPIO_PIN_SET
} GPIO_PinState;

/* set the mode of a gpio pin */
void GPIO_Set_Mode(GPIO_Peripheral *, uint16_t, GPIO_Mode);
/* write to a gpio pin that is in output mode */
void GPIO_Write(GPIO_Peripheral *, uint16_t, GPIO_PinState);
/* read from gpio pin(s) that are in input mode */
GPIO_PinState GPIO_Read(GPIO_Peripheral *, uint16_t);
/* toggle a single gpio pin */
void GPIO_Toggle(GPIO_Peripheral *, uint16_t);

#endif // GPIO_DRIVER_H_