#pragma once

// --- Hand-wired split + OLED (keep: matches the physical build) ---
#define MASTER_LEFT
#define SOFT_SERIAL_PIN D2
#define SPLIT_OLED_ENABLE
#define OLED_BRIGHTNESS 64
#define OLED_UPDATE_INTERVAL 40
#define USB_POLLING_INTERVAL_MS 1

// --- Debounce: 15 ms instead of QMK's default 5 (the T switch chatters and typed "tt").
// With asym_eager_defer_pk a press still registers at once; only the release waits.
#define DEBOUNCE 15

// --- Trimmed 6x8 OLED font (printable ASCII + arrow + 12x16 Apple/Tux icons) ---
#define OLED_FONT_H "oled_font.h"
#define OLED_FONT_START 32
#define OLED_FONT_END 135

// --- Flash diet: 3 layers fit in 8-bit layer mask ---
#define LAYER_STATE_8BIT

// --- Tap-hold: mirrors the ZMK config (balanced flavor + positional mods) ---
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD          // opposite-hand rule for home-row mods
#define QUICK_TAP_TERM_PER_KEY
#define FLOW_TAP_TERM 150     // Ctrl/Cmd/Alt home-row mod pressed <150 ms after a typing key = tap (not Shift, see keymap.c)

// --- Caps Word: hold both home-row Shifts (A + ;) ---
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// --- Combos (Kinesis-position symbols + A+S = Esc) ---
#define COMBO_TERM 40
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER
#define COMBO_PRIOR_IDLE_MS 150

// --- Agent status lights: one byte from the Mac (Raw HID), right-half bits passed on over the split link ---
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_LIGHTS
#define RPC_M2S_BUFFER_SIZE 2
#define RPC_S2M_BUFFER_SIZE 2

// --- Sentence case (getreuer/sentence_case) ---
#define SENTENCE_CASE_TIMEOUT 2000
