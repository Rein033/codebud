#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, SCREEN_HEIGHT / 2 - 10);
  tft.println("CodeBud booted");

  Serial.println("CodeBud booted");
}

void loop() {
}
