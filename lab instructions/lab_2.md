# ECED 3204 – Lab 2: Interfacing a Keypad and a 7-Segment Display with an Arduino Nano

## Pre-Lab Information

Read these lab instructions start to finish before beginning the lab. Doing so will save you considerable time during the lab.

## Objective

This lab has two main objectives:

1. Learn about the 7-segment display and the 4×4 matrix keypad.
2. Write a simple program in C that shows the button pressed on the keypad on the 7-segment display.

---

## Introduction to the 7-Segment Display and 4×4 Keypad

### 7-Segment Display

The seven-segment display is the most common device used for displaying digits and letters. You see them in TV shows counting down to "0". Using LEDs in seven-segment displays made them more popular.

A seven-segment display shows binary information as decimal digits. It is used in microwave ovens, calculators, washing machines, radios, digital clocks and more.

Seven-segment displays are made of either LEDs (light-emitting diodes) or LCDs (liquid crystal displays). An LED is a P-N junction diode that emits energy as light, unlike a normal P-N junction diode, which emits it as heat.

The 7 LEDs, called segments, are labelled **A** to **G**. Forward biasing a segment (LED) makes it emit light, lighting up part of the numeral. An extra segment, **DP**, displays the decimal point.

```text
     ─── a ───
    │         │
    f         b
    │         │
     ─── g ───
    │         │
    e         c
    │         │
     ─── d ───    (DP)
```

The pin diagram and datasheet for the 7-segment display used in this lab:
[Inolux INND-TS40 Series datasheet](http://www.inoluxcorp.com/datasheet/Display/Through-Hole-Display/SingleDigit/INND-TS40%20Series_V1.0.pdf). It is important that you become familiar with this document.

Using the GPIO pins of the ATmega328P, we will display the number 0–9, or the dot, when it is pressed on the keypad.

### 4×4 Matrix Keypad

Matrix keypads are the kind of keypads you see on cell phones, calculators, microwave ovens, door locks, etc. They are everywhere.

In DIY electronics, they are a great way to let users interact with your project, and are often needed to navigate menus, enter passwords and control robots.

The keypad buttons are connected to each other by conductive traces underneath the pad, forming a 4×4 grid. You can peel your keypad and check.

The working principle is simple. Pressing a button shorts one of the row lines to one of the column lines, allowing current to flow between them. First, we drive the row lines to '1' one at a time. While a particular row is high, we scan the columns to check which button was pressed.

```text
          Col 1   Col 2   Col 3   Col 4
Row 1 ──── [1] ─── [2] ─── [3] ─── [A]
Row 2 ──── [4] ─── [5] ─── [6] ─── [B]
Row 3 ──── [7] ─── [8] ─── [9] ─── [C]
Row 4 ──── [*] ─── [0] ─── [#] ─── [D]
```

For example: if row 1 is driven high and key **1** is pressed, column 1 reads high, so we know the key is 1. Similarly, if row 2 is driven high and key **6** is pressed, column 3 reads high.

---

## Procedure

1. Connect the microcontroller to the 7-segment display and the keypad as shown in the wiring diagram in the lab handout (Fritzing breadboard layout). You can also refer to the connection document.
   - The keypad's 8-wire ribbon (4 rows + 4 columns) connects to Nano GPIO pins.
   - The 7-segment display's segment pins (A–G, DP) connect to Nano GPIO pins, with its common pin to the appropriate supply rail.
2. After wiring the circuit, create a new project by following the instructions in **Part 3 of Lab 1**.
3. Code as follows from the PDF attached in the Lab 2 notes.
4. Write the C code. It should build and upload to your board without any issues.
5. Check the output.

---

## Lab Questions

1. What is the advantage of using a 4×4 matrix keypad over traditional push buttons?
2. What are the different types of configurations for the 7-segment display?
3. Make the truth table for the numbers 0–9 with respect to the LEDs in the common-cathode configuration.
4. Give five real-world applications where we use a keypad or a 7-segment display (not mentioned above).
