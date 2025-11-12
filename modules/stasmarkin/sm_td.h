/* Copyright 2025 Stanislav Markin (https://github.com/stasmarkin) *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Version: 0.5.3
 * Date: 2025-09-08
 */
#ifndef SM_TD_H
#define SM_TD_H

#include "quantum.h"

// SMTD Global Configuration
#ifndef SMTD_GLOBAL_SIMULTANEOUS_PRESSES_DELAY_MS
#define SMTD_GLOBAL_SIMULTANEOUS_PRESSES_DELAY_MS 0
#endif

#ifndef SMTD_GLOBAL_TAP_TERM
#define SMTD_GLOBAL_TAP_TERM TAPPING_TERM
#endif

#ifndef SMTD_GLOBAL_SEQUENCE_TERM
#define SMTD_GLOBAL_SEQUENCE_TERM TAPPING_TERM / 2
#endif

#ifndef SMTD_GLOBAL_RELEASE_TERM
#define SMTD_GLOBAL_RELEASE_TERM TAPPING_TERM / 4
#endif

#ifndef SMTD_GLOBAL_AGGREGATE_TAPS
#define SMTD_GLOBAL_AGGREGATE_TAPS false
#endif

// Base Definitions
typedef enum {
    SMTD_ACTION_TOUCH,
    SMTD_ACTION_TAP,
    SMTD_ACTION_HOLD,
    SMTD_ACTION_RELEASE,
} smtd_action;

typedef enum {
    SMTD_RESOLUTION_UNCERTAIN,
    SMTD_RESOLUTION_UNHANDLED,
    SMTD_RESOLUTION_DETERMINED,
} smtd_resolution;

typedef enum {
    SMTD_TIMEOUT_TAP,
    SMTD_TIMEOUT_SEQUENCE,
    SMTD_TIMEOUT_RELEASE,
} smtd_timeout;

typedef enum {
    SMTD_FEATURE_AGGREGATE_TAPS,
} smtd_feature;

// Public Functions
bool process_smtd(uint16_t keycode, keyrecord_t *record);
smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count);
__attribute__((weak)) uint32_t get_smtd_timeout(uint16_t keycode, smtd_timeout timeout);
__attribute__((weak)) bool smtd_feature_enabled(uint16_t keycode, smtd_feature feature);

// Customization Macros
#ifdef CAPS_WORD_ENABLE
#define SMTD_TAP_16(use_cl, key) tap_code16(use_cl && is_caps_word_on() ? LSFT(key) : key)
#define SMTD_REGISTER_16(use_cl, key) register_code16(use_cl && is_caps_word_on() ? LSFT(key) : key)
#define SMTD_UNREGISTER_16(use_cl, key) unregister_code16(use_cl && is_caps_word_on() ? LSFT(key) : key)
#else
#define SMTD_TAP_16(use_cl, key) tap_code16(key)
#define SMTD_REGISTER_16(use_cl, key) register_code16(key)
#define SMTD_UNREGISTER_16(use_cl, key) unregister_code16(key)
#endif

#ifndef NOTHING
#define NOTHING
#endif
#ifndef OVERLOAD6
#define OVERLOAD6(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#endif
#ifndef OVERLOAD5
#define OVERLOAD5(_1, _2, _3, _4, _5, NAME, ...) NAME
#endif
#ifndef OVERLOAD4
#define OVERLOAD4(_1, _2, _3, _4, NAME, ...) NAME
#endif
#ifndef OVERLOAD3
#define OVERLOAD3(_1, _2, _3, NAME, ...) NAME
#endif
#ifndef OVERLOAD2
#define OVERLOAD2(_1, _2, NAME, ...) NAME
#endif
#ifndef EXEC
#define EXEC(code) do { code } while(0)
#endif

#define SMTD_LIMIT(limit, if_under_limit, otherwise) \
    if (tap_count < limit) { if_under_limit; } else { otherwise; }

#define SMTD_DANCE(macro_key, touch_action, tap_action, hold_action, release_action)    \
    case macro_key: {                                                                   \
        switch (action) {                                                               \
            case SMTD_ACTION_TOUCH: touch_action; return SMTD_RESOLUTION_UNCERTAIN;     \
            case SMTD_ACTION_TAP: tap_action; return SMTD_RESOLUTION_DETERMINED;        \
            case SMTD_ACTION_HOLD: hold_action; return SMTD_RESOLUTION_DETERMINED;      \
            case SMTD_ACTION_RELEASE: release_action; return SMTD_RESOLUTION_DETERMINED;\
        }                                                                               \
        break;                                                                          \
    }

