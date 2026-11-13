/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * QMK hook entry points. Each feature lives in its own module:
 *   kvm_switch.c    PC1 / PC2 host switching through the on-board USB hub
 *   os_mode.c       Windows / macOS layer selection
 *   status_led.c    indicator LEDs, brightness, firmware version display
 *   factory_reset.c hold-to-reset
 *   quick_tap.c     SOCD cleaning for WASD / arrow pairs (optional)
 *   kb_config.c     persistent settings
 */
#include "xboard_qs.h"

void keyboard_pre_init_kb(void) {
    kvm_switch_init_pins();
    keyboard_pre_init_user();
}

void keyboard_post_init_kb(void) {
    kb_config_load();
    kvm_switch_restore();
    status_led_init();
#ifdef QUICK_TAP_ENABLE
    quick_tap_init();
#endif
    keyboard_post_init_user();
}

static bool process_custom_keycode(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;

    switch (keycode) {
        case XQ_FACTORY_RESET:
            factory_reset_process_record(record);
            return true;

        case XQ_DICTATION:
            host_consumer_send(pressed ? 0x0CF : 0); /* Voice Command */
            return false;
        case XQ_DO_NOT_DISTURB:
            host_system_send(pressed ? 0x9B : 0); /* System Do Not Disturb */
            return false;
        case XQ_LOCK_SCREEN:
            host_consumer_send(pressed ? 0x19E : 0); /* AL Terminal Lock / Screensaver */
            return false;

        case XQ_WIN_MODE:
            if (pressed) os_mode_set(OS_MODE_WINDOWS);
            return false;
        case XQ_MAC_MODE:
            if (pressed) os_mode_set(OS_MODE_MACOS);
            return false;
        case XQ_OS_TOGGLE:
            if (pressed) os_mode_toggle();
            return false;

        case XQ_PC1:
            if (pressed) kvm_switch_select(KVM_HOST_PC1);
            return false;
        case XQ_PC2:
            if (pressed) kvm_switch_select(KVM_HOST_PC2);
            return false;

        case XQ_STATUS_LED:
            if (pressed) status_led_cycle_level();
            return false;

#ifdef QUICK_TAP_ENABLE
        case XQ_QUICK_TAP:
            if (pressed) quick_tap_cycle_mode();
            return true;
#endif

        case XQ_FW_VERSION:
            fw_version_set_display(pressed);
            return true;

        default:
            return true;
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }
#ifdef QUICK_TAP_ENABLE
    if (!quick_tap_process_record(keycode, record)) {
        return false;
    }
#endif
    return process_custom_keycode(keycode, record);
}

void housekeeping_task_kb(void) {
    kvm_switch_task();
    factory_reset_task();
    status_led_task();
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    status_led_render_indicators();
    return true;
}

#ifdef DYNAMIC_MACRO_ENABLE
bool dynamic_macro_record_start_kb(int8_t direction) {
    status_led_set_macro_recording(true);
    return dynamic_macro_record_start_user(direction);
}

bool dynamic_macro_record_end_kb(int8_t direction) {
    status_led_set_macro_recording(false);
    return dynamic_macro_record_end_user(direction);
}
#endif
