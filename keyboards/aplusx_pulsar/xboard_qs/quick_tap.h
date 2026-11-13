/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include "quantum.h"

/* Quick Tap: SOCD (simultaneous opposing cardinal direction) cleaning for the
 * A/D, W/S, Left/Right and Up/Down key pairs.
 *
 *   QUICK_TAP_LAST_KEY  the most recently pressed key of a pair wins; releasing
 *                       it re-presses the other key if it is still held.
 *   QUICK_TAP_NEUTRAL   holding both keys of a pair outputs nothing; releasing
 *                       one re-presses the other.
 *
 * The mode is stored in kb_config. Enabled with QUICK_TAP_ENABLE in rules.mk. */

typedef enum {
    QUICK_TAP_OFF      = 0,
    QUICK_TAP_LAST_KEY = 1,
    QUICK_TAP_NEUTRAL  = 4, /* value kept for EEPROM compatibility */
} quick_tap_mode_t;

/* Load the saved mode. Call after kb_config_load(). */
void quick_tap_init(void);

quick_tap_mode_t quick_tap_mode(void);

/* OFF -> LAST_KEY -> NEUTRAL -> OFF, saved to EEPROM. */
void quick_tap_cycle_mode(void);

/* Call from process_record_kb(). Returns false when the key was handled here. */
bool quick_tap_process_record(uint16_t keycode, keyrecord_t *record);

/* Colour the pair keys: red for LAST_KEY, yellow for NEUTRAL. */
void quick_tap_render_indicators(uint8_t brightness);
