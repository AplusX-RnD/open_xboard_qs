/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "quantum.h"
#include "os_mode.h"

os_mode_t os_mode_current(void) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    return (layer >= OS_MODE_MACOS_BASE_LAYER) ? OS_MODE_MACOS : OS_MODE_WINDOWS;
}

void os_mode_set(os_mode_t mode) {
    uint8_t base_layer = (mode == OS_MODE_MACOS) ? OS_MODE_MACOS_BASE_LAYER : OS_MODE_WINDOWS_BASE_LAYER;
    set_single_persistent_default_layer(base_layer);
}

void os_mode_toggle(void) {
    os_mode_set(os_mode_current() == OS_MODE_WINDOWS ? OS_MODE_MACOS : OS_MODE_WINDOWS);
}
