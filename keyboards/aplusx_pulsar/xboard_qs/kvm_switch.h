/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

/* The keyboard contains a USB hub whose upstream port can be routed to one of
 * two host PCs. These functions drive the hub control pins (KVM_PIN_*). */

typedef enum {
    KVM_HOST_PC1 = 0,
    KVM_HOST_PC2 = 1,
} kvm_host_t;

/* Configure the control pins as outputs. Call from keyboard_pre_init_kb(). */
void kvm_switch_init_pins(void);

/* Route the hub to the host saved in kb_config. Call after kb_config_load(). */
void kvm_switch_restore(void);

/* Route the hub to the given host and remember it. No-op if already selected. */
void kvm_switch_select(kvm_host_t host);

kvm_host_t kvm_switch_current_host(void);

/* Call from housekeeping_task_kb(); finishes the switch sequence. */
void kvm_switch_task(void);
