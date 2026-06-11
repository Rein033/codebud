#pragma once

// Minimal Arduino API shim so the launcher/apps can be built and demoed
// natively (e.g. for screenshots), without an ESP32 toolchain.

#include <chrono>
#include <cmath>
#include <cstdint>

#define INPUT 0x01
#define HIGH 1
#define LOW 0

inline uint32_t millis() {
    using namespace std::chrono;
    static const auto start = steady_clock::now();
    return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now() - start).count());
}

inline void pinMode(int, int) {}

inline int analogRead(int) {
    // Fake a slowly oscillating sensor reading (0-4095) for the demo.
    double t = millis() / 1000.0;
    return static_cast<int>(2048 + 2000 * std::sin(t));
}
