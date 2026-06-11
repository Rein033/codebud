#!/usr/bin/env bash
# Builds and runs a native LVGL simulator of the launcher, producing a BMP
# screenshot of the home screen and of each app (tools/sim/out_*.bmp).
set -euo pipefail
cd "$(dirname "$0")/../.."

LVGL_SRC=".pio/libdeps/esp32dev/lvgl/src"
if [ ! -d "$LVGL_SRC" ]; then
    echo "LVGL sources not found at $LVGL_SRC - run 'pio run' once to fetch dependencies." >&2
    exit 1
fi

mkdir -p tools/sim/objc
gcc -std=gnu11 -O1 -c -Iinclude -Itools/sim/c_shim -I.pio/libdeps/esp32dev/lvgl \
    $(find "$LVGL_SRC" -name '*.c')
mv ./*.o tools/sim/objc/

g++ -std=gnu++17 -O1 -Iinclude -Itools/sim -Isrc -I.pio/libdeps/esp32dev/lvgl \
    tools/sim/main_sim.cpp src/launcher.cpp src/apps/*.cpp tools/sim/objc/*.o \
    -o tools/sim/sim -lm

./tools/sim/sim
echo "Wrote tools/sim/out_*.bmp"