#define SMTD_MT(...) OVERLOAD4(__VA_ARGS__, SMTD_MT4, SMTD_MT3, SMTD_MT2)(__VA_ARGS__)
#define SMTD_MT2(key, mod) SMTD_MT3_ON_MKEY(key, key, mod)
#define SMTD_MT3(key, mod, threshold) SMTD_MT4_ON_MKEY(key, key, mod, threshold)
#define SMTD_MT4(key, mod, threshold, use_cl) SMTD_MT5_ON_MKEY(key, key, mod, threshold, use_cl)
#define SMTD_MT_ON_MKEY(...) OVERLOAD5(__VA_ARGS__, SMTD_MT5_ON_MKEY, SMTD_MT4_ON_MKEY, SMTD_MT3_ON_MKEY)(__VA_ARGS__)
#define SMTD_MT3_ON_MKEY(...) SMTD_MT4_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_MT4_ON_MKEY(...) SMTD_MT5_ON_MKEY(__VA_ARGS__, true)
#define SMTD_MT5_ON_MKEY(macro_key, tap_key, mod, threshold, use_cl) \
    SMTD_DANCE(macro_key,                                    \
        NOTHING,                                             \
        SMTD_TAP_16(use_cl, tap_key),                        \
        SMTD_LIMIT(threshold,                                \
            register_mods(MOD_BIT(mod));                     \
            send_keyboard_report();                          \
            SMTD_REGISTER_16(use_cl, tap_key)),              \
        SMTD_LIMIT(threshold,                                \
            unregister_mods(MOD_BIT(mod));                   \
            send_keyboard_report();                          \
            SMTD_UNREGISTER_16(use_cl, tap_key));            \
            send_keyboard_report()                           \
    )

#define SMTD_MTE(...) OVERLOAD4(__VA_ARGS__, SMTD_MTE4, SMTD_MTE3, SMTD_MTE2)(__VA_ARGS__)
#define SMTD_MTE2(key, mod_key) SMTD_MTE3_ON_MKEY(key, key, mod_key)
#define SMTD_MTE3(key, mod_key, threshold) SMTD_MTE4_ON_MKEY(key, key, mod_key, threshold)
#define SMTD_MTE4(key, mod_key, threshold, use_cl) SMTD_MTE5_ON_MKEY(key, key, mod_key, threshold, use_cl)
#define SMTD_MTE_ON_MKEY(...) OVERLOAD5(__VA_ARGS__, SMTD_MTE5_ON_MKEY, SMTD_MTE4_ON_MKEY, SMTD_MTE3_ON_MKEY)(__VA_ARGS__)
#define SMTD_MTE3_ON_MKEY(...) SMTD_MTE4_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_MTE4_ON_MKEY(...) SMTD_MTE5_ON_MKEY(__VA_ARGS__, true)
#define SMTD_MTE5_ON_MKEY(macro_key, tap_key, mod_key, threshold, use_cl) \
    SMTD_MBTE5_ON_MKEY(macro_key, tap_key, MOD_BIT(mod_key), threshold, use_cl)
#define SMTD_MBTE5_ON_MKEY(macro_key, tap_key, mods, threshold, use_cl) \
    SMTD_DANCE(macro_key,                                    \
        EXEC(                                                \
            register_mods(mods);                             \
            send_keyboard_report();                          \
        ),                                                   \
        EXEC(                                                \
            unregister_mods(mods);                           \
            SMTD_TAP_16(use_cl, tap_key);                    \
        ),                                                   \
        SMTD_LIMIT(threshold,                                \
            NOTHING,                                         \
            EXEC(                                            \
                unregister_mods(mods);                       \
                send_keyboard_report();                      \
                SMTD_REGISTER_16(use_cl, tap_key);           \
            )                                                \
        ),                                                   \
        SMTD_LIMIT(threshold,                                \
            EXEC(                                            \
                unregister_mods(mods);                       \
                send_keyboard_report();                      \
            ),                                               \
            SMTD_UNREGISTER_16(use_cl, tap_key))              \
    )

#define SMTD_LT(...) OVERLOAD4(__VA_ARGS__, SMTD_LT4, SMTD_LT3, SMTD_LT2)(__VA_ARGS__)
#define SMTD_LT2(key, layer) SMTD_LT3_ON_MKEY(key, key, layer)
#define SMTD_LT3(key, layer, threshold) SMTD_LT4_ON_MKEY(key, key, layer, threshold)
#define SMTD_LT4(key, layer, threshold, use_cl) SMTD_LT5_ON_MKEY(key, key, layer, threshold, use_cl)
#define SMTD_LT_ON_MKEY(...) OVERLOAD5(__VA_ARGS__, SMTD_LT5_ON_MKEY, SMTD_LT4_ON_MKEY, SMTD_LT3_ON_MKEY)(__VA_ARGS__)
#define SMTD_LT3_ON_MKEY(...) SMTD_LT4_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_LT4_ON_MKEY(...) SMTD_LT5_ON_MKEY(__VA_ARGS__, true)
#define SMTD_LT5_ON_MKEY(macro_key, tap_key, layer, threshold, use_cl)\
    SMTD_DANCE(macro_key,                                     \
        NOTHING,                                              \
        SMTD_TAP_16(use_cl, tap_key),                         \
        SMTD_LIMIT(threshold,                                 \
            LAYER_PUSH(layer),                                \
            SMTD_REGISTER_16(use_cl, tap_key)),               \
        SMTD_LIMIT(threshold,                                 \
            LAYER_RESTORE(),                                  \
            SMTD_UNREGISTER_16(use_cl, tap_key));             \
    )

