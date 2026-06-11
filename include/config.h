#pragma once

// Touch controller (XPT2046) interrupt pin.
// TOUCH_CS is configured via the TFT_eSPI build flags in platformio.ini.
#define TOUCH_IRQ_PIN 22

// Display resolution, must match TFT_WIDTH / TFT_HEIGHT build flags.
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

// Analog input used by the "Sensor" demo app (e.g. LDR or potentiometer).
#define SENSOR_PIN 34
