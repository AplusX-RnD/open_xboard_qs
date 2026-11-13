#!/usr/bin/env bash
# Prepare the firmware trees for building:
#   1. check out the pinned QMK / Vial submodules (plus the nested libraries the
#      STM32 build needs),
#   2. apply the patches in patches/<tree>/,
#   3. make keyboards/aplusx_pulsar visible inside each tree (junction on
#      Windows, symlink elsewhere, or a copy with --copy).
#
# Safe to run again: already-applied patches and existing links are skipped.
#
# Usage: scripts/setup.sh [--copy] [tree ...]
#        (default trees: qmk_firmware vial-qmk SRGBmods_QMK)

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
# Setup itself only needs git, but running it inside QMK MSYS keeps the
# submodule checkouts consistent with the git that the build uses.
# shellcheck source=scripts/common.sh
. "$ROOT/scripts/common.sh"
relaunch_in_qmk_msys "${BASH_SOURCE[0]}" "$@"

KEYBOARD_DIR="$ROOT/keyboards/aplusx_pulsar"
# Libraries the STM32 build needs; lufa supplies the USB HID class headers.
NESTED_SUBMODULES="lib/chibios lib/chibios-contrib lib/printf lib/lufa"
LINK_MODE=link

TREES=()
for arg in "$@"; do
    case "$arg" in
        --copy) LINK_MODE=copy ;;
        *) TREES+=("$arg") ;;
    esac
done
if [ ${#TREES[@]} -eq 0 ]; then
    TREES=(qmk_firmware vial-qmk SRGBmods_QMK)
fi

is_windows() {
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) return 0 ;;
        *) return 1 ;;
    esac
}

apply_patches() {
    local tree_dir="$1" patch_dir="$2" patch
    [ -d "$patch_dir" ] || return 0
    for patch in "$patch_dir"/*.patch; do
        [ -e "$patch" ] || continue
        if git -C "$tree_dir" apply --reverse --check "$patch" >/dev/null 2>&1; then
            echo "  patch already applied: $(basename "$patch")"
        elif git -C "$tree_dir" apply --check "$patch" >/dev/null 2>&1; then
            git -C "$tree_dir" apply "$patch"
            echo "  patch applied: $(basename "$patch")"
        else
            echo "  ERROR: patch does not apply: $patch" >&2
            return 1
        fi
    done
}

link_keyboard() {
    local tree_dir="$1" target="$tree_dir/keyboards/aplusx_pulsar"

    if [ -e "$target/xboard_qs/keyboard.json" ]; then
        echo "  keyboard already linked"
        return 0
    fi
    if [ -e "$target" ]; then
        echo "  ERROR: $target exists but is not our keyboard" >&2
        return 1
    fi

    if [ "$LINK_MODE" = copy ]; then
        cp -r "$KEYBOARD_DIR" "$target"
        echo "  keyboard copied (re-run setup after editing sources)"
    elif is_windows; then
        cmd //c mklink //J "$(cygpath -w "$target")" "$(cygpath -w "$KEYBOARD_DIR")" >/dev/null
        echo "  keyboard junction created"
    else
        ln -s "$KEYBOARD_DIR" "$target"
        echo "  keyboard symlink created"
    fi
}

exclude_from_git() {
    # Keep the link and build outputs out of the submodule's `git status`.
    local tree_dir="$1" exclude_file
    exclude_file="$(git -C "$tree_dir" rev-parse --git-path info/exclude)"
    case "$exclude_file" in /*|?:*) ;; *) exclude_file="$tree_dir/$exclude_file" ;; esac
    for entry in "/keyboards/aplusx_pulsar/" "/*.bin" "/*.hex" "/*.elf" "/*.uf2"; do
        grep -qxF "$entry" "$exclude_file" 2>/dev/null || echo "$entry" >> "$exclude_file"
    done
}

init_tree() {
    # Check out the submodule only when it is missing, so a tree that was moved
    # to a new QMK tag by hand is left alone (commit the new pointer afterwards).
    local tree="$1" tree_dir="$ROOT/firmware/$tree"
    if [ ! -e "$tree_dir/.git" ]; then
        git -c core.autocrlf=false submodule update --init "firmware/$tree"
    fi
    # QMK and ChibiOS must be checked out byte-for-byte (LF). Pin that in each
    # repository so it does not depend on which git / global config is used.
    git -C "$tree_dir" config core.autocrlf false
    git -C "$tree_dir" config core.eol lf
    # shellcheck disable=SC2086
    git -c core.autocrlf=false -c core.eol=lf -C "$tree_dir" submodule update --init $NESTED_SUBMODULES
    for nested in $NESTED_SUBMODULES; do
        git -C "$tree_dir/$nested" config core.autocrlf false
        git -C "$tree_dir/$nested" config core.eol lf
        if [ -n "$(git -C "$tree_dir/$nested" status --porcelain)" ]; then
            echo "  normalising line endings in $nested"
            git -C "$tree_dir/$nested" checkout -q -- .
        fi
    done
}

cd "$ROOT"
git config core.autocrlf false
for tree in "${TREES[@]}"; do
    tree_dir="$ROOT/firmware/$tree"
    echo "== $tree"
    init_tree "$tree"
    apply_patches "$tree_dir" "$ROOT/patches/$tree"
    link_keyboard "$tree_dir"
    exclude_from_git "$tree_dir"
done
echo "setup complete"
