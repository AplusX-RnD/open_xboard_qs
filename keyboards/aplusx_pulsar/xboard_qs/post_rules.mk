# Evaluated after the keymap rules.mk, so a keymap can turn Quick Tap off.
ifeq ($(strip $(QUICK_TAP_ENABLE)), yes)
    SRC += quick_tap.c
    OPT_DEFS += -DQUICK_TAP_ENABLE
endif
