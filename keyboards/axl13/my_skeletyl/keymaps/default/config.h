#pragma once

// Match the Skeletyl ZMK keymap's tap/hold timing.
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0
#define PERMISSIVE_HOLD

// Preserve the hand-wired split connection and OLED settings.
#define MASTER_LEFT
#define SOFT_SERIAL_PIN D2
#define SPLIT_OLED_ENABLE
#define OLED_BRIGHTNESS 64
#define OLED_UPDATE_INTERVAL 40
#define USB_POLLING_INTERVAL_MS 1
#define MAX_DEFERRED_EXECUTORS 10