#define SMTD_TD(...) OVERLOAD4(__VA_ARGS__, SMTD_TD4, SMTD_TD3, SMTD_TD2)(__VA_ARGS__)
#define SMTD_TD2(key, tap_key) SMTD_TD3_ON_MKEY(key, key, tap_key)
#define SMTD_TD3(key, tap_key, threshold) SMTD_TD4_ON_MKEY(key, key, tap_key, threshold)
#define SMTD_TD4(key, tap_key, threshold, use_cl) SMTD_TD5_ON_MKEY(key, key, tap_key, threshold, use_cl)
#define SMTD_TD_ON_MKEY(...) OVERLOAD5(__VA_ARGS__, SMTD_TD5_ON_MKEY, SMTD_TD4_ON_MKEY, SMTD_TD3_ON_MKEY)(__VA_ARGS__)
#define SMTD_TD3_ON_MKEY(...) SMTD_TD4_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_TD4_ON_MKEY(...) SMTD_TD5_ON_MKEY(__VA_ARGS__, true)
#define SMTD_TD5_ON_MKEY(macro_key, tap_key, hold_key, threshold, use_cl)\
    SMTD_DANCE(macro_key,                                        \
        NOTHING,                                                 \
        SMTD_TAP_16(use_cl, tap_key),                            \
        SMTD_LIMIT(threshold,                                    \
            SMTD_TAP_16(use_cl, hold_key),                       \
            SMTD_TAP_16(use_cl, tap_key)),                       \
        SMTD_LIMIT(threshold,                                    \
            SMTD_UNREGISTER_16(use_cl, hold_key),                \
            SMTD_UNREGISTER_16(use_cl, tap_key))                 \
    )

#define SMTD_TK(...) OVERLOAD4(__VA_ARGS__, SMTD_TK4, SMTD_TK3, SMTD_TK2)(__VA_ARGS__)
#define SMTD_TK2(key, tap_key) SMTD_TK3_ON_MKEY(key, key, tap_key)
#define SMTD_TK3(key, tap_key, threshold) SMTD_TK3_ON_MKEY(key, key, tap_key, threshold)
#define SMTD_TK4(key, tap_key, threshold, use_cl) SMTD_TK4_ON_MKEY(key, key, tap_key, threshold, use_cl)
#define SMTD_TK_ON_MKEY(...) OVERLOAD4(__VA_ARGS__, SMTD_TK4_ON_MKEY, SMTD_TK3_ON_MKEY, SMTD_TK2_ON_MKEY)(__VA_ARGS__)
#define SMTD_TK2_ON_MKEY(...) SMTD_TK3_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_TK3_ON_MKEY(...) SMTD_TK4_ON_MKEY(__VA_ARGS__, true)
#define SMTD_TK4_ON_MKEY(macro_key, tap_key, threshold, use_cl) \
    SMTD_DANCE(macro_key,                              \
        SMTD_LIMIT(threshold,                          \
            NOTHING,                                   \
            SMTD_TAP_16(use_cl, tap_key)),             \
        NOTHING,                                       \
        NOTHING,                                       \
        NOTHING                                        \
    )

#define SMTD_TTO(...) OVERLOAD4(__VA_ARGS__, SMTD_TTO4, SMTD_TTO3, SMTD_TTO2)(__VA_ARGS__)
#define SMTD_TTO2(key, layer) SMTD_TTO3_ON_MKEY(key, key, layer)
#define SMTD_TTO3(key, layer, threshold) SMTD_TTO3_ON_MKEY(key, key, layer)
#define SMTD_TTO4(key, layer, threshold, use_cl) SMTD_TTO4_ON_MKEY(key, key, layer, threshold, use_cl)
#define SMTD_TTO_ON_MKEY(...) OVERLOAD4(__VA_ARGS__, SMTD_TTO4_ON_MKEY, SMTD_TTO3_ON_MKEY, SMTD_TTO2_ON_MKEY)(__VA_ARGS__)
#define SMTD_TTO2_ON_MKEY(...) SMTD_TTO3_ON_MKEY(__VA_ARGS__, 1)
#define SMTD_TTO3_ON_MKEY(...) SMTD_TTO4_ON_MKEY(__VA_ARGS__, true)
#define SMTD_TTO4_ON_MKEY(macro_key, layer, threshold, use_cl) \
    SMTD_DANCE(macro_key,                              \
        SMTD_LIMIT(threshold,                          \
            NOTHING,                                   \
            layer_move(layer)),                        \
        NOTHING,                                       \
        NOTHING,                                       \
        NOTHING                                        \
    )

// Layer Utils
#define LAYER_PUSH(layer) \
    layer_on(layer)

#define LAYER_RESTORE() \
    layer_off(get_highest_layer(layer_state))

#endif // SM_TD_H