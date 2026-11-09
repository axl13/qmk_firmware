// Skeletyl handwired (ATmega32U4, SH1106 OLED) — keymap v3.6
//
// Mirrors github.com/axl13/zmk-config-bkb (Kinesis Advantage aligned):
//   Thumbs:  Bspc  Cmd  Esc/Sym   |   Tab/Sym  Enter/Fn  Space
//   Combos:  Q+W =   A+S Esc   Z+X `   I+O -   O+P \   L+; '   ,+. [   .+/ ]
//            Shift on the OPPOSITE hand gives + ~ _ | " { }
//   Home-row mods are positional (Chordal Hold): a mod only holds when the
//   next key is on the other hand or a thumb.
//   Caps Word: hold both home-row Shifts (A + ;).
//   Sentence case: getreuer/sentence_case community module (keymap.json).
//   Autocorrect: 221-entry dictionary (Base layer only), toggle on Fn + ;.
//   OS Detection: auto Mac/Linux logo + Ctrl<->GUI swap, cycle on Fn + Z.
//
// OLED (128x32 SH1106, Cyberpunk Telemetry HUD):
//   - Active typing: framed OS icon (Apple/Tux), 2x 7-seg WPM + tap ms,
//     live scrolling keystroke ECG waveform, layer/AC/CW/mod status stack.
//   - Autocorrect toast: full-screen "// AC INTERCEPT" card for 1.5 s.
//   - Layer X-Ray: live symbol/function map when holding Sym/Fn thumb.
//   - Idle (after 15 s): alternates "// LEARNED TYPOS" log & dual-trace scope.

#include QMK_KEYBOARD_H
#include <string.h>

#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

enum layers {
    _BASE,
    _SYMBOLS,
    _FUNCTION,
};

enum custom_keycodes {
    RESET_STATS = SAFE_RANGE,
    OS_TOGG,
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

    [_FUNCTION] = LAYOUT_split_3x5_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_F11,  KC_F12,  EURO,  RESET_STATS, SC_TOGG,    KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, AC_TOGG,
        OS_TOGG, XXXXXXX, KC_HOME, KC_END,  XXXXXXX,      KC_INS,  KC_PGUP, KC_PGDN, KC_BRID, KC_BRIU,
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
// Combos — Kinesis positions + A+S = Esc
// ---------------------------------------------------------------------------
enum combo_events {
    C_QW_EQL,
    C_AS_ESC,
    C_ZX_GRV,
    C_IO_MINS,
    C_OP_BSLS,
    C_LSCLN_QUOT,
    C_COMMDOT_LBRC,
    C_DOTSLSH_RBRC,
};

const uint16_t PROGMEM qw_combo[]      = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM as_combo[]      = {HM_A, HM_S, COMBO_END};
const uint16_t PROGMEM zx_combo[]      = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM io_combo[]      = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM op_combo[]      = {KC_O, KC_P, COMBO_END};
const uint16_t PROGMEM lscln_combo[]   = {HM_L, HM_SCLN, COMBO_END};
const uint16_t PROGMEM commdot_combo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM dotslsh_combo[] = {KC_DOT, KC_SLSH, COMBO_END};

combo_t key_combos[] = {
    [C_QW_EQL]       = COMBO(qw_combo, KC_EQL),
    [C_AS_ESC]       = COMBO(as_combo, KC_ESC),
    [C_ZX_GRV]       = COMBO(zx_combo, KC_GRV),
    [C_IO_MINS]      = COMBO(io_combo, KC_MINS),
    [C_OP_BSLS]      = COMBO(op_combo, KC_BSLS),
    [C_LSCLN_QUOT]   = COMBO(lscln_combo, KC_QUOT),
    [C_COMMDOT_LBRC] = COMBO(commdot_combo, KC_LBRC),
    [C_DOTSLSH_RBRC] = COMBO(dotslsh_combo, KC_RBRC),
};

static uint16_t last_key_press_time = 0;

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

// I+O and A+S use a tight 25 ms window so fast rolls through "io" / "as" stay letters.
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case C_IO_MINS:
        case C_AS_ESC:
            return 25;
        default:
            return COMBO_TERM;
    }
}

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
// OS Detection & Mac <-> Linux Modifier Swap
//   OS_MODE_MAC         : native (Thumb = Cmd, D/K = Cmd, S/L = Ctrl)
//   OS_MODE_LINUX_FULL  : swap Ctrl <-> GUI on both thumb and home-row D/K/S/L
//   OS_MODE_LINUX_THUMB : swap Ctrl <-> GUI on thumb only (home-row S/L stay Ctrl)
// ---------------------------------------------------------------------------
enum os_mode_t {
    OS_MODE_MAC         = 0,
    OS_MODE_LINUX_FULL  = 1,
    OS_MODE_LINUX_THUMB = 2,
};

