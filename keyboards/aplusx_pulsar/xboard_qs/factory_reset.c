/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "factory_reset.h"

#define FACTORY_RESET_HOLD_MS 3000
#define FACTORY_RESET_WARN_MS 2000

static bool     key_held       = false;
static bool     reset_pending  = false;
static uint16_t phase_timer;

void factory_reset_process_record(keyrecord_t *record) {
    if (record->event.pressed) {
        phase_timer = timer_read();
        key_held    = true;
    } else {
        key_held = false;
    }
}

void factory_reset_task(void) {
    if (key_held && timer_elapsed(phase_timer) > FACTORY_RESET_HOLD_MS) {
        key_held      = false;
        reset_pending = true;
        phase_timer   = timer_read();
    }
    if (reset_pending && timer_elapsed(phase_timer) > FACTORY_RESET_WARN_MS) {
        eeconfig_init();
        soft_reset_keyboard();
    }
}

bool factory_reset_is_pending(void) {
    return reset_pending;
}
