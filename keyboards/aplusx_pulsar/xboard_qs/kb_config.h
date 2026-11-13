/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Persistent keyboard settings, stored in the 32-bit QMK "user" EEPROM slot.
 * The layout is shared with earlier firmware versions and must not change,
 * so that a firmware update keeps the user's settings. */
typedef union {
    uint32_t raw;
    struct {
        bool     kvm_host : 1;     /* false = PC1, true = PC2 */
        uint8_t  status_led_level; /* 0 = never set, otherwise STATUS_LED_LEVEL_OFF..MAX */
        uint16_t quick_tap_mode;   /* quick_tap_mode_t */
    };
} kb_config_t;

extern kb_config_t kb_config;

void kb_config_load(void);
void kb_config_save(void);
