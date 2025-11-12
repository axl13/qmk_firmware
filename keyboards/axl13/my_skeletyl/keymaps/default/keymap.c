#include QMK_KEYBOARD_H
#include "split_common/transactions.h"


#include "drivers/oled/oled_driver.h"
#include <math.h> // For sine wave animation

// -- Advanced Key Press Histogram --

// 1. EXPANDED DATA STRUCTURES
#define HISTOGRAM_SIZE 60
#define DISPLAY_SLOTS_COUNT 21
static uint32_t key_press_counts[HISTOGRAM_SIZE] = {0};
static uint16_t press_time_cache[HISTOGRAM_SIZE] = {0}; // Cache for tap time measurement

typedef struct {
    uint16_t keycode;
    uint32_t count;
} key_count_t;

static key_count_t display_slots[DISPLAY_SLOTS_COUNT] = {0};

// 2. AVERAGE TAP TIME TRACKING
static uint32_t total_tap_time  = 0;
static uint32_t total_tap_count = 0;

// 3. IDLE ANIMATION TRACKING
#define IDLE_TIMEOUT 15000 // 15 seconds
static uint32_t last_activity_timer = 0;

// Custom keycode for resetting statistics
enum custom_keycodes {
    RESET_STATS = SAFE_RANGE,
};

// Expanded mapping to include common symbols
int16_t keycode_to_histogram_index(uint16_t keycode) {
    if (keycode >= KC_A && keycode <= KC_Z) {
        return keycode - KC_A;
    }
    if (keycode >= KC_1 && keycode <= KC_9) {
        return 26 + (keycode - KC_1);
    }
    if (keycode == KC_0) {
        return 35;
    }
    switch (keycode) {
        case KC_UNDS:
            return 36;
        case KC_LPRN:
            return 37;
        case KC_RPRN:
            return 38;
        case KC_DOT:
            return 39;
        case KC_COMM:
            return 40;
        case KC_EQL:
            return 41;
        case KC_SLSH:
            return 42;
        case KC_SCLN:
            return 43;
        case KC_QUOT:
            return 44;
        case KC_GRV:
            return 45;
        default:
            return -1;
    }
}

uint16_t histogram_index_to_keycode(int16_t index) {
    if (index >= 0 && index < 26) {
        return KC_A + index;
    }
    if (index >= 26 && index < 35) {
        return KC_1 + (index - 26);
    }
    if (index == 35) {
        return KC_0;
    }
    switch (index) {
        case 36:
            return KC_UNDS;
        case 37:
            return KC_LPRN;
        case 38:
            return KC_RPRN;
        case 39:
            return KC_DOT;
        case 40:
            return KC_COMM;
        case 41:
            return KC_EQL;
        case 42:
            return KC_SLSH;
        case 43:
            return KC_SCLN;
        case 44:
            return KC_QUOT;
        case 45:
            return KC_GRV;
        default:
            return KC_NO;
    }
}

void update_histogram_display(uint16_t keycode, uint32_t count) {
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].keycode == keycode) {
            display_slots[i].count = count;
            return;
        }
    }
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].keycode == KC_NO || display_slots[i].count == 0) {
            display_slots[i].keycode = keycode;
            display_slots[i].count   = count;
            return;
        }
    }
    int      min_index = -1;
    uint32_t min_count = UINT32_MAX;
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].count < min_count) {
            min_count = display_slots[i].count;
            min_index = i;
        }
    }
    if (count > min_count) {
        display_slots[min_index].keycode = keycode;
        display_slots[min_index].count   = count;
    }
}

void increment_key_count(uint16_t keycode) {
    int16_t index = keycode_to_histogram_index(keycode);
    if (index != -1) {
        update_histogram_display(keycode, ++key_press_counts[index]);
    }
}

void reset_statistics(void) {
    memset(key_press_counts, 0, sizeof(key_press_counts));
    memset(display_slots, 0, sizeof(display_slots));
    memset(press_time_cache, 0, sizeof(press_time_cache));
    total_tap_time  = 0;
    total_tap_count = 0;
}
// -- End of Advanced Key Press Histogram --

enum combos { UI_DQUOT, MCOM_UNDS, IO_QUOT, EW_GRV };
const uint16_t PROGMEM ui_dquot_combo[]  = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM mcom_unds_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM io_quot_combo[]   = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM ew_grv_combo[]    = {KC_E, KC_W, COMBO_END};
combo_t                key_combos[]      = {
    [UI_DQUOT]  = COMBO_ACTION(ui_dquot_combo),
    [MCOM_UNDS] = COMBO_ACTION(mcom_unds_combo),
    [IO_QUOT]   = COMBO_ACTION(io_quot_combo),
    [EW_GRV]    = COMBO_ACTION(ew_grv_combo),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT_split_3x5_3(KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_BSPC, KC_LGUI, KC_TAB, KC_ESC, KC_ENT, KC_SPC), [1] = LAYOUT_split_3x5_3(KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_GRV, KC_TILD, KC_LEFT, KC_RGHT, KC_EQL, KC_MINS, KC_UP, KC_DOWN, KC_DQUO, KC_QUOT, KC_DEL, KC_LCTL, KC_TRNS, MO(0), KC_ENT, KC_SPC), [2] = LAYOUT_split_3x5_3(KC_PLUS, LSFT(KC_MINS), RESET_STATS, KC_NO, KC_NO, KC_CIRC, KC_AMPR, KC_DQUO, KC_LCBR, KC_RCBR, KC_EQL, KC_GRV, KC_NO, KC_NO, KC_NO, KC_PLUS, KC_MINS, KC_EQL, KC_NUBS, KC_PIPE, KC_TILD, KC_PIPE, KC_HOME, KC_END, KC_NO, KC_UNDS, KC_PGUP, KC_PGDN, KC_LBRC, KC_RBRC, KC_DEL, KC_LCTL, KC_END, KC_TRNS, KC_ENT, KC_RALT)};

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (pressed) {
        switch (combo_index) {
            case UI_DQUOT:
                tap_code16(KC_DQUO);
                break;
            case MCOM_UNDS:
                tap_code16(KC_UNDS);
                break;
            case IO_QUOT:
                tap_code16(KC_QUOT);
                break;
            case EW_GRV:
                tap_code16(KC_GRV);
                break;
        }
    }
}

