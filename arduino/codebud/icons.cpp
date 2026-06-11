#include "icons.h"

void iconClock(int16_t x, int16_t y, int16_t size) {
  int16_t cx = x + size / 2;
  int16_t cy = y + size / 2;
  int16_t r = size / 2 - 2;
  tft.drawCircle(cx, cy, r, ILI9341_WHITE);
  tft.drawLine(cx, cy, cx, cy - r + 5, ILI9341_WHITE);
  tft.drawLine(cx, cy, cx + r - 6, cy, ILI9341_WHITE);
}

void iconPomodoro(int16_t x, int16_t y, int16_t size) {
  int16_t cx = x + size / 2;
  int16_t cy = y + size / 2 + 2;
  int16_t r = size / 2 - 4;
  tft.fillCircle(cx, cy, r, ILI9341_WHITE);
  tft.fillRect(cx - 2, y + 2, 4, 6, ILI9341_WHITE);
}

void iconPong(int16_t x, int16_t y, int16_t size) {
  tft.fillCircle(x + size / 2 - 4, y + size / 2 - 6, 4, ILI9341_WHITE);
  tft.fillRect(x + 6, y + size - 10, size - 12, 5, ILI9341_WHITE);
}

void iconSensor(int16_t x, int16_t y, int16_t size) {
  int16_t barW = 6, gap = 4;
  int16_t baseY = y + size - 4;
  tft.fillRect(x + 2, baseY - 10, barW, 10, ILI9341_WHITE);
  tft.fillRect(x + 2 + (barW + gap), baseY - 18, barW, 18, ILI9341_WHITE);
  tft.fillRect(x + 2 + 2 * (barW + gap), baseY - 26, barW, 26, ILI9341_WHITE);
}

void iconMessages(int16_t x, int16_t y, int16_t size) {
  int16_t w = size - 4, h = size - 10;
  int16_t ex = x + 2, ey = y + 6;
  tft.drawRect(ex, ey, w, h, ILI9341_WHITE);
  tft.drawLine(ex, ey, ex + w / 2, ey + h / 2, ILI9341_WHITE);
  tft.drawLine(ex + w, ey, ex + w / 2, ey + h / 2, ILI9341_WHITE);
}
