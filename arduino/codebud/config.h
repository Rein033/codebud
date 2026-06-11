#pragma once

// Display
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4

// Touch (XPT2046, shares SPI bus with display)
#define TOUCH_CS      21
#define TOUCH_IRQ_PIN 22

// Display resolution
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

// Analog input used by the "Sensor" app (e.g. LDR or potentiometer)
#define SENSOR_PIN 34
