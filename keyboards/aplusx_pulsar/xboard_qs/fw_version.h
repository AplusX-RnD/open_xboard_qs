/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Firmware version shown on the LEDs while XQ_FW_VERSION is held.
 * Shown as MAJOR.MINOR.PATCH, for example 1.2.3 is displayed as "1.23". */
#define FW_VERSION_MAJOR 1
#define FW_VERSION_MINOR 2
#define FW_VERSION_PATCH 3

/* The firmware flavour is derived from the build, so one source tree serves
 * the VIA, SignalRGB and Vial builds. */
typedef enum {
    FW_TYPE_VIA       = 0,
    FW_TYPE_SIGNALRGB = 1,
    FW_TYPE_VIAL      = 2,
} fw_type_t;

#if defined(VIAL_ENABLE)
#    define FW_TYPE FW_TYPE_VIAL
#elif defined(SIGNALRGB_SUPPORT_ENABLE)
#    define FW_TYPE FW_TYPE_SIGNALRGB
#else
#    define FW_TYPE FW_TYPE_VIA
#endif

void fw_version_set_display(bool enabled);
bool fw_version_display_enabled(void);
