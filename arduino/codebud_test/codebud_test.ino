// CodeBud display test for the Arduino IDE.
//
// Uses Adafruit_GFX + Adafruit_ILI9341, which need no extra
// configuration files (unlike TFT_eSPI) - just install both
// libraries via the Library Manager and upload.
//
// Wiring (ESP32 default VSPI pins):
//   TFT VCC  -> 3V3
//   TFT GND  -> GND
//   TFT CS   -> GPIO 15
//   TFT RST  -> GPIO 4
//   TFT DC   -> GPIO 2
//   TFT MOSI -> GPIO 23
//   TFT SCK  -> GPIO 18
//   TFT MISO -> GPIO 19
//   TFT LED  -> 3V3

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  Serial.println("CodeBud display test started");
}

void loop() {
  tft.fillScreen(ILI9341_RED);
  Serial.println("RED");
  delay(1000);

  tft.fillScreen(ILI9341_GREEN);
  Serial.println("GREEN");
  delay(1000);

  tft.fillScreen(ILI9341_BLUE);
  Serial.println("BLUE");
  delay(1000);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(20, tft.height() / 2 - 10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("CodeBud booted");
  Serial.println("TEXT");
  delay(2000);
}
