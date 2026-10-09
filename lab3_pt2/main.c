/**
 * @file main.c
 * @author julia and masa
 * @date 2026-10-07
 * @brief Multiplexing a 4x4 keypad to control four LEDs on an AVR microcontroller.
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

 /* Check one key of the 4x4 keypad.
  * Rows: row 0-1 -> PC0-PC1, row 2-3 -> PD2-PD3 (driven low one at a time)
  * Cols: col 0-3 -> PD4-PD7 (inputs with pull-ups; read low when the key is pressed)
  */
 unsigned char read_button(unsigned char row, unsigned char col) {

    unsigned char button_state = 0;

    PORTD = 0xf0; // enable pull-ups on column pins PD4-PD7, row pins stay low

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

 /* Scan the whole keypad and convert the pressed key's (row, col) into a code 0x0-0xF.
  * Returns 0xff if no key is pressed.
  *
  * Keypad mapping (row, col) -> code:
  *           col 0  col 1  col 2  col 3
  *   row 0    0xF    0xE    0xD    0xC
  *   row 1    0xB    0xA    0x9    0x8
  *   row 2    0x7    0x6    0x5    0x4
  *   row 3    0x3    0x2    0x1    0x0
  */
 unsigned char decode_buttons(void){
    unsigned char row, col;
    unsigned char button = 0xff;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            if (read_button(row, col)) {
                button = (row << 4) | col; // pack row in upper nibble, col in lower nibble
            }
        }
    }

    switch (button) {
        case (0<<4) | 0: return 0x0f; // row 0, col 0 -> 0xF
        case (0<<4) | 1: return 0x0e; // row 0, col 1 -> 0xE
        case (0<<4) | 2: return 0x0d; // row 0, col 2 -> 0xD
        case (0<<4) | 3: return 0x0c; // row 0, col 3 -> 0xC
        case (1<<4) | 0: return 0x0b; // row 1, col 0 -> 0xB
        case (1<<4) | 1: return 0x0a; // row 1, col 1 -> 0xA
        case (1<<4) | 2: return 0x09; // row 1, col 2 -> 0x9
        case (1<<4) | 3: return 0x08; // row 1, col 3 -> 0x8
        case (2<<4) | 0: return 0x07; // row 2, col 0 -> 0x7
        case (2<<4) | 1: return 0x06; // row 2, col 1 -> 0x6
        case (2<<4) | 2: return 0x05; // row 2, col 2 -> 0x5
        case (2<<4) | 3: return 0x04; // row 2, col 3 -> 0x4
        case (3<<4) | 0: return 0x03; // row 3, col 0 -> 0x3
        case (3<<4) | 1: return 0x02; // row 3, col 1 -> 0x2
        case (3<<4) | 2: return 0x01; // row 3, col 2 -> 0x1
        case (3<<4) | 3: return 0x00; // row 3, col 3 -> 0x0
        default: return 0xff; // no key pressed
    }
 }

int main(){

    unsigned char button, temp_button;

    while(1) {

        temp_button = decode_buttons();

        if (temp_button != 0xff) {
            button = temp_button; // remember the last key pressed so its pattern keeps repeating
        } else {
            _delay_ms(30); // debounce delay
        }

        /* Key code -> LED pattern */
        switch (button) {

            case 0x00: // key at row 3, col 3
                set_led(0); break;

            case 0x01: // key at row 3, col 2
                set_led(1); break;

            case 0x02: // key at row 3, col 1
                set_led(2); break;

            case 0x03: // key at row 3, col 0
                set_led(3);
                _delay_ms(50);
                set_led(2);
                break;

            case 0x04: // key at row 2, col 3
                set_led(3);
                break;

            case 0x05: // key at row 2, col 2
            set_led(3);
                _delay_ms(50);
                set_led(1);
                 break;

            case 0x06: // key at row 2, col 1
                set_led(1);
                _delay_ms(50);
                set_led(3);
                _delay_ms(50);
                set_led(0);
                _delay_ms(50);
                set_led(2);
                break;
                
            case 0x07: // key at row 2, col 0
                set_led(1);
                _delay_ms(100);
                set_led(3);
                break;

            case 0x08: // key at row 1, col 3
                set_led(1);
                _delay_ms(50);
                set_led(0);
                break;

            case 0x09: // ke
            // y at row 1, col 2
                set_led(2);
                _delay_ms(50);
                set_led(1);
                break;

            case 0x0A: // key at row 1, col 1
                set_led(0);
                _delay_ms(50);
                set_led(2);
                break;

            case 0x0B: // key at row 1, col 0
                set_led(0);
                _delay_ms(50);
                set_led(3);
                break;

            case 0x0C: // key at row 0, col 3
                set_led(1);
                _delay_ms(50);
                set_led(2);
                _delay_ms(50);
                set_led(1);
                break;

            case 0x0D: // key at row 0, col 2
                set_led(0);
                _delay_ms(50);
                set_led(1);
                _delay_ms(50);
                set_led(0);
                break;

            case 0x0E: // key at row 0, col 1
                set_led(0);
                _delay_ms(50);
                set_led(1);
                _delay_ms(50);
                set_led(2);
                break;

            case 0x0F: // key at row 0, col 0
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
