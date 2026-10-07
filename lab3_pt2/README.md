# lab3_pt2

Arduino Nano / ATmega328P keypad and LED exercise written in C.

## Structure

| Path       | Purpose                               |
|------------|---------------------------------------|
| `main.c`   | Keypad scanning and LED control source |
| `README.md` | Project-specific documentation        |

The MPLAB/CMake project configuration is not included in the repository. To
build a fresh checkout, create an MPLAB project for the ATmega328P using XC8
and add `main.c` as a source file. Compiler, device-pack, and programmer
settings depend on the local installation and hardware.
