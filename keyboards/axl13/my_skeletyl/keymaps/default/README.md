# Skeletyl/Tracktyl Custom Firmware

This document outlines the features, layout, and build instructions for this custom QMK firmware for the Skeletyl/Tracktyl keyboard.

## How to Build

To build this firmware, you need a working QMK build environment.

1.  **Clone the Repository:**
    Ensure you have this entire `qmk_firmware` fork cloned to your local machine.

2.  **Navigate to the QMK Directory:**
    Open your terminal and navigate to the root of the `qmk_firmware_new` directory:
    ```bash
    cd /path/to/your/skeletyl/qmk_firmware_new
    ```

3.  **Run the Compile Command:**
    Execute the following command to build the firmware.
    ```bash
    qmk compile -kb axl13/my_skeletyl -km default
    ```

4.  **Find the Firmware File:**
    Upon successful compilation, the `.hex` file will be located in the `qmk_firmware_new` root directory, named `axl13_my_skeletyl_default.hex`.

5.  **Flash the Firmware:**
    Use the QMK Toolbox or your preferred flashing utility to flash the generated `.hex` file to your keyboard.

---

## Custom Features

This firmware includes several custom features integrated directly into the keymap.

### 1. Advanced OLED Display

The OLED screen provides real-time feedback and statistics.

*   **Key Press Histogram:** The primary screen displays a histogram of your most frequently pressed keys. It tracks letters and numbers, showing you a visual representation of your typing habits.
*   **Average Tap Time:** In the top right corner, a small display shows the average time (in milliseconds) for your key taps.
*   **Idle Animation:** If the keyboard is idle for more than 15 seconds, the histogram is replaced by an animated sine wave for a simple screen-saver effect.

### 2. Statistics Reset

A custom keycode, `RESET_STATS`, is available on Layer 2. Pressing this key will reset all accumulated statistics for the key press histogram and average tap time.

### 3. Key Combos

To make typing common character pairs easier, several combos are configured:

| Keys Pressed   | Character Produced |
| :------------- | :----------------- |
| `U` + `I`      | `"` (Double Quote) |
| `M` + `,`      | `_` (Underscore)   |
| `I` + `O`      | `'` (Single Quote) |
| `E` + `W`      | `` ` `` (Backtick)     |

---

## Keymap Layouts

There are 3 layers defined in this keymap: Base, Symbols, and Utility.

### Layer 0: Base Layer

This is the default layer for standard typing.

| Q     | W     | E     | R     | T     | | Y     | U     | I     | O     | P     |
| :---- | :---- | :---- | :---- | :---- | :- | :---- | :---- | :---- | :---- | :---- |
| A     | S     | D     | F     | G     | | H     | J     | K     | L     | ;     |
| Z     | X     | C     | V     | B     | | N     | M     | ,     | .     | /     |
|       |       | **LGui**  | **Tab**   | **BSPC**  | | **SPC** | **Enter** | **ESC**   |       |       |

### Layer 1: Symbols & Numbers

Accessed by holding one of the thumb keys.

| !     | @     | #     | $     | %     | | ^     | &     | *     | (     | )     |
| :---- | :---- | :---- | :---- | :---- | :- | :---- | :---- | :---- | :---- | :---- |
| 1     | 2     | 3     | 4     | 5     | | 6     | 7     | 8     | 9     | 0     |
| `    | ~     | Left  | Right | =     | | -     | Up    | Down  | "    | '     |
|       |       | **LCtl**  | **(trans)** | **Del**   | | **SPC** | **Enter** | **MO(0)** |       |       |

### Layer 2: Utility & Reset

| +        | _           | **RST_ST** | (no)  | (no)  | | ^     | &     | "    | {     | }     |
| :------- | :---------- | :--------- | :---- | :---- | :- | :---- | :---- | :---- | :---- | :---- |
| =        | `          | (no)       | (no)  | (no)  | | +     | -     | =     | \    | |    |
| ~        | |          | Home       | End   | (no)  | | _     | PgUp  | PgDn  | [     | ]     |
|          |             | **LCtl**     | **End**   | **Del**   | | **RAlt**| **Enter** | **(trans)** |       |       |

*   **RST_ST**: Resets the OLED statistics.
*   **MO(0)**: Momentarily activates Layer 0 while held.
*   **(trans)**: Transparent key, passes through to the layer below.
