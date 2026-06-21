#!/bin/sh

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
BUILD_DIR=${MK4_BUILD_DIR:-"$PROJECT_ROOT/build/mk4_release_noboot"}
STATE_DIR=${MK4_EMULATOR_STATE_DIR:-"$PROJECT_ROOT/build/mk4-emulator"}
WEB_PORT=${MK4_EMULATOR_WEB_PORT:-8042}
NATIVE_SIMULATOR="$PROJECT_ROOT/.dependencies/mini404-native-0.9.10/qemu-system-buddy"
PINNED_SIMULATOR="$PROJECT_ROOT/.dependencies/mini404-0.9.10/qemu-system-buddy"

cd "$PROJECT_ROOT"

if [ ! -x "$PROJECT_ROOT/.venv/bin/python" ]; then
    echo "Preparing the firmware build environment..."
    python3 utils/build.py --preset mk4 --build-type debug --bootloader no --no-build
fi

if [ ! -f "$BUILD_DIR/firmware" ]; then
    echo "MK4 release firmware is missing; building it now..."
    "$PROJECT_ROOT/.venv/bin/python" utils/build.py \
        --preset mk4 \
        --build-type release \
        --bootloader no \
        --no-store-output
fi

if [ -x "$NATIVE_SIMULATOR" ]; then
    SIMULATOR="$NATIVE_SIMULATOR"
elif [ -x "$PINNED_SIMULATOR" ]; then
    SIMULATOR="$PINNED_SIMULATOR"
else
    echo "Mini404 is not installed. Run a firmware build once to bootstrap dependencies." >&2
    exit 1
fi

if [ "$(uname -s)" = "Darwin" ] && [ "$(uname -m)" = "arm64" ] && [ "$SIMULATOR" = "$PINNED_SIMULATOR" ]; then
    echo "The pinned Mini404 binary is Intel-only and cannot use ARM Homebrew libraries." >&2
    echo "Expected the native simulator at:" >&2
    echo "  $NATIVE_SIMULATOR" >&2
    echo "See CHANGELOG.md under 'MK4 emulator setup' for native build details." >&2
    exit 1
fi

echo "Launching the MK4 emulator..."
echo "  Firmware: $BUILD_DIR/firmware"
echo "  Simulator: $SIMULATOR"
echo "  State:     $STATE_DIR"
echo "  USB drive: $STATE_DIR/usbdir"
echo "  Display:   http://127.0.0.1:$WEB_PORT"
echo "Close the browser tab or press Ctrl-C here to stop it."

exec "$PROJECT_ROOT/.venv/bin/python" utils/run_mk4_emulator.py \
    --build-dir "$BUILD_DIR" \
    --simulator-path "$SIMULATOR" \
    --state-dir "$STATE_DIR" \
    --web-port "$WEB_PORT"
