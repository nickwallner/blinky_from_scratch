#ifndef USART_H_
#define USART_H_

#include "common.h"

/* usart peripheral base addresses */
#define USART1_BASE_ADDR 0x40011000
#define USART2_BASE_ADDR 0x40004400
#define USART3_BASE_ADDR 0x40004800
#define USART4_BASE_ADDR 0x40004C00
#define USART5_BASE_ADDR 0x40005000
#define USART6_BASE_ADDR 0x40011400

/* usart peripheral */
#define USART1 ((USART_Peripheral *) USART1_BASE_ADDR)
#define USART2 ((USART_Peripheral *) USART2_BASE_ADDR)
#define USART3 ((USART_Peripheral *) USART3_BASE_ADDR)
#define USART4 ((USART_Peripheral *) USART4_BASE_ADDR)
#define USART5 ((USART_Peripheral *) USART5_BASE_ADDR)
#define USART6 ((USART_Peripheral *) USART6_BASE_ADDR)

/* usart peripheral registers */
typedef struct
{
    volatile uint32_t SR;   // USART status register
    volatile uint32_t DR;   // USART data register
    volatile uint32_t BRR;  // USART baud rate register
    volatile uint32_t CR1;  // USART control register 1
    volatile uint32_t CR2;  // USART control register 2
    volatile uint32_t CR3;  // USART control register 3
    volatile uint32_t GTPR; // USART guard time and prescaler register
} USART_Peripheral;

/* initialize a usart peripheral */
void USART_Init(USART_Peripheral *, uint32_t, uint16_t);
/* write a buffer to a usart */
void USART_Transmit(USART_Peripheral *, char *, size_t);
/* recieve from a usart */
void USART_Receive_Byte(USART_Peripheral *, char *);

/* these macros are taken verbatim from the STM32F4xx HAL */
/* they are used to calculate the value for the BRR register
given the system clock frequency (_PCLK_) and desired baud rate (_BAUD_) */
/* UART BRR = mantissa + overflow + fraction = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
/* Copyright (c) 2016 STMicroelectronics */
#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_) ((uint32_t)((((uint64_t)(_PCLK_)) * 25U) / (4U * ((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_) (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) / 100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_) ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U) + 50U) / 100U)
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_) ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                             (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U) + \
                                             (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

#endif // USART_H_