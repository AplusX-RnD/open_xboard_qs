# open_xboard_qs

QMK firmware for the **Pulsar XBOARD QS** keyboard, with reproducible builds for
the VIA, Vial and SignalRGB variants.

The keyboard sources live once, in `keyboards/aplusx_pulsar/xboard_qs/`. The
QMK and Vial trees are git submodules pinned to known-good commits, the small
QMK core fix the hardware needs is kept as a patch, and a script links the
keyboard folder into each tree before building.

```
open_xboard_qs/
├── keyboards/aplusx_pulsar/xboard_qs/   keyboard definition and firmware modules
│   ├── keymaps/default|via|vial|srgb/
│   ├── xboard_qs.c            QMK hook entry points
│   ├── kvm_switch.c           PC1 / PC2 host switching (USB hub)
│   ├── os_mode.c              Windows / macOS layer sets
│   ├── status_led.c           indicator LEDs and firmware version display
│   ├── quick_tap.c            SOCD cleaning for WASD / arrow pairs
│   ├── factory_reset.c        hold-to-reset
│   ├── kb_config.c            persistent settings (EEPROM)
│   └── fw_version.h           version number shown on the LEDs
├── firmware/qmk_firmware/     submodule: qmk/qmk_firmware (VIA build)
├── firmware/vial-qmk/         submodule: vial-kb/vial-qmk (Vial build)
├── firmware/SRGBmods_QMK/     submodule: SRGBmods/QMK (SignalRGB build)
├── patches/<tree>/            patches applied to each firmware tree
├── scripts/setup.sh|.cmd      init submodules, apply patches, link the keyboard
├── scripts/build.sh|.cmd      build one or more variants into build/
├── via/                       VIA JSON definition
└── .github/workflows/         CI build of every variant
```

## Requirements

* Git
* The [QMK CLI](https://docs.qmk.fm/newbs_getting_started) with the ARM
  toolchain. On Windows install [QMK MSYS](https://msys.qmk.fm/); the `.cmd`
  wrappers run the scripts inside it (set `QMK_MSYS_DIR` if it is not in
  `C:\Tools\QMK_MSYS`).

## Quick start

```sh
git clone https://github.com/AplusX-RnD/open_xboard_qs.git
cd open_xboard_qs
scripts/setup.sh            # Windows: scripts\setup.cmd
scripts/build.sh all        # Windows: scripts\build.cmd all   (via + vial + srgb)
```

The firmware files land in `build/` as
`xboard_qs_v<version>_<variant>_<date>.bin`. Flash them with QMK Toolbox
(STM32 DFU) or `qmk flash`.

`setup.sh` is safe to run again. It skips patches that are already applied and
links that already exist. Pass `--copy` to copy the keyboard folder instead of
linking it (then re-run it after editing the sources).

## Variants

| Variant   | Tree                     | Keymap    | Notes                                   |
|-----------|--------------------------|-----------|-----------------------------------------|
| `via`     | `firmware/qmk_firmware`  | `via`     | Release firmware                        |
| `vial`    | `firmware/vial-qmk`      | `vial`    | Vial; fewer RGB effects, see keymap config.h |
| `default` | `firmware/qmk_firmware`  | `default` | Same keymap without VIA                 |
| `srgb`    | `firmware/SRGBmods_QMK`  | `srgb`    | VIA + SignalRGB protocol (SRGBmods fork) |

The firmware type shown by the version display is derived from the build, so
`fw_version.h` needs no per-variant edits.

## Releasing a new version

1. Bump `FW_VERSION_*` in `keyboards/aplusx_pulsar/xboard_qs/fw_version.h`.
2. Build all variants and test them on hardware.
3. Commit, tag `vX.YZ` and push. The CI workflow builds every variant and
   attaches the `.bin` files to the run.
4. Update `via/*.json` if custom keycodes changed. Their order is fixed; see
   `xboard_qs_keycodes.h`.

## Updating QMK or Vial

```sh
git -C firmware/qmk_firmware checkout -- .        # drop the applied patch first
git -C firmware/qmk_firmware fetch --tags
git -C firmware/qmk_firmware checkout <new tag>
scripts/setup.sh              # re-applies the patch; fails loudly if it no longer fits
scripts/build.sh all
git add firmware/qmk_firmware && git commit -m "Update QMK to <new tag>"
```

Things that have needed attention on past upgrades: legacy keycode names
(`RGB_*` became `RM_*`, `KC_MS_*`/`KC_BTN*` became `MS_*` in QMK 0.34) and RAM
on the 16 KB STM32F072 (Vial table sizes are capped in
`keymaps/vial/config.h`).

If a patch stops applying, edit the file in the submodule by hand, run
`git -C firmware/qmk_firmware diff > patches/qmk_firmware/0001-...patch`, and
test with `setup.sh` again. The same applies to `firmware/vial-qmk` and
`firmware/SRGBmods_QMK`.

`.gitmodules` sets `ignore = dirty` for the submodules, so the applied patch and
the keyboard link do not show up as changes in `git status`.

## Patches

`patches/<tree>/0001-usb-suspend-wakeup-behind-kvm-hub.patch` changes the
ChibiOS USB suspend handling: the host sits behind the on-board hub / KVM switch
and does not reliably advertise remote wakeup, so the keyboard wakes the host on
any key press and restarts the USB driver. Without it the keyboard can stay
asleep after switching PC1 → PC2 → PC1.

## License

GPL-2.0-or-later, like QMK. See `LICENSE`.
