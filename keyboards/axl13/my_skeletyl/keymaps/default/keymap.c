// Skeletyl handwired (ATmega32U4, SH1106 OLED) — keymap v3.4
//
// Mirrors github.com/axl13/zmk-config-bkb (Kinesis Advantage aligned):
//   Thumbs:  Bspc  Cmd  Esc/Sym   |   Tab/Sym  Enter/Fn  Space
//   Combos:  Q+W =   Z+X `   O+P \   L+; '   ,+. [   .+/ ]
//            Shift on the OPPOSITE hand gives + ~ | " { }
//   Home-row mods are positional (Chordal Hold): a mod only holds when the
//   next key is on the other hand or a thumb.
//   Sentence case: getreuer/sentence_case community module (keymap.json).
//
// OLED: key-press histogram, average tap time and idle wave animation
// (carried over from keymaps/legacy).
//
// Key positions (layout order, used by combos and chordal_hold_layout):
//    0  1  2  3  4      5  6  7  8  9
//   10 11 12 13 14     15 16 17 18 19
//   20 21 22 23 24     25 26 27 28 29
//         30 31 32     33 34 35

#include QMK_KEYBOARD_H
#include <math.h>
#include <stdio.h>
#include <string.h>

enum layers {
    _BASE,
    _SYMBOLS,
    _FUNCTION,
};

enum custom_keycodes {
    RESET_STATS = SAFE_RANGE,
};

// Home-row mods (macOS: GUI = Cmd, ALT = Option)
#define HM_A    LSFT_T(KC_A)
#define HM_S    LCTL_T(KC_S)
#define HM_D    LGUI_T(KC_D)
#define HM_F    LALT_T(KC_F)
#define HM_J    RALT_T(KC_J)
#define HM_K    RGUI_T(KC_K)
#define HM_L    RCTL_T(KC_L)
#define HM_SCLN RSFT_T(KC_SCLN)

// Thumb layer keys
#define SYM_ESC LT(_SYMBOLS, KC_ESC)
#define SYM_TAB LT(_SYMBOLS, KC_TAB)
#define FN_ENT  LT(_FUNCTION, KC_ENT)

#define EURO    LALT(LSFT(KC_2)) // macOS US layout: Option+Shift+2
#define SC_TOGG SENTENCE_CASE_TOGGLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        HM_A,    HM_S,    HM_D,    HM_F,    KC_G,         KC_H,    HM_J,    HM_K,    HM_L,    HM_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_BSPC, KC_LGUI, SYM_ESC,      SYM_TAB, FN_ENT,  KC_SPC
    ),

    [_SYMBOLS] = LAYOUT_split_3x5_3(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_GRV,  KC_EQL,  KC_LEFT, KC_RGHT, KC_COLN,      KC_DOT,  KC_UP,   KC_DOWN, KC_UNDS, KC_MINS,
                          KC_DEL,  KC_LCTL, _______,      _______, KC_ENT,  KC_SPC
    ),

    // ZMK's Bluetooth / soft-off / ext-power keys have no meaning on this wired board.
    [_FUNCTION] = LAYOUT_split_3x5_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_F11,  KC_F12,  EURO,  RESET_STATS, SC_TOGG,    KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, XXXXXXX,
        XXXXXXX, XXXXXXX, KC_HOME, KC_END,  XXXXXXX,      KC_INS,  KC_PGUP, KC_PGDN, KC_BRID, KC_BRIU,
                          KC_DEL,  KC_LCTL, KC_END,       _______, _______, KC_SPC
    ),
};

// Chordal Hold handedness: thumbs are exempt ('*') so mod + thumb always works.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_split_3x5_3(
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
              '*', '*', '*',      '*', '*', '*'
);
// clang-format on

// ---------------------------------------------------------------------------
// Combos — Kinesis positions; the combo sends the unshifted key so that Shift
// held on the opposite hand produces the shifted symbol.
// ---------------------------------------------------------------------------
enum combo_events {
    C_QW_EQL,
    C_ZX_GRV,
    C_OP_BSLS,
    C_LSCLN_QUOT,
    C_COMMDOT_LBRC,
    C_DOTSLSH_RBRC,
};

