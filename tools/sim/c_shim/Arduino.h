#pragma once

// Minimal C-compatible Arduino shim, used only when compiling LVGL's C
// sources (lv_hal_tick.c) for the native simulator build.

#include <stdint.h>
#include <time.h>

#define INPUT 0x01
#define HIGH 1
#define LOW 0

static inline uint32_t millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000u + ts.tv_nsec / 1000000u);
}
