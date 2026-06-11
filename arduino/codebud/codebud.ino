// CodeBud - mini "kodeOS"-style launcher for ESP32 + ILI9341 (SPI) TFT,
// Arduino IDE edition.
//
// Uses Adafruit_GFX + Adafruit_ILI9341 + XPT2046_Touchscreen, which need
// no extra config files (unlike TFT_eSPI/LVGL) - just install the
// libraries listed in README.md and upload.
//
// See README.md for wiring and library setup.

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#include "config.h"
#include "launcher.h"
#include "ble_service.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ_PIN);

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(0);

  ts.begin();
  ts.setRotation(1);

  bleInit();
  launcherShowHome();
}

void loop() {
  launcherUpdate();
  bleProcess();
  delay(5);
}