const uint16_t PROGMEM qw_combo[]      = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM zx_combo[]      = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM op_combo[]      = {KC_O, KC_P, COMBO_END};
const uint16_t PROGMEM lscln_combo[]   = {HM_L, HM_SCLN, COMBO_END};
const uint16_t PROGMEM commdot_combo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM dotslsh_combo[] = {KC_DOT, KC_SLSH, COMBO_END};

combo_t key_combos[] = {
    [C_QW_EQL]       = COMBO(qw_combo, KC_EQL),
    [C_ZX_GRV]       = COMBO(zx_combo, KC_GRV),
    [C_OP_BSLS]      = COMBO(op_combo, KC_BSLS),
    [C_LSCLN_QUOT]   = COMBO(lscln_combo, KC_QUOT),
    [C_COMMDOT_LBRC] = COMBO(commdot_combo, KC_LBRC),
    [C_DOTSLSH_RBRC] = COMBO(dotslsh_combo, KC_RBRC),
};

static uint16_t last_key_press_time = 0;

// Base layer only; "op" (stop, open) and "./" (paths) also need a short pause
// before them, like require-prior-idle-ms in the ZMK config.
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (get_highest_layer(layer_state) != _BASE) {
        return false;
    }
    switch (combo_index) {
        case C_OP_BSLS:
        case C_DOTSLSH_RBRC:
            return timer_elapsed(last_key_press_time) > COMBO_PRIOR_IDLE_MS;
    }
    return true;
}

// Thumb layer keys keep tap-then-hold auto-repeat (Bspc-style); home-row mods don't.
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SYM_ESC:
        case SYM_TAB:
        case FN_ENT:
            return 175;
        default:
            return 0;
    }
}

// ---------------------------------------------------------------------------
// OLED: advanced key-press histogram (from keymaps/legacy)
// ---------------------------------------------------------------------------
#define HISTOGRAM_SIZE 60
#define DISPLAY_SLOTS_COUNT 21
#define IDLE_TIMEOUT 15000 // 15 seconds

static uint32_t key_press_counts[HISTOGRAM_SIZE] = {0};
static uint16_t press_time_cache[HISTOGRAM_SIZE] = {0};

typedef struct {
    uint16_t keycode;
    uint32_t count;
} key_count_t;

static key_count_t display_slots[DISPLAY_SLOTS_COUNT] = {0};
static uint32_t    total_tap_time                     = 0;
static uint32_t    total_tap_count                    = 0;
static uint32_t    last_activity_timer                = 0;

static int16_t keycode_to_histogram_index(uint16_t keycode) {
    if (keycode >= KC_A && keycode <= KC_Z) return keycode - KC_A;
    if (keycode >= KC_1 && keycode <= KC_9) return 26 + (keycode - KC_1);
    switch (keycode) {
        case KC_0:    return 35;
        case KC_UNDS: return 36;
        case KC_LPRN: return 37;
        case KC_RPRN: return 38;
        case KC_DOT:  return 39;
        case KC_COMM: return 40;
        case KC_EQL:  return 41;
        case KC_SLSH: return 42;
        case KC_SCLN: return 43;
        case KC_QUOT: return 44;
        case KC_GRV:  return 45;
        default:      return -1;
    }
}

static void update_histogram_display(uint16_t keycode, uint32_t count) {
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
    if (min_index >= 0 && count > min_count) {
        display_slots[min_index].keycode = keycode;
        display_slots[min_index].count   = count;
    }
}

static void increment_key_count(uint16_t keycode) {
    int16_t index = keycode_to_histogram_index(keycode);
    if (index != -1) {
        update_histogram_display(keycode, ++key_press_counts[index]);
    }
}

static void reset_statistics(void) {
    memset(key_press_counts, 0, sizeof(key_press_counts));
    memset(display_slots, 0, sizeof(display_slots));
    memset(press_time_cache, 0, sizeof(press_time_cache));
    total_tap_time  = 0;
    total_tap_count = 0;
}