static uint8_t current_os_mode = OS_MODE_MAC;

uint16_t keycode_config(uint16_t keycode) {
    if (current_os_mode != OS_MODE_MAC) {
        switch (keycode) {
            case KC_LEFT_GUI:  return KC_LEFT_CTRL;
            case KC_LEFT_CTRL: return KC_LEFT_GUI;
            case KC_RIGHT_GUI: return KC_RIGHT_CTRL;
            case KC_RIGHT_CTRL:return KC_RIGHT_GUI;
        }
    }
    return keycode;
}

uint8_t mod_config(uint8_t mod) {
    if (current_os_mode == OS_MODE_LINUX_FULL) {
        if (((mod & MOD_RCTL) == MOD_LCTL) ^ ((mod & MOD_RGUI) == MOD_LGUI)) {
            mod ^= (MOD_LCTL | MOD_LGUI);
        }
        if (((mod & MOD_RCTL) == MOD_RCTL) ^ ((mod & MOD_RGUI) == MOD_RGUI)) {
            mod ^= (MOD_LCTL | MOD_LGUI);
        }
    }
    return mod;
}

#ifdef OS_DETECTION_ENABLE
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_LINUX:
        case OS_WINDOWS:
            current_os_mode = OS_MODE_LINUX_FULL;
            break;
        case OS_MACOS:
        case OS_IOS:
            current_os_mode = OS_MODE_MAC;
            break;
        default:
            break;
    }
    return true;
}
#endif

// ---------------------------------------------------------------------------
// Telemetry, Autocorrect Intercept & On-Device Typo Learner
// ---------------------------------------------------------------------------
#define IDLE_TIMEOUT 15000     // 15 s before idle screen
#define INTERCEPT_MS 1600      // show "// AC INTERCEPT" card for 1.6 s
#define LEARNED_SLOTS 5        // persisted in 64-byte user EEPROM datablock
#define WORD_MAX 8

typedef struct {
    char    typo[WORD_MAX];
    char    fix[WORD_MAX];
    uint8_t count;
} learned_entry_t;

typedef struct {
    uint8_t         magic;
    uint8_t         ac_hits;
    learned_entry_t entries[3]; // 2 + 3 * 17 = 53 bytes (fits in 64B datablock)
} persisted_log_t;

#define LOG_MAGIC 0xA7

static learned_entry_t learned_typos[3];
static uint8_t         ac_hit_count     = 0;
static bool            eeprom_dirty     = false;
static bool            eeprom_flushed   = true;

// Intercept toast state
static char     intercept_typo[12];
static char     intercept_fix[12];
static uint16_t intercept_timer    = 0;
static bool     intercept_active   = false;

// Live typing telemetry (rolling WPM, tap duration, ECG pulse ring)
static uint32_t last_activity_timer = 0;
static uint16_t press_Timer         = 0;
static uint32_t total_tap_time      = 0;
static uint16_t total_tap_count     = 0;

// 8 buckets x 625 ms = 5-second rolling window for integer WPM (zero float)
static uint8_t  wpm_buckets[8];
static uint8_t  wpm_bucket_idx      = 0;
static uint16_t wpm_bucket_timer    = 0;
static uint8_t  current_wpm         = 0;

// 64-sample scrolling ECG trace (signed offsets from baseline y=26)
static int8_t   ecg_trace[64];
static uint8_t  ecg_head            = 0;
static uint8_t  ecg_spike_phase     = 0;
static const int8_t PROGMEM ecg_kernel[6] = {-2, 6, -9, 4, -1, 0};

