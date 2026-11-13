/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "quick_tap.h"
#include "kb_config.h"

#define QUICK_TAP_KEYS_PER_PAIR 2

typedef struct {
    uint16_t keys[QUICK_TAP_KEYS_PER_PAIR];
    uint8_t  leds[QUICK_TAP_KEYS_PER_PAIR]; /* RGB matrix indexes of the two keys */
    bool     held[QUICK_TAP_KEYS_PER_PAIR];
    uint16_t active_key; /* key currently reported to the host, or KC_NO */
} quick_tap_pair_t;

static quick_tap_pair_t pairs[] = {
    {{KC_A, KC_D}, {73, 75}, {false, false}, KC_NO},
    {{KC_W, KC_S}, {69, 74}, {false, false}, KC_NO},
    {{KC_LEFT, KC_RIGHT}, {85, 109}, {false, false}, KC_NO},
    {{KC_UP, KC_DOWN}, {108, 110}, {false, false}, KC_NO},
};
#define QUICK_TAP_PAIR_COUNT (sizeof(pairs) / sizeof(pairs[0]))

static quick_tap_mode_t mode = QUICK_TAP_OFF;

void quick_tap_init(void) {
    mode = (quick_tap_mode_t)kb_config.quick_tap_mode;
}

quick_tap_mode_t quick_tap_mode(void) {
    return mode;
}

void quick_tap_cycle_mode(void) {
    switch (mode) {
        case QUICK_TAP_OFF:
            mode = QUICK_TAP_LAST_KEY;
            break;
        case QUICK_TAP_LAST_KEY:
            mode = QUICK_TAP_NEUTRAL;
            break;
        default:
            mode = QUICK_TAP_OFF;
            break;
    }
    kb_config.quick_tap_mode = mode;
    kb_config_save();
}

static bool find_pair(uint16_t keycode, uint8_t *pair_index, uint8_t *key_index) {
    for (uint8_t p = 0; p < QUICK_TAP_PAIR_COUNT; p++) {
        for (uint8_t k = 0; k < QUICK_TAP_KEYS_PER_PAIR; k++) {
            if (pairs[p].keys[k] == keycode) {
                *pair_index = p;
                *key_index  = k;
                return true;
            }
        }
    }
    return false;
}

static void send_key(uint16_t keycode) {
    if (keycode != KC_NO) {
        register_code(keycode);
    }
}

bool quick_tap_process_record(uint16_t keycode, keyrecord_t *record) {
    uint8_t pair_index, key_index;

    if (mode == QUICK_TAP_OFF || !find_pair(keycode, &pair_index, &key_index)) {
        return true;
    }

    quick_tap_pair_t *pair        = &pairs[pair_index];
    uint8_t           other_index = key_index ^ 1;
    uint16_t          other_key   = pair->keys[other_index];
    bool              other_held  = pair->held[other_index];
    bool              neutral     = (mode == QUICK_TAP_NEUTRAL);

    if (record->event.pressed) {
        uint16_t key_to_send = keycode;
        if (other_held) {
            unregister_code(other_key);
            if (neutral) {
                key_to_send = KC_NO; /* both held: report nothing */
            }
        }
        send_key(key_to_send);
        pair->held[key_index] = true;
        pair->active_key      = key_to_send;
    } else {
        unregister_code(keycode);
        uint16_t released_key = neutral ? KC_NO : keycode;
        if (other_held && pair->active_key == released_key) {
            send_key(other_key); /* fall back to the key that is still held */
            pair->active_key = other_key;
        }
        if (!other_held) {
            pair->active_key = released_key;
        }
        pair->held[key_index] = false;
    }
    return false;
}

void quick_tap_render_indicators(uint8_t brightness) {
    if (brightness == 0 || mode == QUICK_TAP_OFF) {
        return;
    }
    uint8_t green = (mode == QUICK_TAP_NEUTRAL) ? brightness : 0; /* red or yellow */

    for (uint8_t p = 0; p < QUICK_TAP_PAIR_COUNT; p++) {
        for (uint8_t k = 0; k < QUICK_TAP_KEYS_PER_PAIR; k++) {
            rgb_matrix_set_color(pairs[p].leds[k], brightness, green, 0);
        }
    }
}
