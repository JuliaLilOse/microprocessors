/**
 * @file main.c
 * @author julialilley
 * @date 2026-10-21
 * @brief overflow interrupt
 */

 #include <avr/io.h>
 #include <avr/interrupt.h>

 volatile unsigned int tick; // universal counting variable

 #define LED_ON() (PORTD |= (1 << 7)) // turn on LED
 #define LED_OFF() (PORTD &= ~(1 << 7)) // turn off LED

 // overflow rate = 16MHz / 64 / 256 = 976.5625 Hz -> ~4883 ticks per 5 s
 ISR(TIMER0_OVF_vect) {
    tick++;

    if (tick == 1) {
        LED_ON();
    } else if (tick == 4883) { // 5 s on
        LED_OFF();
    } else if (tick >= 9766) { // 5 s off, restart cycle
        tick = 0;
    }
 }
int main(){
    DDRD |= (1 << 7); // set pin 7 of PORTD as output

    // timer config
    TCCR0A = 0; // dont need to change any bits from their default
    TCCR0B = (1<<CS00) | (1<<CS01);// last 3 bits: CS02-CS00 = Clock select bits -> use pre scalar of 64

    // interrupt mask enables
    TIMSK0 = (1 << TOIE0); // enable timer0 overflow interrupt

    // enable global interrupts
    sei(); // flag is set or cleared to enable or disable interrupts

    while(1) {
        
    }

    return 0;
}
