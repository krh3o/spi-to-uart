/*
 * spi_to_uart.c
 *
 * Created: 3/16/2014 3:48:49 PM
 *
 */ 

#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/power.h>
#include <avr/io.h>
#include "uart.h"

/*
 * Interrupt
 *
*/
// SPI Interrupt for Slave Mode
ISR(SPI_STC_vect)
{

}

// SPI Interrupt Initialization for Slave Mode
void init_spi_slave_int (void)
{
    power_spi_enable();
    
    // Set PB4(MISO) as output
    DDRB    = (1<<DDB4);
    // Enable SPI Interrupt and SPI in Slave Mode
    SPCR  = (1<<SPIE)|(1<<SPE);
}

// SPI Interrupt Receive Routine for Slave Mode
void spi_slave_receive_int (void)
{
    // Update to go to sleep
    while (1)
    {
    }
}

/*
 * Polling
 *
*/
// Initialize SPI Slave Mode (polling)
void init_spi_slave_polling (void)
{
    // Set PB4(MISO) as output. PB3(MOSI), PB2(SS), and PB5(SCK) are input 
    DDRB = (1<<DDB4); 
    // Enable SPI in Slave Mode
    SPCR = (1<<SPE);
   
    power_spi_enable();
}

// SPI Receive Slave Mode (polling)
void spi_slave_receive_polling (void (*fp)(uint8_t))
{
    while (1)
    {
        // Wait until Char is received
        while (!(SPSR & (1<<SPIF)));
            
        // check for expected value
        if (SPDR == 'd')
        {
            break;
        }
            
        // reset spi interface by pulling SS high
        PORTB |= (1<<PORTB2);
        PORTB &= ~(1<<PORTB2);
    }
    
    while (1)
    {
        // Wait until Char is received
        while (!(SPSR & (1<<SPIF)));
            
        // Callback
        fp(SPDR);
    }
}

int main (void)
{
    uart_init();
    
    // init_spi_slave_int();
    // spi_slave_receive_int();
    
    init_spi_slave_polling();
    spi_slave_receive_polling(uart_putchar);
}
