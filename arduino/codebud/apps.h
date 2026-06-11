#pragma once

#include <stdint.h>

#include "ui.h"
#include "icons.h"

struct App {
  const char *name;
  uint16_t color;
  IconDrawFn drawIcon;
  void (*onOpen)();
  void (*onUpdate)();
  void (*onTouch)(int16_t x, int16_t y);
  void (*onDrag)(int16_t x, int16_t y);
};

extern const App clockApp;
extern const App pomodoroApp;
extern const App pongApp;
extern const App sensorApp;
extern const App notifyApp;

// Updates the message shown by the "Messages" app.
void notifySetMessage(const char *title, const char *msg);
