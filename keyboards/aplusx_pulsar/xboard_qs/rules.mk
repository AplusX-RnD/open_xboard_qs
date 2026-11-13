# Keyboard-level sources. Keymap-level rules.mk files select VIA / Vial.
SRC += kb_config.c \
       kvm_switch.c \
       os_mode.c \
       status_led.c \
       factory_reset.c \
       fw_version.c

# Quick Tap: SOCD cleaning for the WASD and arrow key pairs (quick_tap.c).
# Set QUICK_TAP_ENABLE = no in a keymap rules.mk to build without it.
# This is unrelated to QMK's QUICK_TAP_TERM tap-hold setting.
QUICK_TAP_ENABLE ?= yes
