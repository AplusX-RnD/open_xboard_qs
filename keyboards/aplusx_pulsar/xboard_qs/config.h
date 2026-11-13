/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

/* Drive wait_us() from a hardware timer; keeps the encoder and RGB animation smooth. */
#define WAIT_US_TIMER GPTD3

/* USB hub / KVM switch control pins (see kvm_switch.c). */
#define KVM_PIN_POWER_EN  C1 /* hub device power on/off */
#define KVM_PIN_OUTPUT_EN C2 /* hub host connection on/off */
#define KVM_PIN_HOST_SEL  C0 /* hub host selection: low = PC1, high = PC2 */

/* WS2812 RGB matrix over SPI. The data pin is set in keyboard.json. */
#define WS2812_SPI_DRIVER        SPID2
#define WS2812_SPI_MOSI_PAL_MODE 0
#define WS2812_SPI_SCK_PIN       B13
#define WS2812_SPI_SCK_PAL_MODE  5
#define WS2812_SPI_DIVISOR       16
#define WS2812_SPI_USE_CIRCULAR_BUFFER

#define DYNAMIC_MACRO_DELAY 10