// Backspace-correction detector state
static char    cur_word[12];
static uint8_t cur_len        = 0;
static char    erased_typo[12];
static uint8_t erased_at_len  = 0;
static uint8_t bspc_streak    = 0;

static void record_learned_pair(const char *typo, const char *fix) {
    if ( strcmp(typo, fix) == 0 ) return;
    uint8_t tl = strlen(typo), fl = strlen(fix);
    if (tl < 3 || fl < 3 || tl >= WORD_MAX || fl >= WORD_MAX) return;

    int8_t min_idx = 0;
    for (uint8_t i = 0; i < 3; i++) {
        if (learned_typos[i].count > 0 &&
            strncmp(learned_typos[i].typo, typo, WORD_MAX - 1) == 0 &&
            strncmp(learned_typos[i].fix, fix, WORD_MAX - 1) == 0) {
            if (learned_typos[i].count < 250) learned_typos[i].count++;
            eeprom_dirty = true;
            eeprom_flushed = false;
            return;
        }
        if (learned_typos[i].count < learned_typos[min_idx].count) {
            min_idx = i;
        }
    }
    strncpy(learned_typos[min_idx].typo, typo, WORD_MAX - 1);
    learned_typos[min_idx].typo[WORD_MAX - 1] = '\0';
    strncpy(learned_typos[min_idx].fix, fix, WORD_MAX - 1);
    learned_typos[min_idx].fix[WORD_MAX - 1] = '\0';
    learned_typos[min_idx].count = 1;
    eeprom_dirty   = true;
    eeprom_flushed = false;
}

static void learner_feed_key(uint16_t kc, uint8_t mods) {
    if (mods & (MOD_MASK_CTRL | MOD_MASK_GUI | MOD_MASK_ALT)) {
        cur_len = 0; erased_typo[0] = '\0'; bspc_streak = 0;
        return;
    }
    if (kc >= KC_A && kc <= KC_Z) {
        if (cur_len < sizeof(cur_word) - 1) {
            cur_word[cur_len++] = 'a' + (kc - KC_A);
            cur_word[cur_len]   = '\0';
        }
        bspc_streak = 0;
    } else if (kc == KC_BSPC) {
        if (cur_len > 0) {
            if (bspc_streak == 0 && cur_len >= 3 && cur_len < WORD_MAX) {
                memcpy(erased_typo, cur_word, cur_len + 1);
                erased_at_len = cur_len;
            }
            cur_word[--cur_len] = '\0';
            bspc_streak++;
        }
    } else if (kc == KC_SPC || kc == KC_ENT || kc == KC_DOT || kc == KC_COMM) {
        if (erased_typo[0] != '\0' && erased_at_len >= 3 && cur_len >= 3 && cur_len < WORD_MAX) {
            record_learned_pair(erased_typo, cur_word);
        }
        cur_len = 0; cur_word[0] = '\0';
        erased_typo[0] = '\0'; bspc_streak = 0;
    } else if (kc != KC_LSFT && kc != KC_RSFT) {
        cur_len = 0; erased_typo[0] = '\0'; bspc_streak = 0;
    }
}

#ifdef AUTOCORRECT_ENABLE
// Only run Autocorrect on the Base layer.
bool process_autocorrect_user(uint16_t *keycode, keyrecord_t *record, uint8_t *typo_buffer_size, uint8_t *mods) {
    if (get_highest_layer(layer_state) != _BASE) {
        *typo_buffer_size = 0;
        return false;
    }
    return process_autocorrect_default_handler(keycode, record, typo_buffer_size, mods);
}

bool apply_autocorrect(uint8_t backspaces, const char *str, char *typo, char *correct) {
    strncpy(intercept_typo, typo, sizeof(intercept_typo) - 1);
    intercept_typo[sizeof(intercept_typo) - 1] = '\0';
    strncpy(intercept_fix, correct, sizeof(intercept_fix) - 1);
    intercept_fix[sizeof(intercept_fix) - 1] = '\0';
    intercept_timer  = timer_read();
    intercept_active = true;
    if (ac_hit_count < 250) ac_hit_count++;
    eeprom_dirty   = true;
    eeprom_flushed = false;
    // Clear manual learner state so an autocorrect doesn't look like a manual backspace fix
    cur_len = 0; erased_typo[0] = '\0'; bspc_streak = 0;
    return true;
}
#endif

