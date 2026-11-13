/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include "quantum.h"

/* Hold XQ_FACTORY_RESET for 3 s: all LEDs turn red, and 2 s later the EEPROM
 * is cleared and the keyboard reboots. Releasing before 3 s cancels. */

void factory_reset_process_record(keyrecord_t *record);
void factory_reset_task(void);

/* True during the red warning phase, before the reboot. */
bool factory_reset_is_pending(void);
