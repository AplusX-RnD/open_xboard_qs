/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

/* The keymaps use layers 0/1 for Windows and 2/3 for macOS. The OS mode is
 * the persistent default layer, so it survives a power cycle. */

#define OS_MODE_WINDOWS_BASE_LAYER 0
#define OS_MODE_MACOS_BASE_LAYER   2

typedef enum {
    OS_MODE_WINDOWS,
    OS_MODE_MACOS,
} os_mode_t;

os_mode_t os_mode_current(void);
void      os_mode_set(os_mode_t mode);
void      os_mode_toggle(void);
