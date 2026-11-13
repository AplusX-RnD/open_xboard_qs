/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "kb_config.h"
#include "eeconfig.h"

kb_config_t kb_config;

void kb_config_load(void) {
    kb_config.raw = eeconfig_read_user();
}

void kb_config_save(void) {
    eeconfig_update_user(kb_config.raw);
}
