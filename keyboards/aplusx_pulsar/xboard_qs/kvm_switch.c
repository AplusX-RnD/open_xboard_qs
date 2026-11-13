/* Copyright 2024 Aplusx Inc.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "quantum.h"
#include "kvm_switch.h"
#include "kb_config.h"

/* Keep the hub device power off for this long after switching hosts, so the
 * downstream devices re-enumerate cleanly on the new host. */
#define KVM_SWITCH_SETTLE_MS 100

static kvm_host_t current_host = KVM_HOST_PC1;
static bool       switch_pending = false;
static uint16_t   switch_timer;

static void route_hub_to_host(kvm_host_t host) {
    gpio_write_pin_high(KVM_PIN_OUTPUT_EN); /* disconnect the current host */
    gpio_write_pin_low(KVM_PIN_POWER_EN);   /* power-cycle the hub devices */
    gpio_write_pin(KVM_PIN_HOST_SEL, host == KVM_HOST_PC2);
    gpio_write_pin_low(KVM_PIN_OUTPUT_EN); /* connect to the selected host */

    switch_timer   = timer_read();
    switch_pending = true;
    current_host   = host;
}

void kvm_switch_init_pins(void) {
    /* Hardware power-on defaults: OUTPUT_EN = 0, HOST_SEL = 0, POWER_EN = 1 */
    gpio_set_pin_output(KVM_PIN_OUTPUT_EN);
    gpio_set_pin_output(KVM_PIN_HOST_SEL);
    gpio_set_pin_output(KVM_PIN_POWER_EN);
}

void kvm_switch_restore(void) {
    route_hub_to_host(kb_config.kvm_host ? KVM_HOST_PC2 : KVM_HOST_PC1);
}

void kvm_switch_select(kvm_host_t host) {
    if (host == current_host) {
        return;
    }
    route_hub_to_host(host);
    kb_config.kvm_host = (host == KVM_HOST_PC2);
    kb_config_save();
}

kvm_host_t kvm_switch_current_host(void) {
    return current_host;
}

void kvm_switch_task(void) {
    if (switch_pending && timer_elapsed(switch_timer) > KVM_SWITCH_SETTLE_MS) {
        gpio_write_pin_high(KVM_PIN_POWER_EN);
        switch_pending = false;
    }
}
