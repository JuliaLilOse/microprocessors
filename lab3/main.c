/**
 * @file main.c
 * @author julialilley
 * @date 2026-10-07
 * @brief Main function
 */
 #include <avr/io.h>
 #include <util/delay.h>

int main(void){

    // set port D7 to output: set bit 7 of DDRD to 1
    DDRD |= (1 << 7);
    // set port B4 to input: set bit 4 of DDRB to 0
    DDRB &= ~(1 << 4);
    // set port D7 to high: set bit 7 of PORTD to 1 to turn on the LED
    PORTD |= (1 << 7);

    //turn on pull-up resistor for port B4: set bit 4 of PORTB to 1
    PORTB |= (1 << 4);

    unsigned char ledstate = 1; // LED starts ON, or 1 
    unsigned char buttonstate = 0; // button starts not pressed, or 0
    unsigned char lastbuttonstate = 0; // last button state starts not pressed, or 0

    while(1) {
        unsigned char tempbuttonstate = PINB & (1 << 4); // read the button state from port B4
        _delay_ms(20);
        if ((PINB & (1 << 4)) == tempbuttonstate ) { // check if button is pressed
            buttonstate = tempbuttonstate; // button is pressed
        } else {
            buttonstate = 1; // button is not pressed
        }
        
        if ((buttonstate != lastbuttonstate) && (buttonstate == 0)) { // check if button state has changed and is pressed
            ledstate ^=1; 
        } 

        lastbuttonstate = buttonstate; // update last button state

        if (ledstate) {
            PORTD |= (1 << 7); // turn on the LED
        } else {
            PORTD &= ~(1 << 7); // turn off the LED
        }
    
    }

    return 0;
}
