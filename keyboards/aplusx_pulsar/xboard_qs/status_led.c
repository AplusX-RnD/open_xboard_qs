/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "quantum.h"
#include "status_led.h"
#include "kb_config.h"
#include "kvm_switch.h"
#include "os_mode.h"
#include "factory_reset.h"
#include "fw_version.h"
#ifdef QUICK_TAP_ENABLE
#    include "quick_tap.h"
#endif

/* RGB matrix LED indexes used as indicators (see the layout in keyboard.json). */
enum {
    LED_SCROLL_LOCK = 22,  /* Scroll Lock key */
    LED_OS_MODE     = 24,  /* "QUICK" key right of F12 */
    LED_MACRO_REC   = 38,  /* ` key */
    LED_CAPS_LOCK   = 72,  /* Caps Lock key */
    LED_QUICK_TAP   = 97,  /* left Shift key */
    LED_WIN_LOCK    = 99,  /* left GUI key */
    LED_PC1         = 101, /* indicator LED left of the space bar */
    LED_PC2         = 103, /* indicator LED right of the space bar */
};

/* LEDs used by the firmware version display: digits count from the left. */
static const uint8_t fw_major_leds[10] = {36, 35, 34, 32, 31, 30, 29, 28, 27, 26}; /* F1 .. F10 */
static const uint8_t fw_minor_leds[10] = {70, 69, 68, 67, 66, 65, 64, 63, 62, 61}; /* Q .. P */
static const uint8_t fw_patch_leds[10] = {96, 95, 94, 93, 92, 91, 90, 89, 88, 87}; /* Z .. / */
static const uint8_t fw_type_leds[3]   = {98, 99, 100};                            /* LCtrl (VIA), LGUI (SignalRGB), LAlt (Vial) */

#define STATUS_LED_BRIGHTNESS_STEP 50 /* level 1..6 -> 0..250 */
#define MACRO_REC_BLINK_MS         500
#define MACRO_REC_MIN_BRIGHTNESS   50 /* keep the recording blink visible when indicators are off */

static uint8_t  level;
static bool     macro_recording = false;
static bool     macro_led_on    = false;
static uint16_t macro_blink_timer;

void status_led_init(void) {
    level = kb_config.status_led_level;
    if (level == 0) {
        level = STATUS_LED_LEVEL_MAX;
    }
}

void status_led_cycle_level(void) {
    if (++level > STATUS_LED_LEVEL_MAX) {
        level = STATUS_LED_LEVEL_OFF;
    }
    kb_config.status_led_level = level;
    kb_config_save();
}

uint8_t status_led_brightness(void) {
    return (level - STATUS_LED_LEVEL_OFF) * STATUS_LED_BRIGHTNESS_STEP;
}

void status_led_set_macro_recording(bool recording) {
    macro_recording   = recording;
    macro_led_on      = recording;
    macro_blink_timer = timer_read();
}

void status_led_task(void) {
    if (macro_recording && timer_elapsed(macro_blink_timer) > MACRO_REC_BLINK_MS) {
        macro_led_on      = !macro_led_on;
        macro_blink_timer = timer_read();
    }
}

void status_led_set_white(uint8_t led_index, uint8_t brightness) {
    if (brightness) {
        rgb_matrix_set_color(led_index, brightness, brightness, brightness);
    }
}

static void render_os_mode_and_host(uint8_t brightness) {
    if (brightness == 0) {
        return;
    }
    uint8_t host_led = (kvm_switch_current_host() == KVM_HOST_PC2) ? LED_PC2 : LED_PC1;

    if (os_mode_current() == OS_MODE_WINDOWS) {
        rgb_matrix_set_color(LED_OS_MODE, 0, 0, brightness); /* blue */
        rgb_matrix_set_color(host_led, 0, 0, brightness);
    } else {
        rgb_matrix_set_color(LED_OS_MODE, brightness, 0, 0); /* red */
        rgb_matrix_set_color(host_led, brightness, 0, 0);
    }
}

static void render_fw_version(uint8_t brightness) {
    rgb_matrix_set_color_all(0, 0, 0);

    for (uint8_t i = 0; i < FW_VERSION_MAJOR; i++) {
        rgb_matrix_set_color(fw_major_leds[i], brightness, brightness, brightness);
    }
    for (uint8_t i = 0; i < FW_VERSION_MINOR; i++) {
        rgb_matrix_set_color(fw_minor_leds[i], brightness, brightness, brightness);
    }
    for (uint8_t i = 0; i < FW_VERSION_PATCH; i++) {
        rgb_matrix_set_color(fw_patch_leds[i], brightness, brightness, brightness);
    }
    rgb_matrix_set_color(fw_type_leds[FW_TYPE], brightness, brightness, brightness);
}

void status_led_render_indicators(void) {
    uint8_t brightness = status_led_brightness();
    led_t   host_leds  = host_keyboard_led_state();

    if (host_leds.caps_lock) {
        status_led_set_white(LED_CAPS_LOCK, brightness);
    }
    if (host_leds.scroll_lock) {
        status_led_set_white(LED_SCROLL_LOCK, brightness);
    }

    if (factory_reset_is_pending()) {
        rgb_matrix_set_color_all(0xFF, 0, 0);
    }

    if (keymap_config.no_gui) {
        if (os_mode_current() == OS_MODE_MACOS) {
            keymap_config.no_gui = false; /* GUI lock is a Windows-only feature */
        } else {
            status_led_set_white(LED_WIN_LOCK, brightness);
        }
    }

    if (macro_led_on) {
        uint8_t v = brightness ? brightness : MACRO_REC_MIN_BRIGHTNESS;
        rgb_matrix_set_color(LED_MACRO_REC, v, v, v);
    }

    render_os_mode_and_host(brightness);

#ifdef QUICK_TAP_ENABLE
    if (quick_tap_mode() != QUICK_TAP_OFF) {
        status_led_set_white(LED_QUICK_TAP, brightness);
        quick_tap_render_indicators(brightness);
    }
#endif

    if (fw_version_display_enabled()) {
        render_fw_version(brightness);
    }
}
