#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _SYMBOLS,
    _FUNCTION,
};

enum tap_dances {
    TD_Q_GRV,
    TD_P_SQT,
    TD_A_ESC,
    TD_SEMI_QUOTE,
    TD_SLASH_NUBS,
};

static bool dance_a_shift_registered;
static bool dance_semi_shift_registered;

static void dance_a_esc_finished(tap_dance_state_t *state, void *user_data) {
    (void)user_data;
    if (state->count >= 2) {
        tap_code16(KC_ESC);
    } else if (state->pressed) {
        register_mods(MOD_LSFT);
        dance_a_shift_registered = true;
    } else {
        tap_code16(KC_A);
    }
}

static void dance_a_esc_reset(tap_dance_state_t *state, void *user_data) {
    (void)state;
    (void)user_data;
    if (dance_a_shift_registered) {
        unregister_mods(MOD_LSFT);
        dance_a_shift_registered = false;
    }
}

static void dance_semi_quote_finished(tap_dance_state_t *state, void *user_data) {
    (void)user_data;
    if (state->count >= 2) {
        tap_code16(KC_QUOT);
    } else if (state->pressed) {
        register_mods(MOD_RSFT);
        dance_semi_shift_registered = true;
    } else {
        tap_code16(KC_SCLN);
    }
}

static void dance_semi_quote_reset(tap_dance_state_t *state, void *user_data) {
    (void)state;
    (void)user_data;
    if (dance_semi_shift_registered) {
        unregister_mods(MOD_RSFT);
        dance_semi_shift_registered = false;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_Q_GRV]       = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_GRV),
    [TD_P_SQT]       = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_QUOT),
    [TD_A_ESC]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_a_esc_finished, dance_a_esc_reset),
    [TD_SEMI_QUOTE]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_semi_quote_finished, dance_semi_quote_reset),
    [TD_SLASH_NUBS]  = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_NUBS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
        TD(TD_Q_GRV), KC_W, KC_E, KC_R, KC_T,                         KC_Y, KC_U, KC_I, KC_O, TD(TD_P_SQT),
        TD(TD_A_ESC), LCTL_T(KC_S), LGUI_T(KC_D), LALT_T(KC_F), KC_G, KC_H, RALT_T(KC_J), RGUI_T(KC_K), RCTL_T(KC_L), TD(TD_SEMI_QUOTE),
        LT(_FUNCTION, KC_Z), KC_X, KC_C, LT(_FUNCTION, KC_V), KC_B,  KC_N, LT(_FUNCTION, KC_M), KC_COMM, KC_DOT, TD(TD_SLASH_NUBS),
        KC_BSPC, KC_LGUI, LT(_SYMBOLS, KC_ESC), LT(_SYMBOLS, KC_TAB), KC_ENT, KC_SPC
    ),

    [_SYMBOLS] = LAYOUT_split_3x5_3(
        KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        KC_1, KC_2, KC_3, KC_4, KC_5,                                 KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_GRV, KC_TILD, KC_LEFT, KC_RGHT, KC_EQL,                    KC_MINS, KC_UP, KC_DOWN, KC_DQUO, KC_QUOT,
        KC_DEL, KC_LCTL, KC_TRNS, MO(_FUNCTION), KC_ENT, KC_SPC
    ),

    // ZMK's Bluetooth and external-power controls have no equivalent on this wired ATmega32U4 build.
    [_FUNCTION] = LAYOUT_split_3x5_3(
        KC_PLUS, LSFT(KC_MINS), KC_NO, KC_NO, KC_NO,                  KC_CIRC, KC_AMPR, KC_DQUO, KC_LCBR, KC_RCBR,
        KC_EQL, KC_GRV, KC_F20, KC_F20, KC_NO,                       KC_PLUS, KC_MINS, KC_EQL, KC_NUBS, KC_PIPE,
        KC_TILD, KC_PIPE, KC_HOME, KC_END, KC_NO,                    KC_UNDS, KC_PGUP, KC_PGDN, KC_LBRC, KC_RBRC,
        KC_DEL, KC_LCTL, KC_END, KC_TRNS, KC_ENT, KC_RALT
    ),
};