// Home-row mods and layer-taps report e.g. LCTL_T(KC_S); count their tap key
// only when they were actually tapped.
static uint16_t histogram_keycode(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_MOD_TAP(keycode)) {
        return record->tap.count ? QK_MOD_TAP_GET_TAP_KEYCODE(keycode) : KC_NO;
    }
    if (IS_QK_LAYER_TAP(keycode)) {
        return record->tap.count ? QK_LAYER_TAP_GET_TAP_KEYCODE(keycode) : KC_NO;
    }
    return keycode;
}

void keyboard_post_init_user(void) {
    last_activity_timer = timer_read32();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_activity_timer = timer_read32();
    if (record->event.pressed) {
        last_key_press_time = timer_read();
    }

    if (keycode == RESET_STATS) {
        if (record->event.pressed) reset_statistics();
        return false;
    }

    uint16_t kc    = histogram_keycode(keycode, record);
    int16_t  index = keycode_to_histogram_index(kc);
    if (index != -1) {
        if (record->event.pressed) {
            press_time_cache[index] = record->event.time;
        } else if (press_time_cache[index] != 0) {
            increment_key_count(kc);
            total_tap_time += timer_elapsed(press_time_cache[index]);
            total_tap_count++;
            press_time_cache[index] = 0;
        }
    }
    return true;
}

#ifdef OLED_ENABLE

static void write_slot_letters(void) {
    char display_string[DISPLAY_SLOTS_COUNT + 1];
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        uint16_t keycode = display_slots[i].keycode;
        if (keycode >= KC_A && keycode <= KC_Z) {
            display_string[i] = 'a' + (keycode - KC_A);
        } else if (keycode >= KC_1 && keycode <= KC_0) {
            display_string[i] = (keycode == KC_0) ? '0' : '1' + (keycode - KC_1);
        } else {
            display_string[i] = ' ';
        }
    }
    display_string[DISPLAY_SLOTS_COUNT] = '\0';
    oled_set_cursor(0, 3);
    oled_write(display_string, false);
}

static void draw_bar(uint8_t slot, uint8_t bar_height) {
    const uint8_t char_width_pixels = 6;
    uint8_t       bar_start_x       = slot * char_width_pixels;
    for (uint8_t y = 0; y < bar_height; y++) {
        for (uint8_t x = 0; x < char_width_pixels - 1; x++) {
            oled_write_pixel(bar_start_x + x, 23 - y, true);
        }
    }
}

static void render_histogram(void) {
    uint32_t max_count = 0;
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].count > max_count) max_count = display_slots[i].count;
    }
    if (max_count == 0) return;

    write_slot_letters();
    const uint8_t max_bar_height = 24;
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        if (display_slots[i].count == 0) continue;
        uint8_t bar_height = (uint8_t)((float)display_slots[i].count / max_count * max_bar_height);
        if (bar_height > 0) draw_bar(i, bar_height);
    }
}

static void render_idle_wave(void) {
    oled_clear();
    write_slot_letters();

    float         phase          = timer_read32() / 600.0f;
    float         phase_rand     = timer_read32() / 250.0f;
    const uint8_t max_bar_height = 24;
    for (int i = 0; i < DISPLAY_SLOTS_COUNT; i++) {
        float random_mod = sinf(1.0f * i + phase_rand);
        float amplitude  = 10.0f + (random_mod * 3.0f);
        int   bar_height = (int)(amplitude * sinf(0.5f * i + phase) + 12.0f);
        if (bar_height < 0) bar_height = 0;
        if (bar_height > max_bar_height) bar_height = max_bar_height;
        if (bar_height > 0) draw_bar(i, (uint8_t)bar_height);
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
            char     buffer[8];
            snprintf(buffer, sizeof(buffer), "%ums", avg_time);
            oled_set_cursor(16, 0); // top right corner
            oled_write(buffer, false);
        }
    }
    return false;
}

#endif // OLED_ENABLE
