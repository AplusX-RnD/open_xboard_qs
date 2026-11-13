#!/usr/bin/env bash
# Build one or more firmware variants and collect the .bin files in build/.
#
# Usage: scripts/build.sh [--clean] <via|vial|default|srgb|all> ...
#
# Each variant is built in its own firmware tree:
#   via      firmware/qmk_firmware   keymap via      (VIA)
#   vial     firmware/vial-qmk       keymap vial     (Vial)
#   default  firmware/qmk_firmware   keymap default  (no VIA)
#   srgb     firmware/SRGBmods_QMK   keymap via      (SignalRGB, phase 2)
#
# Requires the qmk CLI (QMK MSYS on Windows: use scripts/build.cmd).

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
KEYBOARD="aplusx_pulsar/xboard_qs"
OUT_DIR="$ROOT/build"
CLEAN=""
VARIANTS=()

for arg in "$@"; do
    case "$arg" in
        --clean) CLEAN="-c" ;;
        all) VARIANTS+=(via vial) ;;
        via|vial|default|srgb) VARIANTS+=("$arg") ;;
        *) echo "unknown variant: $arg" >&2; exit 2 ;;
    esac
done
if [ ${#VARIANTS[@]} -eq 0 ]; then
    echo "usage: $0 [--clean] <via|vial|default|srgb|all> ..." >&2
    exit 2
fi

fw_version() {
    local h="$ROOT/keyboards/$KEYBOARD/fw_version.h"
    local major minor patch
    major=$(sed -n 's/^#define FW_VERSION_MAJOR *\([0-9]*\).*/\1/p' "$h")
    minor=$(sed -n 's/^#define FW_VERSION_MINOR *\([0-9]*\).*/\1/p' "$h")
    patch=$(sed -n 's/^#define FW_VERSION_PATCH *\([0-9]*\).*/\1/p' "$h")
    echo "v${major}.${minor}${patch}"
}

build_variant() {
    local variant="$1" tree keymap
    case "$variant" in
        via)     tree=qmk_firmware; keymap=via ;;
        vial)    tree=vial-qmk;     keymap=vial ;;
        default) tree=qmk_firmware; keymap=default ;;
        srgb)    tree=SRGBmods_QMK; keymap=via ;;
    esac
    local tree_dir="$ROOT/firmware/$tree"

    if [ ! -e "$tree_dir/keyboards/aplusx_pulsar/xboard_qs/keyboard.json" ]; then
        echo "firmware/$tree is not set up; run scripts/setup.sh $tree" >&2
        return 1
    fi

    echo "== building $variant  (firmware/$tree, keymap $keymap)"
    (cd "$tree_dir" && QMK_HOME="$tree_dir" qmk compile $CLEAN -kb "$KEYBOARD" -km "$keymap")

    local bin="$tree_dir/.build/aplusx_pulsar_xboard_qs_${keymap}.bin"
    local out="$OUT_DIR/xboard_qs_$(fw_version)_${variant}_$(date +%Y%m%d).bin"
    mkdir -p "$OUT_DIR"
    cp "$bin" "$out"
    printf '   -> %s (%s bytes)\n' "${out#$ROOT/}" "$(wc -c < "$out" | tr -d ' ')"
}

for variant in "${VARIANTS[@]}"; do
    build_variant "$variant"
done
