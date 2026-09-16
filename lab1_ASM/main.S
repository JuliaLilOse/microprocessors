#include <avr/io.h>

.global main
.type main, @function

main:
    sbi _SFR_IO_ADDR(DDRB), DDB0

blink_loop:
    cbi _SFR_IO_ADDR(PORTB), PORTB0
    rcall delay_loop

    sbi _SFR_IO_ADDR(PORTB), PORTB0
    rcall delay_loop
    rjmp blink_loop

delay_loop:
    // 500000 decimal = 0x07A120 (loop count, not milliseconds)
    ldi r18, 0x20
    ldi r19, 0xA1
    ldi r20, 0x07
    
delay_count:
    subi r18, 1
    sbci r19, 0
    sbci r20, 0
    brne delay_count
    ret
    
.size main, .-main