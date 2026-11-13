@echo off
rem Windows wrapper: runs scripts/setup.sh inside QMK MSYS.
rem Set QMK_MSYS_DIR if QMK MSYS is not installed in C:\Tools\QMK_MSYS.
setlocal
if "%QMK_MSYS_DIR%"=="" set "QMK_MSYS_DIR=C:\Tools\QMK_MSYS"
if not exist "%QMK_MSYS_DIR%\usr\bin\bash.exe" (
    echo QMK MSYS not found at %QMK_MSYS_DIR%. Install it from https://msys.qmk.fm or set QMK_MSYS_DIR.
    exit /b 1
)
set MSYSTEM=MINGW64
set CHERE_INVOKING=1
rem msys-run.sh loads the MSYS profile quietly (no welcome banner) and execs the script.
"%QMK_MSYS_DIR%\usr\bin\bash.exe" --noprofile --norc "%~dp0msys-run.sh" "%~dp0setup.sh" %*
