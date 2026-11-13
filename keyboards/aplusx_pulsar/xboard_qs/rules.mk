# Keyboard-level sources. Keymap-level rules.mk files select VIA / Vial.
SRC += kb_config.c \
       kvm_switch.c \
       os_mode.c \
       status_led.c \
       factory_reset.c \
       fw_version.c

# QMK links with -flto=auto, which makes GCC's lto-wrapper run the LTRANS
# stage through a sub-make. On Windows (QMK MSYS) that sub-make fails on the
# temp file paths and lto-wrapper falls back to running the job itself, after
# printing "Error 1 (ignored)" at link time. Later flags win, so force LTO
# into in-process serial mode (-flto=1; plain -flto would still pick up the
# make jobserver) with a single partition. The generated code is the same;
# only the link step is marginally slower.
EXTRAFLAGS += -flto=1 -flto-partition=one

# Quick Tap: SOCD cleaning for the WASD and arrow key pairs (quick_tap.c).
# Set QUICK_TAP_ENABLE = no in a keymap rules.mk to build without it.
# This is unrelated to QMK's QUICK_TAP_TERM tap-hold setting.
QUICK_TAP_ENABLE ?= yes
