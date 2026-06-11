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

// Reads the touch panel and maps raw XPT2046 coordinates to screen
// coordinates. Tweak the map() ranges below if touches feel off or
// inverted on your panel.
TouchPoint readTouch();

// Draws a rounded-rect button with a centered label.
void drawButton(const Rect &r, const char *label, uint16_t bg, uint16_t fg);
