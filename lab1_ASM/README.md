# lab1_ASM

Arduino Nano / ATmega328P LED blink exercise written in AVR assembly. The
program toggles the LED connected to D8 (PB0).

## Structure

| Path                                      | Purpose                                      |
|-------------------------------------------|----------------------------------------------|
| `main.S`                                  | LED blink firmware source                   |
| `.vscode/`                                | MPLAB project and workspace settings         |
| `cmake/`                                  | MPLAB-generated CMake project configuration |
| `_build/`                                 | Generated CMake build tree                   |
| `out/`                                    | Compiled firmware output                     |