static void save_log_to_eeprom(void) {
    persisted_log_t data;
    data.magic   = LOG_MAGIC;
    data.ac_hits = ac_hit_count;
    memcpy(data.entries, learned_typos, sizeof(learned_typos));
    eeconfig_update_user_datablock(&data, 0, sizeof(data));
    eeprom_dirty   = false;
    eeprom_flushed = true;
}

static void load_log_from_eeprom(void) {
    persisted_log_t data;
    eeconfig_read_user_datablock(&data, 0, sizeof(data));
    if (data.magic == LOG_MAGIC) {
        ac_hit_count = data.ac_hits;
        memcpy(learned_typos, data.entries, sizeof(learned_typos));
    }
}

static void reset_statistics(void) {
    total_tap_time  = 0;
    total_tap_count = 0;
    ac_hit_count    = 0;
    memset(wpm_buckets, 0, sizeof(wpm_buckets));
    memset(learned_typos, 0, sizeof(learned_typos));
    save_log_to_eeprom();
}

static uint16_t base_tap_keycode(uint16_t keycode, keyrecord_t *record) {
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
    wpm_bucket_timer    = timer_read();
    load_log_from_eeprom();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_activity_timer = timer_read32();

    if (record->event.pressed) {
        last_key_press_time = timer_read();
        press_Timer         = record->event.time;
        if (wpm_buckets[wpm_bucket_idx] < 250) {
            wpm_buckets[wpm_bucket_idx]++;
        }
        ecg_spike_phase = 1; // trigger QRS spike on the OLED ECG trace
    } else if (press_Timer != 0) {
        uint16_t dt = timer_elapsed(press_Timer);
        if (dt < 500) {
            total_tap_time += dt;
            total_tap_count++;
        }
        press_Timer = 0;
    }

    if (keycode == RESET_STATS) {
        if (record->event.pressed) reset_statistics();
        return false;
    }
    if (keycode == OS_TOGG) {
        if (record->event.pressed) {
            current_os_mode = (current_os_mode + 1) % 3;
        }
        return false;
    }

    if (record->event.pressed && get_highest_layer(layer_state) == _BASE) {
        uint16_t tkc = base_tap_keycode(keycode, record);
        if (tkc != KC_NO) {
            learner_feed_key(tkc, get_mods() | get_oneshot_mods());
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Cyberpunk 128x32 SH1106 OLED Renderer (zero float, zero printf)
// ---------------------------------------------------------------------------
#ifdef OLED_ENABLE

#define GLYPH_ARROW    ((char)0x7F)
#define GLYPH_APPLE_TL ((char)0x80)
#define GLYPH_APPLE_TR ((char)0x81)
#define GLYPH_APPLE_BL ((char)0x82)
#define GLYPH_APPLE_BR ((char)0x83)
#define GLYPH_LINUX_TL ((char)0x84)
#define GLYPH_LINUX_TR ((char)0x85)
#define GLYPH_LINUX_BL ((char)0x86)
#define GLYPH_LINUX_BR ((char)0x87)

// 16-entry quarter-sine lookup table (amplitude 0..9) for zero-float idle scope
static const int8_t PROGMEM sin32_lut[32] = {
    0,  2,  3,  5,  6,  8,  9,  9,  9,  9,  9,  8,  6,  5,  3,  2,
    0, -2, -3, -5, -6, -8, -9, -9, -9, -9, -9, -8, -6, -5, -3, -2
};

// 7-segment bitmask (bit0=top, 1=TR, 2=BR, 3=bot, 4=BL, 5=TL, 6=mid)
static const uint8_t PROGMEM seg7_lut[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static void draw_hline(uint8_t x0, uint8_t x1, uint8_t y) {
    for (uint8_t x = x0; x <= x1; x++) oled_write_pixel(x, y, true);
}

static void draw_vline(uint8_t x, uint8_t y0, uint8_t y1) {
    if (y0 > y1) { uint8_t t = y0; y0 = y1; y1 = t; }
    for (uint8_t y = y0; y <= y1; y++) oled_write_pixel(x, y, true);
}

static void draw_corners(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t len) {
    draw_hline(x0, x0 + len, y0); draw_vline(x0, y0, y0 + len);
    draw_hline(x1 - len, x1, y0); draw_vline(x1, y0, y0 + len);
    draw_hline(x0, x0 + len, y1); draw_vline(x0, y1 - len, y1);
    draw_hline(x1 - len, x1, y1); draw_vline(x1, y1 - len, y1);
}

static void draw_big_digit(uint8_t x, uint8_t y, uint8_t d) {
    if (d > 9) return;
    uint8_t s = pgm_read_byte(&seg7_lut[d]);
    if (s & 0x01) { draw_hline(x + 1, x + 6, y);      draw_hline(x + 1, x + 6, y + 1); }
    if (s & 0x02) { draw_vline(x + 6, y + 1, y + 6);  draw_vline(x + 7, y + 1, y + 6); }
    if (s & 0x04) { draw_vline(x + 6, y + 8, y + 13); draw_vline(x + 7, y + 8, y + 13); }
    if (s & 0x08) { draw_hline(x + 1, x + 6, y + 13); draw_hline(x + 1, x + 6, y + 14); }
    if (s & 0x10) { draw_vline(x,     y + 8, y + 13); draw_vline(x + 1, y + 8, y + 13); }
    if (s & 0x20) { draw_vline(x,     y + 1, y + 6);  draw_vline(x + 1, y + 1, y + 6); }
    if (s & 0x40) { draw_hline(x + 1, x + 6, y + 6);  draw_hline(x + 1, x + 6, y + 7); }
}

// Tiny unsigned integer formatter (replaces 1.2 KB snprintf/vfprintf)
static void fmt_u16(char *buf, uint16_t val, const char *suffix) {
    char tmp[5];
    uint8_t n = 0;
    if (val == 0) {
        tmp[n++] = '0';
    } else {
        while (val > 0 && n < 5) {
            tmp[n++] = '0' + (val % 10);
            val /= 10;
        }
    }
    uint8_t p = 0;
    while (n > 0) buf[p++] = tmp[--n];
    while (*suffix) buf[p++] = *suffix++;
    buf[p] = '\0';
}

static void draw_os_pod(void) {
    draw_corners(0, 0, 19, 21, 3);
    bool mac = (current_os_mode == OS_MODE_MAC);
    oled_set_cursor(0, 0);
    oled_write_char(mac ? GLYPH_APPLE_TL : GLYPH_LINUX_TL, false);
    oled_write_char(mac ? GLYPH_APPLE_TR : GLYPH_LINUX_TR, false);
    oled_set_cursor(0, 1);
    oled_write_char(mac ? GLYPH_APPLE_BL : GLYPH_LINUX_BL, false);
    oled_write_char(mac ? GLYPH_APPLE_BR : GLYPH_LINUX_BR, false);
    oled_set_cursor(0, 3);
    oled_write_P(current_os_mode == OS_MODE_MAC ? PSTR("MAC") :
                 (current_os_mode == OS_MODE_LINUX_FULL ? PSTR("LNX") : PSTR("L-T")), false);
    draw_vline(22, 0, 31);
}

static void tick_telemetry(void) {
    if (timer_elapsed(wpm_bucket_timer) >= 625) {
        wpm_bucket_timer = timer_read();
        wpm_bucket_idx   = (wpm_bucket_idx + 1) & 7;
        wpm_buckets[wpm_bucket_idx] = 0;

        uint16_t sum = 0;
        for (uint8_t i = 0; i < 8; i++) sum += wpm_buckets[i];
        // 5-second window: WPM = (keys / 5 chars) * (60 / 5 s) = keys * 12 / 5
        uint16_t w = (sum * 12) / 5;
        current_wpm = (w > 199) ? 199 : (uint8_t)w;
    }

    int8_t sample = 0;
    if (ecg_spike_phase > 0 && ecg_spike_phase <= 6) {
        sample = (int8_t)pgm_read_byte(&ecg_kernel[ecg_spike_phase - 1]);
        ecg_spike_phase++;
        if (ecg_spike_phase > 6) ecg_spike_phase = 0;
    }
    ecg_trace[ecg_head] = sample;
    ecg_head = (ecg_head + 1) & 63;
}

static void render_cyber_deck(void) {
    draw_os_pod();

    // Center pod: 2x 7-seg WPM + tap ms + scrolling ECG trace
    uint8_t w = current_wpm;
    if (w >= 100) {
        draw_big_digit(25, 1, 1);
        w -= 100;
    }
    draw_big_digit(28, 1, w / 10);
    draw_big_digit(38, 1, w % 10);

    oled_set_cursor(8, 0);
    oled_write_P(PSTR("WPM"), false);

    uint16_t avg_ms = (total_tap_count > 0) ? (uint16_t)(total_tap_time / total_tap_count) : 0;
    if (avg_ms > 999) avg_ms = 999;
    char ms_buf[8];
    fmt_u16(ms_buf, avg_ms, "ms");
    oled_set_cursor(8, 1);
    oled_write(ms_buf, false);

    uint8_t prev_y = 26;
    for (uint8_t i = 0; i < 64; i++) {
        int8_t dy = ecg_trace[(ecg_head + i) & 63];
        uint8_t y = (uint8_t)(26 + dy);
        draw_vline(26 + i, prev_y, y);
        prev_y = y;
    }

    // Right status stack
    draw_vline(93, 0, 31);
    oled_set_cursor(16, 0);
    oled_write_P(PSTR("BASE"), false);

    oled_set_cursor(16, 1);
    bool ac_on = autocorrect_is_enabled();
    bool cw_on = is_caps_word_on();
    oled_write_P(ac_on ? PSTR("AC ") : PSTR("-- "), false);
    oled_write_P(cw_on ? PSTR("CW")  : PSTR("--"),  false);
    if (ac_on) draw_hline(96, 106, 15);
    if (cw_on) draw_hline(114, 124, 15);

    uint8_t mods = get_mods() | get_oneshot_mods();
    oled_set_cursor(16, 3);
    char mbuf[6] = {'[', '-', '-', '-', ']', '\0'};
    uint8_t mp = 1;
    if (mods & MOD_MASK_GUI)   mbuf[mp++] = 'G';
    if (mods & MOD_MASK_CTRL && mp < 4) mbuf[mp++] = 'C';
    if (mods & MOD_MASK_ALT  && mp < 4) mbuf[mp++] = 'A';
    if (mods & MOD_MASK_SHIFT&& mp < 4) mbuf[mp++] = 'S';
    oled_write(mbuf, false);
}

static void render_intercept_card(void) {
    draw_corners(0, 0, 127, 31, 5);
    draw_hline(6, 121, 0);
    oled_set_cursor(1, 0);
    oled_write_P(PSTR("// AC INTERCEPT [OK]"), false);
    draw_hline(4, 123, 10);

    oled_set_cursor(1, 2);
    oled_write(intercept_typo, false);
    uint8_t tl = strlen(intercept_typo);
    if (tl > 9) tl = 9;
    draw_hline(6, 6 + tl * 6 - 2, 19); // strikethrough on typo

    oled_set_cursor(1 + tl, 2);
    oled_write_char(GLYPH_ARROW, false);
    oled_write(intercept_fix, false);
    uint8_t fl = strlen(intercept_fix);
    if (fl > 9) fl = 9;
    uint8_t fx0 = (2 + tl) * 6;
    if (fx0 + fl * 6 < 126) {
        draw_hline(fx0, fx0 + fl * 6 - 2, 24); // underline on fix
    }
}

static void render_layer_xray(uint8_t layer) {
    draw_os_pod();
    oled_set_cursor(4, 0);
    if (layer == _SYMBOLS) {
        oled_write_P(current_os_mode == OS_MODE_MAC ? PSTR("SYM LAYER") : PSTR("SYM   CTL=GUI"), false);
        draw_hline(24, 127, 8);
        oled_set_cursor(4, 2);
        oled_write_P(PSTR("!@#$%   ^&*()"), false);
        oled_set_cursor(4, 3);
        oled_write_P(PSTR("`=<>:   .^v_-"), false);
    } else {
        oled_write_P(PSTR("FN LAYER"), false);
        draw_hline(24, 127, 8);
        oled_set_cursor(4, 2);
        oled_write_P(PSTR("F1-12 EUR RST SC"), false);
        oled_set_cursor(4, 3);
        oled_write_P(PSTR("OS:Z  AC:; MEDIA"), false);
    }
}

static void render_learned_log(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("// LEARNED TYPOS"), false);
    char acbuf[8];
    fmt_u16(acbuf, ac_hit_count, "");
    oled_set_cursor(17, 0);
    oled_write_P(PSTR("AC:"), false);
    oled_write(acbuf, false);
    draw_hline(0, 127, 8);

    bool any = false;
    for (uint8_t i = 0; i < 3; i++) {
        if (learned_typos[i].count == 0) continue;
        any = true;
        oled_set_cursor(0, 1 + i);
        oled_write(learned_typos[i].typo, false);
        oled_write_char(GLYPH_ARROW, false);
        oled_write(learned_typos[i].fix, false);
        char cbuf[6];
        cbuf[0] = 'x';
        fmt_u16(cbuf + 1, learned_typos[i].count, "");
        oled_set_cursor(18, 1 + i);
        oled_write(cbuf, false);
    }
    if (!any) {
        oled_set_cursor(1, 2);
        oled_write_P(PSTR("No repeat typos yet"), false);
    }
}

static void render_idle_scope(void) {
    draw_corners(0, 0, 127, 31, 4);
    bool mac = (current_os_mode == OS_MODE_MAC);
    oled_set_cursor(1, 0);
    oled_write_char(mac ? GLYPH_APPLE_TL : GLYPH_LINUX_TL, false);
    oled_write_char(mac ? GLYPH_APPLE_TR : GLYPH_LINUX_TR, false);
    oled_set_cursor(1, 1);
    oled_write_char(mac ? GLYPH_APPLE_BL : GLYPH_LINUX_BL, false);
    oled_write_char(mac ? GLYPH_APPLE_BR : GLYPH_LINUX_BR, false);
    oled_set_cursor(16, 0);
    oled_write_P(PSTR("STBY"), false);

    uint8_t phase = (uint8_t)(timer_read() >> 6);
    uint8_t py    = 18;
    for (uint8_t x = 0; x < 128; x++) {
        int8_t s1 = (int8_t)pgm_read_byte(&sin32_lut[(x / 3 + phase) & 31]);
        int8_t s2 = (int8_t)pgm_read_byte(&sin32_lut[(x / 5 - phase) & 31]) / 2;
        uint8_t y1 = (uint8_t)(18 + s1);
        uint8_t y2 = (uint8_t)(18 + s2);
        if (x > 0) draw_vline(x, py, y1);
        if ((x & 1) == 0) oled_write_pixel(x, y2, true);
        py = y1;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (!is_keyboard_master()) return false;

    tick_telemetry();
    oled_clear();

    uint32_t idle_ms = timer_elapsed32(last_activity_timer);
    if (idle_ms > IDLE_TIMEOUT) {
        if (eeprom_dirty && !eeprom_flushed) {
            save_log_to_eeprom();
        }
        // Alternate every ~6 seconds between Learned Typos log and Dual-Trace Scope
        if (((idle_ms >> 12) & 1) == 0) {
            render_learned_log();
        } else {
            render_idle_scope();
        }
        return false;
    }

    if (intercept_active) {
        if (timer_elapsed(intercept_timer) < INTERCEPT_MS) {
            render_intercept_card();
            return false;
        }
        intercept_active = false;
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (layer != _BASE) {
        render_layer_xray(layer);
    } else {
        render_cyber_deck();
    }
    return false;
}

#endif // OLED_ENABLE
