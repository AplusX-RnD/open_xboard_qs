# Shared helpers for scripts/*.sh. Sourced, not executed.

# On Windows the qmk CLI only exists inside QMK MSYS. If this script was
# started from another shell (Git Bash, VS Code terminal, plain MSYS2) and
# `qmk` is not on PATH, re-run it under QMK MSYS's bash and exit with its
# status. Set QMK_MSYS_DIR when QMK MSYS is installed somewhere else.
relaunch_in_qmk_msys() {
    local script="$1"
    shift

    if command -v qmk >/dev/null 2>&1 || [ "${XQ_IN_QMK_MSYS:-}" = 1 ]; then
        return 0
    fi
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) ;;
        *) return 0 ;; # Linux / macOS: qmk must simply be installed
    esac

    local dir candidate msys_bash=""
    for dir in "${QMK_MSYS_DIR:-}" "C:/Tools/QMK_MSYS" "C:/QMK_MSYS"; do
        [ -n "$dir" ] || continue
        candidate="$(cygpath -u "$dir" 2>/dev/null || echo "$dir")/usr/bin/bash.exe"
        if [ -x "$candidate" ]; then
            msys_bash="$candidate"
            break
        fi
    done
    if [ -z "$msys_bash" ]; then
        echo "qmk is not on PATH and QMK MSYS was not found." >&2
        echo "Install it from https://msys.qmk.fm or set QMK_MSYS_DIR, then run again." >&2
        exit 127
    fi

    # Go through the matching .cmd wrapper (scripts/build.cmd, setup.cmd) via
    # cmd.exe rather than exec'ing the other MSYS runtime directly: two
    # different msys-2.0.dll runtimes do not hand the environment over cleanly
    # (QMK MSYS's Python then cannot find the home directory), while a native
    # intermediary receives a plain Windows environment block. The wrapper
    # also keeps all the quoting on the Windows side.
    local wrapper
    wrapper="$(cd "$(dirname "$script")" && pwd)/$(basename "${script%.sh}").cmd"
    if [ ! -f "$wrapper" ]; then
        echo "missing wrapper $wrapper for $script" >&2
        exit 127
    fi

    echo "(re-running inside QMK MSYS: $msys_bash)"
    QMK_MSYS_DIR="$(cygpath -w "$(dirname "$(dirname "$(dirname "$msys_bash")")")")" \
    XQ_IN_QMK_MSYS=1 MSYS2_ARG_CONV_EXCL='*' \
        exec cmd /c "$(cygpath -w "$wrapper")" "$@" </dev/null
}
