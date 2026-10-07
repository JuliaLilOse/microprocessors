/**
 * @file main.c
 * @author julialilley
 * @date 2026-10-07
 * @brief Main function
 */

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include <util/delay.h>

 /* Turn on one of the four LED based on lednum by setting appropriate bits in DDRB and PORTB: output means set the bit to 1 */
 void set_led(unsigned char lednum) {

    switch (lednum) {
        case 0: // turn on first led
        DDRB = 1 << 0 | 1 << 2; // set bit 0 and bit 2 as output
        PORTB = 1 << 0; // set bit 0 of PORTB to high
        break;

        case 1: // turn on second led
        DDRB = 1 << 1 | 1 << 2; // set bit 1 and bit 2 as output
        PORTB = 1 << 1; // set bit 1 of PORTB to high
        break;

        case 2: // turn on third led
        DDRB = 1 << 0 | 1 << 3; // set bit 0 and bit 3 as output
        PORTB = 1 << 0; // set bit 0 of PORTB to high
        break;

        case 3: // turn on fourth led
        DDRB = 1 << 1 | 1 << 3; // set bit 1 and bit 3 as output
        PORTB = 1 << 1; // set bit 1 of PORTB to high
        break;
    }
 }

 unsigned char read_button(unsigned char row, unsigned char col) {

    unsigned char button_state = 0;

    PORTD = 0xf0;

    if (row < 2) {
        DDRC |= 1 << row; // set the row pin as output
    }
    else {
        DDRD |= 1<<row;
    }

    _delay_us(10); // wait for the pin to settle

    if ((PIND & (1<<(col+4))) == 0) {
        _delay_ms(20); // debounce delay
        if ((PIND & (1<<(col+4))) == 0) {
            button_state = 1; // button is pressed
        }
    }

    if (row < 2) {
        DDRC &= ~(1 << row); // set the row pin back to input
    }
    else {
        DDRD &= ~(1<<row);
    }

    return button_state; //returns 1 if button is pressed, 0 if not
 }

 unsigned char decode_buttons(void){
    unsigned char row, col;
    unsigned char button = 0xff;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            if (read_button(row, col)) {
                button = (row << 4) | col;
            }
        }
    }

    switch (button) {
        case (0<<4) | 0: return 0x0f;
        case (0<<4) | 1: return 0x0e;
        case (0<<4) | 2: return 0x0d;
        case (0<<4) | 3: return 0x0c;
        case (1<<4) | 0: return 0x0b;
        case (1<<4) | 1: return 0x0a;
        case (1<<4) | 2: return 0x09;
        case (1<<4) | 3: return 0x08;
        case (2<<4) | 0: return 0x07;
        case (2<<4) | 1: return 0x06;
        case (2<<4) | 2: return 0x05;
        case (2<<4) | 3: return 0x04;
        case (3<<4) | 0: return 0x03;
        case (3<<4) | 1: return 0x02;
        case (3<<4) | 2: return 0x01;
        case (3<<4) | 3: return 0x00;
        default: return 0xff;
    }
 }


int main(){

    unsigned char button, temp_button;

    while(1) {

        temp_button = decode_buttons();

        if (temp_button != 0xff) {
            button = temp_button;
        } else {
            _delay_ms(30); // debounce delay
        }

        switch (button) {
            case 0x00:
                set_led(0); break;
            case 0x01:
                set_led(1); break;
            case 0x02:
                set_led(2); break;
            case 0x03:
                set_led(3);
                _delay_ms(50);
                set_led(2);
                break;
            case 0x04:
                set_led(3);
                break;
            case 0x05:
            set_led(3);
                _delay_ms(50);
                set_led(1);
                 break;
            case 0x06:
                set_led(1);
                _delay_ms(50);
                set_led(3);
                _delay_ms(50);
                set_led(0);
                _delay_ms(50);
                set_led(2);
                break;
            case 0x07:
                set_led(1);
                _delay_ms(100);
                set_led(3);


                break;
            case 0x08:
                set_led(1);
                _delay_ms(50);
                set_led(0);
                break;
            case 0x09:
                set_led(2);
                _delay_ms(50);
                set_led(1);
                break;
            case 0x0A:
                set_led(0);
                _delay_ms(50);
                set_led(2);
                break;
            case 0x0B:
                set_led(0);
                _delay_ms(50);
                set_led(3);
                break;
            case 0x0C:
                set_led(1);
                _delay_ms(50);
                set_led(2);
                _delay_ms(50);
                set_led(1);
                break;

            case 0x0D:
                set_led(0);
                _delay_ms(50);
                set_led(1);
                _delay_ms(50);
                set_led(0);
                break;

            case 0x0E:
                set_led(0);
                _delay_ms(50);
                set_led(1);
                _delay_ms(50);
                set_led(2);
                break;

            case 0x0F:
                set_led(3);
                _delay_ms(50);
                set_led(2);
                _delay_ms(50);
                set_led(1);
                break;
        }
    }

    return 0;
}
