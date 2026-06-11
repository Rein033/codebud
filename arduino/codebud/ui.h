#pragma once

#include "display.h"
#include "config.h"

struct Rect {
  int16_t x, y, w, h;

  bool contains(int16_t px, int16_t py) const {
    return px >= x && px < x + w && py >= y && py < y + h;
  }
};

struct TouchPoint {
  bool pressed;
  int16_t x, y;
};

// Function pointer type for the small icons drawn on app tiles.
typedef void (*IconDrawFn)(int16_t x, int16_t y, int16_t size);

// Packs 8-bit RGB into the 16-bit 565 format used by the display.
constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

// Reads the touch panel and maps raw XPT2046 coordinates to screen
// coordinates. Tweak the map() ranges below if touches feel off or
// inverted on your panel.
TouchPoint readTouch();

// Draws a rounded-rect button with a centered label.
void drawButton(const Rect &r, const char *label, uint16_t bg, uint16_t fg);

// Draws a colored app tile with an icon and a label underneath.
void drawTile(const Rect &r, uint16_t color, IconDrawFn drawIcon, const char *label);
