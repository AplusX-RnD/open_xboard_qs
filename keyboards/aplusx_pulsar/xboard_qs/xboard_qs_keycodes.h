/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include "quantum_keycodes.h"

/* Custom keycodes.
 *
 * The order is part of the public interface: the VIA and Vial JSON files refer
 * to these by index (CUSTOM(0) ... CUSTOM(20)). Never reorder or remove an
 * entry; append new keycodes at the end. Entries marked "reserved" keep their
 * slot but are not handled by the firmware. */
enum xboard_qs_keycodes {
    XQ_GUI_LOCK = QK_KB_0, /* 0  reserved; use GU_TOGG */
    XQ_WIN_MODE,           /* 1  switch to the Windows layers */
    XQ_MAC_MODE,           /* 2  switch to the macOS layers */
    XQ_OS_TOGGLE,          /* 3  toggle Windows <-> macOS */
    XQ_SPOTLIGHT,          /* 4  reserved */
    XQ_DICTATION,          /* 5  macOS dictation (consumer Voice Command) */
    XQ_DO_NOT_DISTURB,     /* 6  macOS do not disturb (system usage) */
    XQ_LOCK_SCREEN,        /* 7  macOS lock screen (consumer AL Lock) */
    XQ_LOPT,               /* 8  reserved */
    XQ_ROPT,               /* 9  reserved */
    XQ_LCMD,               /* 10 reserved */
    XQ_RCMD,               /* 11 reserved */
    XQ_TASK_VIEW,          /* 12 reserved; use XQ_WIN_TASK_VIEW */
    XQ_FILE_EXPLORER,      /* 13 reserved; use XQ_WIN_EXPLORER */
    XQ_PC1,                /* 14 KVM: connect to PC1 */
    XQ_PC2,                /* 15 KVM: connect to PC2 */
    XQ_PC_TOGGLE,          /* 16 reserved; KVM toggle is intentionally disabled */
    XQ_FACTORY_RESET,      /* 17 hold 3 s: clear EEPROM and reboot */
    XQ_STATUS_LED,         /* 18 cycle the status LED brightness (off, 1..5) */
    XQ_QUICK_TAP,          /* 19 cycle the Quick Tap mode (off, last key, neutral) */
    XQ_FW_VERSION,         /* 20 show the firmware version on the LEDs while held */
};

/* Shortcut aliases used by the keymaps. */
#define XQ_WIN_TASK_VIEW   LGUI(KC_TAB)
#define XQ_WIN_EXPLORER    LGUI(KC_E)
#define XQ_WIN_DICTATION   LGUI(KC_H)
#define XQ_WIN_LOCK        LGUI(KC_L)
#define XQ_MAC_SCREENSHOT  SCMD(KC_5)
