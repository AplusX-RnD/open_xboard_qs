# Pulsar XBOARD QS

![Pulsar XBOARD QS](https://i.imgur.com/wpTinU4.jpeg)

An 80% keyboard with three special keys, a rotary encoder, a built-in USB hub
and a two-host (PC1 / PC2) switch.

* Keyboard Maintainer: Erik Jung ([AplusX-RnD](https://github.com/AplusX-RnD))
* Hardware Supported: Pulsar XBOARD QS (STM32F072)
* Hardware Availability: [Pulsar](https://www.pulsar.gg/collections/xboard-series/products/xboard-qs)

## Building

This keyboard is normally built from the
[open_xboard_qs](https://github.com/AplusX-RnD/open_xboard_qs) repository, which
pins the QMK / Vial versions and applies the required QMK patch:

    scripts/build.sh via      # VIA firmware   (firmware/qmk_firmware)
    scripts/build.sh vial     # Vial firmware  (firmware/vial-qmk)

Inside a plain QMK tree the usual commands work too:

    qmk compile -kb aplusx_pulsar/xboard_qs -km via
    qmk compile -kb aplusx_pulsar/xboard_qs -km default

## Bootloader

Enter the bootloader in one of three ways:

* **Bootmagic reset**: unplug the cable, hold the `U` key, plug the cable back in.
* **Physical reset button**: unplug the cable, hold the button on the back of
  the PCB, plug the cable back in.
* **Keycode in layout**: press the key mapped to `QK_BOOT` if you added one.

## Custom keys

| Key              | Function                                                        |
|------------------|-----------------------------------------------------------------|
| `XQ_PC1` / `XQ_PC2` | Route the USB hub to host PC1 / PC2 (remembered across power cycles) |
| `XQ_OS_TOGGLE`   | Switch between the Windows (layers 0/1) and macOS (layers 2/3) sets |
| `XQ_STATUS_LED`  | Cycle the indicator brightness: off, then five levels           |
| `XQ_QUICK_TAP`   | Cycle Quick Tap (SOCD cleaning) for A/D, W/S, arrows: off, last key, neutral |
| `XQ_FACTORY_RESET` | Hold 3 s, LEDs turn red, 2 s later EEPROM is cleared and the board reboots |
| `XQ_FW_VERSION`  | While held, show the firmware version on the F-row, Q-row and Z-row |

The custom keycode order is fixed because the VIA / Vial JSON files refer to
them by index. See `xboard_qs_keycodes.h`.
