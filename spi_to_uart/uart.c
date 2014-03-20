/*
 * uart.c
 *
 * Created: 3/17/2014 7:31:51 PM
 *
 */ 

#include <avr/power.h>
#include <avr/io.h>
#include <stdlib.h>
#include "config.h"

#define BAUDRATE 250000 // 0% error
#define UBBR_VALUE (SYSCLK / 16 / BAUDRATE - 1)
#define ACTUAL_BAUD (SYSCLK / 16 / (UBBR_VALUE + 1))
#define BAUD_ERR_PCT (100L * (ACTUAL_BAUD - BAUDRATE) / BAUDRATE)

#if UBBR_VALUE > (1 << 12)
#error Baud rate out of range for sysclock
#endif

#if BAUD_ERR_PCT > 1 || BAUD_ERR_PCT < -1
#warning Excessive baud rate error
#endif

void uart_init(void)
{
    power_usart0_enable();
    UBRR0H = UBBR_VALUE >> 8;
    UBRR0L = UBBR_VALUE & 0xff;

    // Double Speed off and Multi-processor Communication Mode off
    UCSR0A = (0 << U2X0) | (0 << MPCM0);
    // Rx enabled. Tx enabled. 8 data bits.
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
    // Async operation. Parity disabled. 1 stop bit. 8 data bits.
    UCSR0C = (0 << UMSEL00) | (0 << UPM00) | (0 << USBS0) | (3 << UCSZ00);
}

void uart_putchar(uint8_t ch)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = ch;
}

