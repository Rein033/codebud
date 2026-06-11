#pragma once

#include "display.h"

// Each icon is drawn in white inside a size x size square starting at
// (x, y), meant to sit on top of a colored tile.
void iconClock(int16_t x, int16_t y, int16_t size);
void iconPomodoro(int16_t x, int16_t y, int16_t size);
void iconPong(int16_t x, int16_t y, int16_t size);
void iconSensor(int16_t x, int16_t y, int16_t size);
void iconMessages(int16_t x, int16_t y, int16_t size);
void iconStopwatch(int16_t x, int16_t y, int16_t size);
void iconDice(int16_t x, int16_t y, int16_t size);
void iconCounter(int16_t x, int16_t y, int16_t size);
