#pragma once

// --- Hand-wired split + OLED (keep: matches the physical build) ---
#define MASTER_LEFT
#define SOFT_SERIAL_PIN D2
#define SPLIT_OLED_ENABLE
#define OLED_BRIGHTNESS 64
#define OLED_UPDATE_INTERVAL 40
#define USB_POLLING_INTERVAL_MS 1
#define MAX_DEFERRED_EXECUTORS 10

// --- Tap-hold: mirrors the ZMK config (balanced flavor + positional mods) ---
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD          // opposite-hand rule for home-row mods
#define QUICK_TAP_TERM_PER_KEY

// --- Combos (Kinesis-position symbols) ---
#define COMBO_TERM 40
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER
#define COMBO_PRIOR_IDLE_MS 150

// --- Sentence case (getreuer/sentence_case) ---
#define SENTENCE_CASE_TIMEOUT 2000
