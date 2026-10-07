# ECED 3204 – Lab 3: Debouncing and Multiplexing

## Pre-Lab Information

Read these lab instructions start to finish before beginning the lab. Each part builds on the last, so **do not rip up your breadboard** between Part 2 and Part 3.

## Objective

This lab has three main objectives:

1. Learn about debouncing button presses.
2. Learn about multiplexing to drive multiple LEDs from fewer pins.
3. Learn about multiplexing to read a 4×4 keypad.

Build and upload work the same as Lab 1: open the lab folder in VS Code, `Cmd+Shift+B` to build, then run **Upload MPLAB HEX to Arduino Nano** (see **Lab 1, Parts 3–4**).

### Pin reference

| Nano label | AVR pin | Used for |
|---|---|---|
| D7 | PD7 | Part 1 LED |
| D12 | PB4 | Part 1 push button |
| D8 | PB0 | LED multiplexing (via 330 Ω) |
| D9 | PB1 | LED multiplexing (via 330 Ω) |
| D10 | PB2 | LED multiplexing |
| D11 | PB3 | LED multiplexing |
| A0, A1 | PC0, PC1 | Keypad rows 1–2 |
| D2, D3 | PD2, PD3 | Keypad rows 3–4 |
| D4–D7 | PD4–PD7 | Keypad columns 1–4 |

> PD0/PD1 (RX/TX) are used for USB serial while the Nano is powered from your computer, so the keypad uses PC0/PC1 instead.

---

## Part 1 — Debouncing a Push Button

Project folder: [`lab3/`](../lab3/)

### Wire it up

1. Disconnect USB before touching the breadboard.
2. Seat the tactile button **across the centre channel** so no legs are shorted together. Each pair of legs on the same side is internally connected.
3. One side of the button → **D12 (PB4)**. Other side → **GND**.
4. **D7 (PD7)** → 330 Ω resistor → LED anode (long lead). LED cathode → **GND**.
5. Reconnect USB.

No external pull-up resistor is needed — the code turns on PB4's internal pull-up, so the pin reads **high when released** and **low when pressed**.

### Run it

The lab builds the program up in three steps. Run each one, then move to the next:

**1a. LED follows the button.** In the `while (1)` loop, read `PINB & (1 << 4)` and set or clear PD7. The LED is on when the button is released and off when it is held down.

**1b. Toggle without debouncing.** Replace the loop with the toggle code from the handout (`buttonstate`, `lastbuttonstate`, `ledstate ^= 1`). Each press should flip the LED. Press it many times, and press-and-hold then release.

**1c. Toggle with debouncing.** This is the current [`lab3/main.c`](../lab3/main.c). Build, upload, and press the button repeatedly again.

### What's happening

- **Setup:** `DDRD |= (1 << 7)` makes D7 an output; `DDRB &= ~(1 << 4)` makes PB4 an input; `PORTB |= (1 << 4)` enables PB4's pull-up.
- **1a:** The loop just copies the button pin onto the LED pin, so the LED mirrors the button.
- **1b:** The LED toggles only on the edge where the button goes from released to pressed (`buttonstate != lastbuttonstate && buttonstate == 0`). Every so often you'll catch a glitch — the LED doesn't change, or it toggles on release too. That's **switch bounce**: the metal contacts bounce for a fraction of a millisecond (see the scope capture in the handout), so the pin flickers between high and low several times. The loop runs fast enough to see each flicker as a separate press.
- **1c:** The button is read, the code waits `_delay_ms(20)`, then reads it again. The state is only accepted if both reads agree. 20 ms is far longer than the bounce, so the glitches go away and the LED toggles exactly once per press.

---

## Part 2 — Multiplexing LEDs

Remove the push button **and the Part 1 LED and resistor on D7** (D7 is used by the keypad in Part 3). Keep the Nano where it is.

### Wire it up

1. Disconnect USB.
2. **D8 (PB0)** → 330 Ω (R2) → node **A**.
3. **D9 (PB1)** → 330 Ω (R1) → node **B**.
4. Wire the four LEDs (long lead = anode):

| LED | Anode (long lead) | Cathode (short lead) | To light it: high / low |
|---|---|---|---|
| LED1 | node A (PB0) | D10 (PB2) | PB0 high, PB2 low |
| LED2 | node B (PB1) | D10 (PB2) | PB1 high, PB2 low |
| LED3 | node A (PB0) | D11 (PB3) | PB0 high, PB3 low |
| LED4 | node B (PB1) | D11 (PB3) | PB1 high, PB3 low |

5. Reconnect USB.

### Run it

Use the `set_led()` function from [`lab3_pt2/main.c`](../lab3_pt2/main.c) with this test loop as `main()`:

```c
int main(void)
{
    while (1)
    {
        set_led(0); _delay_ms(2000);   // LED1
        set_led(1); _delay_ms(2000);   // LED2
        set_led(2); _delay_ms(2000);   // LED3
        set_led(3); _delay_ms(2000);   // LED4
    }
}
```

