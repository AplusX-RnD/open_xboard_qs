/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Status indicators drawn on top of the RGB matrix: lock keys, OS mode,
 * selected PC, macro recording, factory reset warning, Quick Tap mode and
 * the firmware version display. Their brightness is a user setting with
 * six levels (off plus five steps) stored in kb_config. */

enum status_led_level {
    STATUS_LED_LEVEL_OFF = 1,
    STATUS_LED_LEVEL_MAX = 6,
};

/* Load the saved level. Call after kb_config_load(). */
void status_led_init(void);

/* Step to the next level, wrapping from MAX back to OFF, and save it. */
void status_led_cycle_level(void);

/* Current indicator brightness, 0 (off) .. 250. */
uint8_t status_led_brightness(void);

/* Macro recording indicator: blinks while recording. */
void status_led_set_macro_recording(bool recording);

/* Call from housekeeping_task_kb(). */
void status_led_task(void);

/* Call from rgb_matrix_indicators_kb(). Draws nothing while the firmware
 * version display is active. */
void status_led_render_indicators(void);

/* Call from rgb_matrix_indicators_advanced_kb(). While the firmware version
 * display is active, overwrites every LED in [led_min, led_max): white for
 * the version digits, off for the rest. Returns false when it drew. */
bool status_led_render_fw_version(uint8_t led_min, uint8_t led_max);

/* Set one LED to white at the given brightness; does nothing when it is 0. */
void status_led_set_white(uint8_t led_index, uint8_t brightness);
