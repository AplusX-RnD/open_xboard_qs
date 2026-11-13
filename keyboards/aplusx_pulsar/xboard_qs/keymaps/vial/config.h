/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define VIAL_KEYBOARD_UID {0x49, 0x93, 0x3B, 0x6F, 0x87, 0x00, 0x72, 0xC7}

/* Unlock with Esc + Enter */
#define VIAL_UNLOCK_COMBO_ROWS {0, 3}
#define VIAL_UNLOCK_COMBO_COLS {0, 14}

#define DYNAMIC_KEYMAP_LAYER_COUNT 4
#define DYNAMIC_KEYMAP_MACRO_COUNT 8
#define DYNAMIC_MACRO_SIZE 64

/* The STM32F072 has 16 KB of RAM and the Vial build uses almost all of it.
 * Tap dance and combo keep the 32 slots the v1.23 firmware shipped with; key
 * override is halved and the newer Alt Repeat Key table capped at 8 so that
 * about 500 bytes of heap remain (v1.23 shipped with 264). Check the heap
 * headroom (__heap_base__ .. __heap_end__ in the ELF) after changing these. */
#define VIAL_TAP_DANCE_ENTRIES 32
#define VIAL_COMBO_ENTRIES 32
#define VIAL_KEY_OVERRIDE_ENTRIES 16
#define VIAL_ALT_REPEAT_KEY_ENTRIES 8

/* The shipped Vial firmware (v1.23) leaves out these RGB effects. Remove an
 * #undef to get the effect back. */
#undef ENABLE_RGB_MATRIX_DUAL_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#undef ENABLE_RGB_MATRIX_HUE_BREATHING
#undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#undef ENABLE_RGB_MATRIX_HUE_WAVE
#undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_MULTISPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
