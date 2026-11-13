/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "fw_version.h"

static bool display_enabled = false;

void fw_version_set_display(bool enabled) {
    display_enabled = enabled;
}

bool fw_version_display_enabled(void) {
    return display_enabled;
}
