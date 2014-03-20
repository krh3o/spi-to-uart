/*
 * uart.h
 *
 * Created: 3/17/2014 7:32:07 PM
 *  Author: karl
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_init(void);
void uart_putchar(uint8_t ch);

#endif /* UART_H_ */