#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "common.h"

/* base address for gpio peripherals */
#define GPIO_PERIPH_BASE_ADDR 0x40020000

/* STM32F446RE has 3 i/o ports: A, B, and C */
/* each i/o port is controlled by a corresponding GPIO peripheral
and each GPIO peripheral is 0x0400 large (aka 1024 KiB or 1 MiB) */
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
    volatile uint32_t AFRL;    // GPIO alternate function register low
    volatile uint32_t AFRH;    // GPIO alternate function register high
} GPIO_Peripheral;

/* generic io pins */
/* each pin has the corresponding bit set, i.e. pin 4 -> bit 4 set */
typedef enum
{
    PIN0  = 0x0001U, // 0000 0000 0000 0001
    PIN1  = 0x0002U, // 0000 0000 0000 0010
    PIN2  = 0x0004U, // 0000 0000 0000 0100
    PIN3  = 0x0008U, // 0000 0000 0000 1000
    PIN4  = 0x0010U, // 0000 0000 0001 0000
    PIN5  = 0x0020U, // 0000 0000 0010 0000
    PIN6  = 0x0040U, // 0000 0000 0100 0000
    PIN7  = 0x0080U, // 0000 0000 1000 0000
    PIN8  = 0x0100U, // 0000 0001 0000 0000
    PIN9  = 0x0200U, // 0000 0010 0000 0000
    PIN10 = 0x0400U, // 0000 0100 0000 0000
    PIN11 = 0x0800U, // 0000 1000 0000 0000
    PIN12 = 0x1000U, // 0001 0000 0000 0000
    PIN13 = 0x2000U, // 0010 0000 0000 0000
    PIN14 = 0x4000U, // 0100 0000 0000 0000
    PIN15 = 0x8000U  // 1000 0000 0000 0000
} GPIO_Pin;

/* gpio pin mode enum, these are the 4 possible modes a
gpio pin can be set too */
typedef enum
{
    GPIO_MODE_INPUT  = 0U,
    GPIO_MODE_OUTPUT = 1U,
    GPIO_MODE_AF     = 2U,
    GPIO_MODE_ANALOG = 3U
} GPIO_Pin_Mode;

/* gpio pin state enum, a pin is either high (set) or low (reset) */
typedef enum
{
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET   = 1U
} GPIO_Pin_State;

/* alternate functions for gpio pins */
/* alternate functions must be looked up in the STM32F446RE
datasheet to determine which pins support which alternate functions */
typedef enum
{
    AF0  = 0x0U,
    AF1  = 0x1U,
    AF2  = 0x2U,
    AF3  = 0x3U,
    AF4  = 0x4U,
    AF5  = 0x5U,
    AF6  = 0x6U,
    AF7  = 0x7U,
    AF8  = 0x8U,
    AF9  = 0x9U,
    AF10 = 0xAU,
    AF11 = 0xBU,
    AF12 = 0xCU,
    AF13 = 0xDU,
    AF14 = 0xEU,
    AF15 = 0xFU,
} GPIO_AF;

/* set the mode of a gpio pin */
void GPIO_Set_Mode(GPIO_Peripheral *, GPIO_Pin, GPIO_Pin_Mode);
/* set alternate function for a gpio pin */
void GPIO_Set_AF(GPIO_Peripheral *, GPIO_Pin, GPIO_AF);
/* write to a gpio pin that is in output mode */
void GPIO_Write(GPIO_Peripheral *, GPIO_Pin, GPIO_Pin_State);
/* read from gpio pin(s) that are in input mode */
GPIO_Pin_State GPIO_Read(GPIO_Peripheral *, GPIO_Pin);
/* toggle a single gpio pin */
void GPIO_Toggle(GPIO_Peripheral *, GPIO_Pin);

#endif // GPIO_DRIVER_H_