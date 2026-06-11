#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);

  Serial.println("CodeBud display test started");
}

void loop() {
  tft.fillScreen(TFT_RED);
  Serial.println("RED");
  delay(1000);

  tft.fillScreen(TFT_GREEN);
  Serial.println("GREEN");
  delay(1000);

  tft.fillScreen(TFT_BLUE);
  Serial.println("BLUE");
  delay(1000);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, SCREEN_HEIGHT / 2 - 10);
  tft.println("CodeBud booted");
  Serial.println("TEXT");
  delay(2000);
}
