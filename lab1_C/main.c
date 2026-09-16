/**
 * @file main.c
 * @author ngesb
 * @date 2026-08-12
 * @brief Main function
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << DDB0);          // Configure PB0 (Nano D8) as output.

    while (1)
    {
        PORTB &= ~(1 << PORTB0);  // D8 low: LED off.
        _delay_ms(500);

        PORTB |= (1 << PORTB0);   // D8 high: LED on.
        _delay_ms(500);
    }
}
