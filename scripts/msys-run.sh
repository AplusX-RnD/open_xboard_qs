#!/usr/bin/env bash
# Launcher used by the .cmd wrappers: initialise the QMK MSYS environment
# (PATH for the mingw64 toolchain) without the interactive welcome banner
# and its update check, then run the given script with the remaining
# arguments. Start it with: bash --noprofile --norc msys-run.sh <script> ...
# shellcheck disable=SC1091
source /etc/profile >/dev/null 2>&1
# The qmk CLI shipped with QMK MSYS 1.9 warns about a deprecated milc call on
# every invocation; the warning is not ours to fix and only clutters the log.
export PYTHONWARNINGS="${PYTHONWARNINGS:-ignore::UserWarning}"
exec bash "$@"
