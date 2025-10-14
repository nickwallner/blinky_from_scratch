#include "drivers/include/usart.h"

/* write a single byte to a usart */
static inline void write_byte(USART_Peripheral *usartx, char *byte)
{
    /* write byte to data register */
    usartx->DR = *byte;

    /* wait for bit 6 to be set in status register 
    to indicate transmission is complete */
    while ((usartx->SR & BIT(6)) == 0) {}
}

/* read a single byte from a usart into a pointer */
static inline void read_byte(USART_Peripheral *usartx, __attribute__((unused)) char *buf)
{
    /* check that bit 5 is set to indicate received byte
    is ready to be read */
    if (usartx->SR & BIT(5))
    {
        /* read byte and return it */
        *buf = (char)(usartx->DR & 255);        
    }
}

/* initialize a uart peripheral */
void USART_Init(USART_Peripheral *usartx, uint32_t freq, uint16_t baudrate)
{
    /* reset usart control registers in case it has they have
    any bits already set */
    usartx->CR1 = 0;
    usartx->CR2 = 0;
    usartx->CR3 = 0;

    /* enable usart receiver (bit2), transmitter (bit3), and the usart itself (bit13) */
    usartx->CR1 |= BIT(2) | BIT(3) | BIT(13);

    /* set bit5 to generate an interrupt if usart detects a byte in the receive data register */
    usartx->CR1 |= BIT(5);

    /* set baud rate */
    usartx->BRR = UART_BRR_SAMPLING16(freq, baudrate);
}

/* write a buffer to a usart */
void USART_Transmit(USART_Peripheral *usartx, char *buf, size_t len)
{
    while (len > 0)
    {
        write_byte(usartx, buf); // write single character
        buf++; // go to next character
        len--;
    }
}

void USART_Receive_Byte(USART_Peripheral *usartx, __attribute__((unused)) char *buf)
{
    read_byte(usartx, buf);
}