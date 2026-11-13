/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include QMK_KEYBOARD_H

/* Layers 0/1 are the Windows set and 2/3 the macOS set; see os_mode.h. */
enum layer {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT(
                                                                                                                   XQ_PC1,  XQ_PC2,  KC_MUTE,
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  XQ_OS_TOGGLE, KC_PSCR, KC_SCRL, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,      KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,      KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,               KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    KC_RALT, MO(WIN_FN), KC_APP, KC_RCTL,        KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT(
                                                                                                                   XQ_PC1,  XQ_PC2,  RM_NEXT,
        XQ_FACTORY_RESET, KC_BRID, KC_BRIU, XQ_WIN_TASK_VIEW, KC_WSCH, XQ_WIN_DICTATION, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, XQ_OS_TOGGLE, KC_CALC, KC_WHOM, XQ_WIN_LOCK,
        DM_RSTP, DM_REC1, DM_REC2, DM_PLY1, DM_PLY2, _______, _______, _______, _______, _______, _______, NK_OFF,  NK_ON,   RM_TOGG,      RM_SATU, RM_HUEU, RM_NEXT,
        _______, MS_BTN1, MS_UP,   MS_BTN2, MS_WHLU, _______, _______, _______, _______, _______, _______, _______, _______, XQ_FW_VERSION, RM_SATD, RM_HUED, RM_PREV,
        _______, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD, _______, _______, _______, _______, _______, _______, _______,          _______,
        XQ_QUICK_TAP,     MS_BTN4, MS_BTN3, MS_BTN5, _______, XQ_STATUS_LED, _______, _______, _______, _______, _______,    _______,               RM_VALU,
        _______, GU_TOGG, _______,                   _______,                   _______, _______, _______, _______,          RM_SPDD, RM_VALD, RM_SPDU),

    [MAC_BASE] = LAYOUT(
                                                                                                                   XQ_PC1,  XQ_PC2,  KC_MUTE,
        KC_ESC,  KC_BRID, KC_BRIU, KC_MCTL, KC_WSCH, XQ_DICTATION, XQ_DO_NOT_DISTURB, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, XQ_OS_TOGGLE, XQ_MAC_SCREENSHOT, KC_LPAD, XQ_LOCK_SCREEN,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,      KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,      KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,               KC_UP,
        KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC,                    KC_RGUI, MO(MAC_FN), KC_RALT, KC_RCTL,       KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT(
                                                                                                                   XQ_PC1,  XQ_PC2,  RM_NEXT,
        XQ_FACTORY_RESET, KC_F1, KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  XQ_OS_TOGGLE, KC_F13,  KC_F14,  KC_F15,
        DM_RSTP, DM_REC1, DM_REC2, DM_PLY1, DM_PLY2, _______, _______, _______, _______, _______, _______, NK_OFF,  NK_ON,   RM_TOGG,      RM_SATU, RM_HUEU, RM_NEXT,
        _______, MS_BTN1, MS_UP,   MS_BTN2, MS_WHLU, _______, _______, _______, _______, _______, _______, _______, _______, XQ_FW_VERSION, RM_SATD, RM_HUED, RM_PREV,
        _______, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD, _______, _______, _______, _______, _______, _______, _______,          _______,
        XQ_QUICK_TAP,     MS_BTN4, MS_BTN3, MS_BTN5, _______, XQ_STATUS_LED, _______, _______, _______, _______, _______,    _______,               RM_VALU,
        _______, _______, _______,                   _______,                   _______, _______, _______, _______,          RM_SPDD, RM_VALD, RM_SPDU),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(RM_VALD, RM_VALU)},
};
#endif
// clang-format on
