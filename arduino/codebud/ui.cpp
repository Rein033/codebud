#include "ui.h"

TouchPoint readTouch() {
  TouchPoint t = {false, 0, 0};
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    // Raw touch values are display/wiring specific - tweak these if
    // touch feels off or inverted.
    t.x = constrain(map(p.x, 200, 3700, 0, SCREEN_WIDTH), 0, SCREEN_WIDTH - 1);
    t.y = constrain(map(p.y, 240, 3800, 0, SCREEN_HEIGHT), 0, SCREEN_HEIGHT - 1);
    t.pressed = true;
  }
  return t;
}

void drawButton(const Rect &r, const char *label, uint16_t bg, uint16_t fg) {
  tft.fillRoundRect(r.x, r.y, r.w, r.h, 6, bg);
  tft.drawRoundRect(r.x, r.y, r.w, r.h, 6, fg);

  tft.setTextSize(2);
  tft.setTextColor(fg);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor(r.x + (r.w - (int16_t)w) / 2, r.y + (r.h - (int16_t)h) / 2);
  tft.print(label);
}
