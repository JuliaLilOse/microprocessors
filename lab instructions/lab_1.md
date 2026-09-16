# ECED 3204 – Lab 1: Arduino Nano / ATmega328P Dev Environment (macOS)

Programming an Arduino Nano with VS Code + MPLAB in C and AVR assembly. Adapted from the Windows-only handout — the Windows requirement was only about driver support, which isn't an issue on modern macOS (CH340 support is built into the kernel).

## Before You Start

- 4 GB free disk space
- **USB data cable** (not charge-only — won't create a serial device)
- macOS (tested on Apple Silicon, e.g. M2)
- Use real **VS Code**, not Cursor — MPLAB Extension Pack is VS Code Marketplace-only, and Cursor defaults to Open VSX, so the extension pack won't install cleanly. Keep Cursor for other work, just use VS Code for this course.
- Don't mix old Atmel Studio / Arduino IDE instructions with this workflow

**Key pin note:** ATmega328P pin names ≠ Nano board pin names (e.g. PD2 on the chip = D2 on the board). This lab uses **D8 / PB0**.

**Electrical warning:** Never wire an LED directly between an output pin and ground — always use the 330 Ω series resistor. Long lead = anode, short lead/flat side = cathode.

Reference docs:
- [ATmega328P datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
- [AVR instruction set manual](http://ww1.microchip.com/downloads/en/devicedoc/atmel-0856-avr-instruction-set-manual.pdf)

---

## Part 2 — Install the Toolchain

| Component | Purpose |
|---|---|
| VS Code + MPLAB Extension Pack | Opens the project template, edits code, launches builds |
| MPLAB XC8 compiler | Compiles C and GNU-style AVR assembly for the ATmega328P |
| Arduino CLI + Arduino AVR Boards core | Uploads the .hex file over the Nano's USB bootloader |
| USB serial driver | Not needed on macOS — CH340/CH341 support is built into the kernel |

### A. VS Code + MPLAB Extension Pack
1. Download the macOS build from [code.visualstudio.com/download](https://code.visualstudio.com/download), unzip, and drag `Visual Studio Code.app` into `/Applications`.
2. Launch VS Code → `Cmd+Shift+X` → search **MPLAB Extension Pack** → confirm publisher is **Microchip Technology Inc.** → Install.
3. Restart VS Code → `Cmd+Shift+P` → type `MPLAB:` → confirm commands appear.

To check whether VS Code (vs. a fork like Cursor) is actually on your PATH:
```bash
ls /Applications | grep -i "visual studio code"
which code
```
Note: `code --version` can be unreliable if another editor (e.g. Cursor) has also registered a `code` CLI shim — if the version printed doesn't look like a normal VS Code version (`1.9x.x`), confirm via **VS Code → About Visual Studio Code** in the app menu instead.

> **Stop condition:** `MPLAB: Welcome` must appear and open successfully in the Command Palette before continuing.

### B. MPLAB XC8 (installed separately)
1. Download the macOS installer from [microchip.com/mplab/compilers](https://www.microchip.com/mplab/compilers) and run it. You can install to the default location or a custom folder (e.g. inside your project directory) — either works, it just affects who on the machine can use it.
2. Restart VS Code → `Cmd+Shift+P` → `MPLAB: List Toolchains`.
3. If it reports **"No toolchains detected"**, register it manually:
   - Find the folder containing the `xc8-cc` binary:
     ```bash
     find /path/to/your/install/location -name "xc8-cc" -o -name "xc8"
     ```
   - `Cmd+Shift+P` → `MPLAB: Add Toolchain` → point it at the `bin` folder containing `xc8-cc`.
   - Re-run `MPLAB: List Toolchains` and confirm it now shows up (e.g. `xc8@4.00`).

> **Stop condition:** Don't create a project until `MPLAB: List Toolchains` shows XC8.
>
> **Version note:** the extension may *register* one XC8 version while CMake's build log shows it actually *invoking* a different one (e.g. a v3.10 install found elsewhere on your system takes precedence). Check the build terminal output for the actual `xc8-cc` path being called, and record **that** version in your lab notes — not just whatever `MPLAB: List Toolchains` reports.

### C. Arduino CLI + AVR core

Easiest via Homebrew:
```bash
brew install arduino-cli
```

Then:
```bash
arduino-cli version
arduino-cli core update-index
arduino-cli core install arduino:avr
```

Confirm the install path (used later in the upload task):
```bash
which arduino-cli
```
Typically `/opt/homebrew/bin/arduino-cli` on Apple Silicon.

### D. Confirm the Nano's serial device
- Plug in the Nano (data cable).
- Run:
  ```bash
  arduino-cli board list
  ```
- If unsure which device is the Nano, unplug it, run the command again, and see which `/dev/cu.usbserial-*` entry disappears — then replug and confirm it reappears.
- The device will look something like `/dev/cu.usbserial-A5069RR4`.

> **Stop condition:** No serial device = don't continue. On macOS this is almost always a charge-only cable or a bad USB port/hub — driver installation generally isn't needed.

---

## Part 3 — Open & Build the C Template

1. Download `Nano_XC8_C_Template.zip` from the course site, extract to a short local path (e.g. `~/microprocessors/lab1_C`) — don't work inside the zip.
2. Rename **only the outer folder** (e.g. `lab1_C`). Do **not** rename the internal `.vscode/Nano_XC8_C_Template.mplab.json`.
3. VS Code → **File → Open Folder** → open the renamed folder.
4. `MPLAB: Edit Project Properties (UI)` and verify:
   - Device: **ATmega328P**
   - Toolchain: **XC8**
   - Optimization: **level 1 (-O1)**
   - Tool: **Simulator**
5. Open `main.c`, paste in the Part 5 blink program, save.
6. `Cmd+Shift+B` → run the MPLAB build task.

> **Stop condition:** Wait for `CMake Build successful. (exit code 0)` in the terminal. Editor red squiggles are not proof of build success or failure — read the actual compiler output.

The .hex output lands automatically in the workspace's `out` folder — the Upload task (Part 4) finds it on its own.

---

## Part 4 — Global Nano Upload Task

This task is stored once in VS Code **User Tasks** (not per-project) because the CLI path/serial device are machine-specific. Configure it once in Lab 1 and it works for every future lab.

### Bootloader profile

| Nano type | FQBN |
|---|---|
| Older official Nano / common clone (**default**) | `arduino:avr:nano:cpu=atmega328old` |
| Newer official classic Nano (Optiboot) | `arduino:avr:nano:cpu=atmega328` |

Start with `atmega328old` — it's the profile that worked on first try in testing (`UPLOAD SUCCESSFUL`, `208 bytes of flash verified`). If upload fails with "programmer is not responding / not in sync," check the cable/device path, then try `atmega328` once.

### Setup steps
1. Close any program using the serial device (Serial Monitor, `screen`, etc.).
2. `Cmd+Shift+P` → `Tasks: Open User Tasks` → **Other**.
3. Paste in the task config (see below — this is the **bash** version, not PowerShell). Edit the `env` block:
   - `ARDUINO_CLI_PATH`: output of `which arduino-cli` (e.g. `/opt/homebrew/bin/arduino-cli`)
   - `NANO_COM_PORT`: your actual device path (e.g. `/dev/cu.usbserial-A5069RR4`)
   - `NANO_FQBN`: leave as `atmega328old` unless told otherwise
4. Save (`Cmd+S`).
5. Confirm `MPLAB CMake: Build` succeeded (exit code 0).
6. `Cmd+Shift+P` → `Tasks: Run Task` → **Upload MPLAB HEX to Arduino Nano**.
7. Confirm output shows `BUILD OUTPUT VERIFIED`, a HEX path inside your current workspace, and ends with `UPLOAD SUCCESSFUL`.

<details>
<summary>User Tasks JSON — macOS/bash version (click to expand)</summary>

```jsonc
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Upload MPLAB HEX to Arduino Nano",
      "type": "shell",
      "command": "workspace=\"${workspaceFolder}\"; configDir=\"$workspace/.vscode\"; mplabConfig=$(find \"$configDir\" -maxdepth 1 -name '*.mplab.json' | head -n 1); if [ -z \"$mplabConfig\" ]; then echo 'UPLOAD CANCELLED: The opened folder is not an MPLAB project root. Open the folder containing .vscode, cmake, and main.c or main.S.'; exit 1; fi; outDir=\"$workspace/out\"; if [ ! -d \"$outDir\" ]; then echo 'UPLOAD CANCELLED: No out folder exists. Build the MPLAB project first.'; exit 1; fi; hex=$(find \"$outDir\" -name '*.hex' -type f -exec stat -f '%m %N' {} \\; | sort -rn | head -n 1 | cut -d' ' -f2-); if [ -z \"$hex\" ]; then echo 'UPLOAD CANCELLED: No HEX file was found. Build the MPLAB project first.'; exit 1; fi; newerSources=$(find \"$workspace\" \\( -name '*.c' -o -name '*.h' -o -name '*.cpp' -o -name '*.S' -o -name '*.s' \\) -newer \"$hex\" -not -path '*/out/*' -not -path '*/_build/*' -not -path '*/build/*' -not -path '*/cmake/*'); if [ -n \"$newerSources\" ]; then echo 'UPLOAD CANCELLED: Source code is newer than the generated HEX file.'; echo 'Run Build successfully before uploading.'; exit 2; fi; echo ''; echo 'BUILD OUTPUT VERIFIED'; echo \"Project: $workspace\"; echo \"HEX file: $hex\"; echo \"Uploading to $NANO_COM_PORT...\"; \"$ARDUINO_CLI_PATH\" upload --port \"$NANO_COM_PORT\" --fqbn \"$NANO_FQBN\" --input-file \"$hex\" --verify --verbose; exit_status=$?; if [ $exit_status -ne 0 ]; then echo ''; echo 'UPLOAD FAILED'; echo 'The build succeeded, but the HEX file was not transferred.'; exit $exit_status; fi; echo ''; echo 'UPLOAD SUCCESSFUL'",
      "options": {
        "env": {
          "ARDUINO_CLI_PATH": "/opt/homebrew/bin/arduino-cli",
          "NANO_COM_PORT": "/dev/cu.usbserial-A5069RR4",
          "NANO_FQBN": "arduino:avr:nano:cpu=atmega328old"
        }
      },
      "problemMatcher": [],
      "presentation": {
        "reveal": "always",
        "focus": true,
        "panel": "dedicated",
        "clear": true
      }
    }
  ]
}
```
</details>

> **zsh gotcha:** `status` is a reserved variable in zsh — the exit-code check uses `exit_status=$?` instead, or the task errors out after a successful upload.
>
> Build and upload are separate steps: a successful build only produces the HEX file. If upload reports source code newer than HEX, rebuild first.

### ✅ Mandatory setup checkpoint — record in your lab notes:
- VS Code version (check via **VS Code → About Visual Studio Code** in the app menu, not just `code --version` in the terminal)
- MPLAB Extension Pack version (`Cmd+Shift+X` → find it in installed extensions → version shows in the details panel)
- XC8 version (the one actually invoked in the build log — see the version note in Part 2B)
- Nano serial device path and CLI directory (`which arduino-cli`)
- Working Nano profile (`atmega328old` / `atmega328`)
- Global Upload task result and current-workspace HEX path reported

---

## Part 5 — C: Blink an LED

`main.c`:
```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << DDB0);           // Configure PB0 (Nano D8) as output.

    while (1)
    {
        PORTB &= ~(1 << PORTB0);   // D8 low: LED off.
        _delay_ms(500);

        PORTB |= (1 << PORTB0);    // D8 high: LED on.
        _delay_ms(500);
    }
}
```

| Line | Purpose |
|---|---|
| `#define F_CPU 16000000UL` | Tells `<util/delay.h>` the Nano runs at 16 MHz |
| `DDRB \|= (1 << DDB0);` | Sets only PB0's direction bit — makes D8 an output |
| `PORTB &= ~(1 << PORTB0);` | Clears only PB0 — drives D8 low (0V) |
| `PORTB \|= (1 << PORTB0);` | Sets only PB0 — drives D8 high (5V) |
| `_delay_ms(500);` | ~500 ms blocking delay, based on `F_CPU` |

"Drives high/low" just means the actual voltage on the physical pin: low = 0V (no current through the LED, off), high = 5V (current flows, LED on).

**Build → Upload:**
1. Save `main.c`, run Build, fix all errors/warnings.
2. After exit code 0, run **Upload MPLAB HEX to Arduino Nano**.
3. Confirm the reported HEX path is inside this workspace and it ends in `UPLOAD SUCCESSFUL`.

**Wire it up:**
1. Disconnect USB before touching the breadboard.
2. Nano D8 (PB0) → LED anode (long lead).
3. LED cathode → 330 Ω resistor → Nano GND. (Resistor/LED order in series doesn't matter.)
4. Reconnect USB — LED should blink ~500 ms on / 500 ms off.

---

## Part 6 — AVR Assembly: Blink an LED

1. Extract `Nano_XC8_ASM_Template.zip`, rename outer folder to `lab1_ASM` (don't touch the internal MPLAB JSON).
2. Open in a **separate** VS Code window. Contains `main.S`, no `main.c`.
3. Keep the **uppercase `.S`** extension (needed for the C preprocessor to handle `<avr/io.h>`).
4. Replace `main.S` contents with:

```asm
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
```

> **Note:** `500000` here is a *loop count*, not milliseconds — at 16 MHz this delay is noticeably shorter than the C version's `_delay_ms(500)`. That timing mismatch is intentional and part of the analysis.

**Build → Upload → Compare:**
1. Save, build, confirm exit code 0.
2. Run the same global Upload task — confirm the HEX path points into the `lab1_ASM` workspace.
3. Observe blink rate vs. the C version (faster/slower?).
4. Compare HEX file sizes between the two builds (same build config for a fair comparison).

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| XC8 not listed / "No toolchains detected" | Compiler not auto-registered | `find /path/to/install -name "xc8-cc" -o -name "xc8"`, then `MPLAB: Add Toolchain` pointing at the `bin` folder |
| MPLAB Extension Pack won't install | Using Cursor instead of VS Code | Install real VS Code — Cursor defaults to Open VSX and can't pull Microsoft Marketplace extensions cleanly |
| No serial device shows up | Charge-only cable, bad port/hub | Try a real data cable, different port — drivers usually aren't the issue on macOS |
| `avrdude: programmer is not responding / not in sync` | Wrong device path, wrong bootloader profile, busy port, bad cable | Close serial tools, recheck `/dev/cu.usbserial-*`, try the other `atmega328`/`atmega328old` profile once |
| Build succeeds, upload fails | Build and upload are separate ops | Debug CLI path/device/cable/bootloader — not your C code |
| Upload task errors right after printing UPLOAD SUCCESSFUL-looking output | zsh treats `status` as reserved | Make sure the task uses `exit_status=$?`, not `status=$?` |
| Upload succeeds, no behavior change | Wrong/stale workspace or HEX | Open the correct lab folder, rebuild, confirm HEX path matches that workspace |
| `F_CPU` warning or wrong delay | `F_CPU` missing/misplaced | Define `F_CPU 16000000UL` *before* `#include <util/delay.h>` |
| LED never lights | Reversed LED, wrong pin, no ground | Power off; check D8/PB0, LED polarity, 330 Ω resistor, GND |
| MPLAB project won't initialize | Wrong folder / incomplete extraction | Open the extracted root containing `.vscode`, `cmake`, `main.c`/`main.S`, `README.md`; reload VS Code once if needed |
| `code --version` prints something odd (e.g. `3.19.13`) | Another editor (Cursor) has claimed the `code` CLI shim | Check the real version via **VS Code → About Visual Studio Code** in the app menu |

---

## Lab Questions

1. What is the Arduino Nano CPU clock frequency in MHz?
2. What happens to `_delay_ms()` timing if `F_CPU` is defined lower or higher than the actual clock frequency?
3. Which generated HEX file contains more program data, C or assembly? Record both sizes and explain one reason they differ.
4. Was the assembly blink faster or slower than the C blink? Explain why `500000` in the assembly source isn't equivalent to `_delay_ms(500)`.
5. Which ATmega328P port bit corresponds to Nano D8, and which register controls whether that pin is an input or output?

---

## References
- [Visual Studio Code](https://code.visualstudio.com/download)
- [MPLAB Extension Pack (VS Marketplace)](https://marketplace.visualstudio.com/)
- [MPLAB XC Compilers](https://www.microchip.com/mplab/compilers)
- [Arduino CLI documentation](https://arduino.github.io/arduino-cli/latest/)
- [Arduino CLI upload command](https://arduino.github.io/arduino-cli/latest/commands/arduino-cli_upload/)
- [ATmega328P product page](https://www.microchip.com/en-us/product/atmega328p)