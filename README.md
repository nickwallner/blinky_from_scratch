# Blinky From Scratch

Welcome to the Blinky From Scratch project! This project implements multiple
ways to blink an LED using a microcontroller. All code is written completely from
scratch using the `arm-none-eabi` compiler toolchain. This code is compatible
with the STM32F446RE mcu (ARM Cortex-M4), and was tested using a ST Nucleo-F446RE dev board.

Right now, the project has 4 main methods of blinking an led. Each method builds
upon the previous one in the following order:
1. Blinky Basic
2. Blinky Systick
3. Blinky Button
4. Blinky Interrupt

### Blinky Basic

The most simple way of blinky an led, using as few peripherals
as possible. Blinks the led at constant interval. 

**Peripherals Used:** GPIO, RCC

### Blinky Systick

Builds upon the blinky basic code to improve the blinking interval accuracy. This is
achieved by using the Systick timer in the cortex-m4, allowing the led to
be turned on or off at specific intervals.

**Peripherals Used:** GPIO, RCC, SYSTICK

### Blinky Button

Implements additional GPIO functionality to use a button to control the led.
Includes multiple, simple ways of blinking the led with the button by
repeatedly polling a GPIO input pin.

**Peripherals Used:** GPIO, RCC, SYSTICK

### Blinky Interrupt

Completely revamps the blinky button code to use an interrupt to toggle
the button instead of constant polling.

**Peripherals Used:** GPIO, RCC, SYSTICK, EXTI, NVIC, SYSCFG

### Blinky UART

Completely refactored codebase that now acts more as a minimal HAL. Uses the USART2
peripheral alongside the ST-Link debugger/programmer to send/receive data. The program
now sends "led toggled" every time the led is toggled and it will toggle the led if
it recieves a 't'.

**Peripherals Used:** GPIO, RCC, SYSTICK, EXTI, NVIC, SYSCFG, USART