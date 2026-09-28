#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/*
 * Wiring (Arduino Nano):
 *   Keypad rows 1-4    -> D13, D12, D11, D10 (PB5, PB4, PB3, PB2)
 *   Keypad columns 1-4 -> A3, A2, A1, A0     (PC3, PC2, PC1, PC0)
 *   7-segment display  -> D2, D3, D4, D5, D8, D9, A4, A5
 *                         (PD2-PD5, PB0-PB1, PC4-PC5), common cathode
 *
 * The columns have no external resistors, so the internal pull-ups are used.
 * A column reads HIGH when no key is pressed. To scan, one row at a time is
 * driven LOW; a pressed key in that row pulls its column LOW.
 */

/* Set to 1 if the display's common pin goes to 5V (common anode). */
#define COMMON_ANODE 0

/*
 * Set to 1 to find the segment wiring: the display pins are lit one at a
 * time, one second each, in this order: D2, D3, D4, D5, D8, D9, A4, A5.
 * Note which segment lights at each step and update seg_pin[] below.
 */
#define SEGMENT_TEST 0

#define ROW_MASK 0x3C  /* PB2-PB5 */
#define COL_MASK 0x0F  /* PC0-PC3 */

/* Port C bit for keypad columns 1-4. */
static const uint8_t col_bit[4] = { 3, 2, 1, 0 };

typedef struct
{
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    uint8_t bit;
} seg_pin_t;

/* Pin driving each segment, in order a, b, c, d, e, f, g, DP. */
static const seg_pin_t seg_pin[8] =
{
    { &PORTD, &DDRD, 2 },  /* a  - D2 */
    { &PORTD, &DDRD, 3 },  /* b  - D3 */
    { &PORTD, &DDRD, 4 },  /* c  - D4 */
    { &PORTD, &DDRD, 5 },  /* d  - D5 */
    { &PORTC, &DDRC, 4 },  /* e  - A4 */
    { &PORTC, &DDRC, 5 },  /* f  - A5 */
    { &PORTB, &DDRB, 0 },  /* g  - D8 */
    { &PORTB, &DDRB, 1 },  /* DP - D9 */
};

/* Segment patterns, bit 0 = a ... bit 6 = g, bit 7 = DP. */
#define PAT_0  0x3F
#define PAT_1  0x06
#define PAT_2  0x5B
#define PAT_3  0x4F
#define PAT_4  0x66
#define PAT_5  0x6D
#define PAT_6  0x7D
#define PAT_7  0x07
#define PAT_8  0x7F
#define PAT_9  0x6F
#define PAT_A  0x77
#define PAT_B  0x7C  /* lower-case b */
#define PAT_C  0x39
#define PAT_D  0x5E  /* lower-case d */
#define PAT_DP 0x80

/* Pattern for each key, indexed [row][column]; '*' and '#' show the decimal point. */
static const uint8_t key_pattern[4][4] =
{
    { PAT_1,  PAT_2, PAT_3,  PAT_A },
    { PAT_4,  PAT_5, PAT_6,  PAT_B },
    { PAT_7,  PAT_8, PAT_9,  PAT_C },
    { PAT_DP, PAT_0, PAT_DP, PAT_D },
};

void show(uint8_t pattern)
{
#if COMMON_ANODE
    pattern = ~pattern;
#endif

    for (uint8_t i = 0; i < 8; i++)
    {
        if (pattern & (1 << i))
        {
            *seg_pin[i].port |= (1 << seg_pin[i].bit);
        }
        else
        {
            *seg_pin[i].port &= ~(1 << seg_pin[i].bit);
        }
    }
}

/* Returns the pressed key as (row << 2) | column, or -1 if no key is pressed. */
int8_t scan_keypad(void)
{
    for (uint8_t row = 0; row < 4; row++)
    {
        /*
         * Only the active row is an output (driven LOW); the others are left
         * as inputs so two keys pressed together cannot short two outputs.
         * Row 1 is PB5, row 4 is PB2.
         */
        DDRB = (DDRB & ~ROW_MASK) | (1 << (5 - row));
        PORTB &= ~ROW_MASK;
        _delay_us(10);  /* let the column lines settle */

        uint8_t cols = ~PINC & COL_MASK;  /* pressed column reads LOW */

        for (uint8_t col = 0; col < 4; col++)
        {
            if (cols & (1 << col_bit[col]))
            {
                DDRB &= ~ROW_MASK;
                return (int8_t)((row << 2) | col);
            }
        }
    }

    DDRB &= ~ROW_MASK;
    return -1;
}

int main(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        *seg_pin[i].ddr |= (1 << seg_pin[i].bit);  /* segment outputs */
    }
    show(0x00);

    DDRB &= ~ROW_MASK;   /* rows start as inputs, no pull-ups */
    PORTB &= ~ROW_MASK;

    DDRC &= ~COL_MASK;   /* columns are inputs */
    PORTC |= COL_MASK;   /* with internal pull-ups */

#if SEGMENT_TEST
    while (1)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            show(1 << i);
            _delay_ms(1000);
        }
        show(0x00);
        _delay_ms(2000);  /* pause so the start of the sequence is clear */
    }
#endif

    while (1)
    {
        int8_t key = scan_keypad();

        if (key >= 0)
        {
            /* Keep showing the last key pressed until a new one is pressed. */
            show(key_pattern[key >> 2][key & 0x03]);
            _delay_ms(20);  /* debounce */
        }
    }

    return 0;
}