void matrix_init_user(void) {
    last_activity_timer = timer_read32();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_activity_timer = timer_read32();

    if (keycode == RESET_STATS && record->event.pressed) {
        reset_statistics();
        return false;
    }

    int16_t index = keycode_to_histogram_index(keycode);
    if (index != -1 && record->event.pressed) {
        press_time_cache[index] = record->event.time;
    }

    if (index != -1 && !record->event.pressed) {
        uint16_t press_time = press_time_cache[index];
        if (press_time != 0) {
            increment_key_count(keycode);
            total_tap_time += timer_elapsed(press_time);
            total_tap_count++;
            press_time_cache[index] = 0;
        }
    }
    return true;
}

void matrix_scan_user(void) {}

#ifdef OLED_ENABLE

void render_histogram(void) {
    uint32_t max_count = 0;
    char     display_string[DISPLAY_SLOTS_COUNT + 1];
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].count > max_count) max_count = display_slots[i].count;
        uint16_t keycode = display_slots[i].keycode;
        if (keycode >= KC_A && keycode <= KC_Z) {
            display_string[i] = 'a' + (keycode - KC_A);
        } else if (keycode >= KC_0 && keycode <= KC_9) {
            display_string[i] = (keycode == KC_0) ? '0' : '1' + (keycode - KC_1);
        } else {
            display_string[i] = ' ';
        }
    }
    display_string[DISPLAY_SLOTS_COUNT] = '\0';

    if (max_count == 0) return;

    const uint8_t char_width_pixels = 6;
    const uint8_t max_bar_height    = 24;
    oled_set_cursor(0, 3);
    oled_write(display_string, false);

    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].count == 0) continue;
        uint8_t bar_height = (uint8_t)((float)display_slots[i].count / max_count * max_bar_height);
        if (bar_height > 0) {
            uint8_t bar_start_x = i * char_width_pixels;
            for (int y = 0; y < bar_height; y++) {
                for (int x = 0; x < char_width_pixels - 1; x++) {
                    oled_write_pixel(bar_start_x + x, 23 - y, true);
                }
            }
        }
    }
}

void render_idle_wave(void) {
    oled_clear();
    char display_string[DISPLAY_SLOTS_COUNT + 1];
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        uint16_t keycode = display_slots[i].keycode;
        if (keycode >= KC_A && keycode <= KC_Z) {
            display_string[i] = 'a' + (keycode - KC_A);
        } else if (keycode >= KC_0 && keycode <= KC_9) {
            display_string[i] = (keycode == KC_0) ? '0' : '1' + (keycode - KC_1);
        } else {
            display_string[i] = ' ';
        }
    }
    display_string[DISPLAY_SLOTS_COUNT] = '\0';
    oled_set_cursor(0, 3);
    oled_write(display_string, false);

    float         phase             = timer_read32() / 600.0f;
    float         phase_rand        = timer_read32() / 250.0f;
    const uint8_t char_width_pixels = 6;
    const uint8_t max_bar_height    = 24;

    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        float random_mod = sinf(1.0f * i + phase_rand);
        float amplitude  = 10.0f + (random_mod * 3.0f);
        int   bar_height = (int)(amplitude * sinf(0.5f * i + phase) + 12.0f);
        if (bar_height < 0) bar_height = 0;
        if (bar_height > max_bar_height) bar_height = max_bar_height;

        if (bar_height > 0) {
            uint8_t bar_start_x = i * char_width_pixels;
            for (int y = 0; y < bar_height; y++) {
                for (int x = 0; x < char_width_pixels - 1; x++) {
                    oled_write_pixel(bar_start_x + x, 23 - y, true);
                }
            }
        }
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        if (timer_elapsed32(last_activity_timer) > IDLE_TIMEOUT) {
            render_idle_wave();
        } else {
            oled_clear();
            render_histogram();
            uint16_t avg_time = (total_tap_count > 0) ? (uint16_t)(total_tap_time / total_tap_count) : 0;
            char     buffer[6];
            sprintf(buffer, "%ums", avg_time);
            oled_set_cursor(16, 0); // Top right corner (adjusted for string length)
            oled_write(buffer, false);
        }
    }
    return false;
}

#endif