1. Build and upload. The four LEDs should light one at a time, 2 s each.
2. Comment out all four `_delay_ms(2000)` lines, rebuild, and upload again.

### What's happening

- Each LED sits between one "high" pin (PB0/PB1) and one "low" pin (PB2/PB3). `set_led()` makes **only those two pins outputs** (`DDRB = ...`) and drives the high one to 5 V (`PORTB = ...`). The other two pins become inputs (high-impedance), so no current can flow through the other LEDs.
- Because LEDs share pins, you can't light any combination at once — e.g. LED1 + LED4 would also light LED2 and LED3.
- **With delays:** you see each LED on its own, one after the other.
- **Without delays:** the loop cycles through all four LEDs thousands of times a second. Your eyes blend it together, so **all four look on at once** (each a bit dimmer, because each is only on ¼ of the time). This is how multiplexing shows any pattern.

---

## Part 3 — Multiplexing a 4×4 Keypad

Keep the Part 2 LED circuit wired exactly as it is.

Project folder: [`lab3_pt2/`](../lab3_pt2/)

### Wire it up

1. Disconnect USB.
2. Looking at the keypad from the front, the 8 ribbon pins from left to right are **Row 1–4**, then **Col 1–4**. Connect them in order:

| Keypad pin | Function | Nano pin |
|---|---|---|
| 1 | Row 1 (1 2 3 A) | A0 (PC0) |
| 2 | Row 2 (4 5 6 B) | A1 (PC1) |
| 3 | Row 3 (7 8 9 C) | D2 (PD2) |
| 4 | Row 4 (* 0 # D) | D3 (PD3) |
| 5 | Col 1 (1 4 7 *) | D4 (PD4) |
| 6 | Col 2 (2 5 8 0) | D5 (PD5) |
| 7 | Col 3 (3 6 9 #) | D6 (PD6) |
| 8 | Col 4 (A B C D) | D7 (PD7) |

3. Reconnect USB.

### Run it

1. Open `lab3_pt2/`, build, and upload [`lab3_pt2/main.c`](../lab3_pt2/main.c).
2. Press each key and check the LEDs against the table below.

### What's happening

**Reading one key — `read_button(row, col)`:**
- `PORTD = 0xF0` turns on the pull-ups on the column pins PD4–PD7, so every column reads **high** by default.
- The chosen row pin is made an output (`DDRC`/`DDRD |= 1 << row`). Its PORT bit is 0, so the row is driven **low**.
- If the key at that row/column is pressed, it connects the low row to its column, pulling that column pin **low**.
- The column is read, then read again after `_delay_ms(20)` — the same debounce idea as Part 1.
- The row pin is switched back to an input so it can't affect the next row being scanned.

**Scanning all keys — `decode_buttons()`:** loops over all 4 rows × 4 columns, records which key was pressed as `(row << 4) | col`, then converts that into a code from `0x00` to `0x0F`. It returns `0xFF` if nothing is pressed.

**`main()`:** keeps the **last** key pressed in `button`, so a pattern keeps running after you let go. Each case calls `set_led()` one or more times. Patterns that switch between LEDs with `_delay_ms(50)` are multiplexing from Part 2 — with 50 ms steps you will see them chase or flicker. Shorter delays (a few ms) would blend them into a steady pattern.

| Key | Code | LEDs |
|---|---|---|
| 1 | 0x0F | LED4 → LED3 → LED2 |
| 2 | 0x0E | LED1 → LED2 → LED3 |
| 3 | 0x0D | LED1 → LED2 → LED1 |
| A | 0x0C | LED2 → LED3 → LED2 |
| 4 | 0x0B | LED1 ↔ LED4 |
| 5 | 0x0A | LED1 ↔ LED3 |
| 6 | 0x09 | LED3 ↔ LED2 |
| B | 0x08 | LED2 ↔ LED1 |
| 7 | 0x07 | LED2 ↔ LED4 |
| 8 | 0x06 | LED2 → LED4 → LED1 → LED3 |
| 9 | 0x05 | LED4 ↔ LED2 |
| C | 0x04 | LED4 |
| * | 0x03 | LED4 ↔ LED3 |
| 0 | 0x02 | LED3 |
| # | 0x01 | LED2 |
| D | 0x00 | LED1 |

> If the key-to-LED mapping looks mirrored (e.g. **D** behaves like **1**), the ribbon is plugged in reversed. Flip the 8 wires.

---

## Lab Questions

1. What causes problems when trying to read a switch, and how do we fix it?
2. The LED multiplexing in Part 2 could only light certain LED patterns because the LEDs were linked together. How could we light an arbitrary pattern? Include a code snippet as an example. (Hint: your eyes blend very quickly displayed patterns together — think about Part 2, step 2.)
3. Include code from each section in your report. Comment it properly so that a reader can understand everything you have done.
