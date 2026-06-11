#include "apps.h"
#include "ui.h"
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

// ---------------------------------------------------------------------
// Clock - uptime clock (HH:MM:SS since boot, no RTC needed)
// ---------------------------------------------------------------------

static unsigned long clockLastUpdate = 0;
static const Rect clockDisplay = {10, 120, SCREEN_WIDTH - 20, 60};

static void clockOpen() {
  clockLastUpdate = 0;
  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
}

static void clockUpdate() {
  unsigned long now = millis();
  if (clockLastUpdate != 0 && now - clockLastUpdate < 1000) return;
  clockLastUpdate = now;

  unsigned long secs = now / 1000;
  int h = (secs / 3600) % 24;
  int m = (secs / 60) % 60;
  int s = secs % 60;

  char buf[16];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);

  drawCenteredText(clockDisplay, buf, &FreeSansBold24pt7b, ILI9341_GREEN);
}

static void clockTouch(int16_t x, int16_t y) {}
static void clockDrag(int16_t x, int16_t y) {}

const App clockApp = {"Clock", rgb565(0, 121, 140), iconClock, clockOpen, clockUpdate, clockTouch, clockDrag};

// ---------------------------------------------------------------------
// Pomodoro - 25 minute countdown timer with start/pause/reset
// ---------------------------------------------------------------------

static const long POMODORO_SECONDS = 25 * 60;
static long pomodoroRemaining = POMODORO_SECONDS;
static bool pomodoroRunning = false;
static unsigned long pomodoroLastTick = 0;

static const Rect pomodoroStartBtn = {20, 220, 90, 40};
static const Rect pomodoroResetBtn = {130, 220, 90, 40};
static const Rect pomodoroDisplay = {10, 90, SCREEN_WIDTH - 20, 80};

static void pomodoroDrawTime() {
  int m = pomodoroRemaining / 60;
  int s = pomodoroRemaining % 60;
  char buf[8];
  snprintf(buf, sizeof(buf), "%02d:%02d", m, s);

  drawCenteredText(pomodoroDisplay, buf, &FreeSansBold24pt7b, ILI9341_WHITE);
}

static void pomodoroOpen() {
  pomodoroRemaining = POMODORO_SECONDS;
  pomodoroRunning = false;
  pomodoroLastTick = millis();

  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
  pomodoroDrawTime();
  drawButton(pomodoroStartBtn, "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
  drawButton(pomodoroResetBtn, "Reset", ILI9341_MAROON, ILI9341_WHITE);
}

static void pomodoroUpdate() {
  if (!pomodoroRunning) return;
  unsigned long now = millis();
  if (now - pomodoroLastTick < 1000) return;
  pomodoroLastTick = now;

  if (pomodoroRemaining > 0) {
    pomodoroRemaining--;
    pomodoroDrawTime();
  }
}

static void pomodoroTouch(int16_t x, int16_t y) {
  if (pomodoroStartBtn.contains(x, y)) {
    pomodoroRunning = !pomodoroRunning;
    pomodoroLastTick = millis();
    drawButton(pomodoroStartBtn, pomodoroRunning ? "Pause" : "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
  } else if (pomodoroResetBtn.contains(x, y)) {
    pomodoroRemaining = POMODORO_SECONDS;
    pomodoroRunning = false;
    drawButton(pomodoroStartBtn, "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
    pomodoroDrawTime();
  }
}

static void pomodoroDrag(int16_t x, int16_t y) {}

const App pomodoroApp = {"Pomodoro", rgb565(198, 40, 40), iconPomodoro, pomodoroOpen, pomodoroUpdate, pomodoroTouch, pomodoroDrag};

// ---------------------------------------------------------------------
// Pong - tiny single-player Pong, drag to move the paddle
// ---------------------------------------------------------------------

static const int16_t pongPlayfieldTop = 40;
static const int16_t pongPaddleW = 60;
static const int16_t pongPaddleH = 10;
static const int16_t pongPaddleY = SCREEN_HEIGHT - 20;
static const int16_t pongBallSize = 8;

static float pongBallX, pongBallY, pongBallVX, pongBallVY;
static int16_t pongPaddleX;
static unsigned long pongLastFrame = 0;

static void pongDrawPaddle(int16_t oldX) {
  tft.fillRect(oldX, pongPaddleY, pongPaddleW, pongPaddleH, ILI9341_BLACK);
  tft.fillRect(pongPaddleX, pongPaddleY, pongPaddleW, pongPaddleH, ILI9341_BLUE);
}

static void pongOpen() {
  pongBallX = SCREEN_WIDTH / 2;
  pongBallY = pongPlayfieldTop + 20;
  pongBallVX = 2;
  pongBallVY = 2;
  pongPaddleX = (SCREEN_WIDTH - pongPaddleW) / 2;
  pongLastFrame = millis();

  tft.fillRect(0, pongPlayfieldTop, SCREEN_WIDTH, SCREEN_HEIGHT - pongPlayfieldTop, ILI9341_BLACK);
  tft.fillRect(pongPaddleX, pongPaddleY, pongPaddleW, pongPaddleH, ILI9341_BLUE);
}

static void pongUpdate() {
  unsigned long now = millis();
  if (now - pongLastFrame < 30) return;
  pongLastFrame = now;

  int16_t oldBX = (int16_t)pongBallX;
  int16_t oldBY = (int16_t)pongBallY;

  pongBallX += pongBallVX;
  pongBallY += pongBallVY;

  if (pongBallX <= 0 || pongBallX >= SCREEN_WIDTH - pongBallSize) pongBallVX = -pongBallVX;
  if (pongBallY <= pongPlayfieldTop) pongBallVY = -pongBallVY;

  if (pongBallY + pongBallSize >= pongPaddleY &&
      pongBallX + pongBallSize >= pongPaddleX &&
      pongBallX <= pongPaddleX + pongPaddleW) {
    pongBallVY = -pongBallVY;
    pongBallY = pongPaddleY - pongBallSize;
  }

  if (pongBallY > SCREEN_HEIGHT) {
    pongBallX = SCREEN_WIDTH / 2;
    pongBallY = pongPlayfieldTop + 20;
  }

  tft.fillRect(oldBX, oldBY, pongBallSize, pongBallSize, ILI9341_BLACK);
  tft.fillRect((int16_t)pongBallX, (int16_t)pongBallY, pongBallSize, pongBallSize, ILI9341_BLUE);
}

static void pongTouch(int16_t x, int16_t y) {}

static void pongDrag(int16_t x, int16_t y) {
  int16_t oldX = pongPaddleX;
  pongPaddleX = constrain((int16_t)(x - pongPaddleW / 2), (int16_t)0, (int16_t)(SCREEN_WIDTH - pongPaddleW));
  if (pongPaddleX != oldX) pongDrawPaddle(oldX);
}

const App pongApp = {"Pong", rgb565(25, 60, 180), iconPong, pongOpen, pongUpdate, pongTouch, pongDrag};

// ---------------------------------------------------------------------
// Sensor - live bar graph of an analog input (LDR, potentiometer, ...)
// ---------------------------------------------------------------------

static const Rect sensorBar = {40, 100, 160, SCREEN_HEIGHT - 140};
static const Rect sensorValueDisplay = {10, 50, SCREEN_WIDTH - 20, 40};
static unsigned long sensorLastUpdate = 0;

static void sensorOpen() {
  sensorLastUpdate = 0;
  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
  tft.drawRect(sensorBar.x, sensorBar.y, sensorBar.w, sensorBar.h, ILI9341_WHITE);
}

static void sensorUpdate() {
  unsigned long now = millis();
  if (now - sensorLastUpdate < 100) return;
  sensorLastUpdate = now;

  int value = analogRead(SENSOR_PIN); // 0-4095
  int barH = map(value, 0, 4095, 0, sensorBar.h - 4);

  tft.fillRect(sensorBar.x + 2, sensorBar.y + 2, sensorBar.w - 4, sensorBar.h - 4, ILI9341_BLACK);
  tft.fillRect(sensorBar.x + 2, sensorBar.y + sensorBar.h - 2 - barH, sensorBar.w - 4, barH, ILI9341_ORANGE);

  char buf[8];
  snprintf(buf, sizeof(buf), "%d", value);
  drawCenteredText(sensorValueDisplay, buf, &FreeSans12pt7b, ILI9341_WHITE);
}

static void sensorTouch(int16_t x, int16_t y) {}
static void sensorDrag(int16_t x, int16_t y) {}

const App sensorApp = {"Sensor", rgb565(46, 125, 50), iconSensor, sensorOpen, sensorUpdate, sensorTouch, sensorDrag};

// ---------------------------------------------------------------------
// Messages - shows the latest notification pushed from the companion app
// ---------------------------------------------------------------------

static String notifyTitle = "No messages yet";
static String notifyMessage = "";
static bool notifyNeedsRedraw = true;

void notifySetMessage(const char *title, const char *msg) {
  notifyTitle = title;
  notifyMessage = msg;
  notifyNeedsRedraw = true;
}

static void notifyDraw() {
  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(10, 60);
  tft.println(notifyTitle);

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 100);
  tft.println(notifyMessage);
}

static void notifyOpen() {
  notifyNeedsRedraw = true;
}

static void notifyUpdate() {
  if (!notifyNeedsRedraw) return;
  notifyDraw();
  notifyNeedsRedraw = false;
}

static void notifyTouch(int16_t x, int16_t y) {}
static void notifyDrag(int16_t x, int16_t y) {}

const App notifyApp = {"Messages", rgb565(106, 27, 154), iconMessages, notifyOpen, notifyUpdate, notifyTouch, notifyDrag};

// ---------------------------------------------------------------------
// Stopwatch - count-up timer with start/pause/reset
// ---------------------------------------------------------------------

static unsigned long stopwatchElapsed = 0; // accumulated milliseconds while paused
static unsigned long stopwatchStartedAt = 0;
static bool stopwatchRunning = false;

static const Rect stopwatchStartBtn = {20, 220, 90, 40};
static const Rect stopwatchResetBtn = {130, 220, 90, 40};
static const Rect stopwatchDisplay = {10, 90, SCREEN_WIDTH - 20, 80};

static void stopwatchDrawTime() {
  unsigned long total = stopwatchElapsed;
  if (stopwatchRunning) total += millis() - stopwatchStartedAt;

  unsigned long secs = total / 1000;
  int m = (secs / 60) % 60;
  int s = secs % 60;
  char buf[8];
  snprintf(buf, sizeof(buf), "%02d:%02d", m, s);

  drawCenteredText(stopwatchDisplay, buf, &FreeSansBold24pt7b, ILI9341_WHITE);
}

static void stopwatchOpen() {
  stopwatchElapsed = 0;
  stopwatchRunning = false;

  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
  stopwatchDrawTime();
  drawButton(stopwatchStartBtn, "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
  drawButton(stopwatchResetBtn, "Reset", ILI9341_MAROON, ILI9341_WHITE);
}

static void stopwatchUpdate() {
  if (!stopwatchRunning) return;

  static unsigned long lastDraw = 0;
  unsigned long now = millis();
  if (now - lastDraw < 200) return;
  lastDraw = now;

  stopwatchDrawTime();
}

static void stopwatchTouch(int16_t x, int16_t y) {
  if (stopwatchStartBtn.contains(x, y)) {
    if (stopwatchRunning) {
      stopwatchElapsed += millis() - stopwatchStartedAt;
      stopwatchRunning = false;
    } else {
      stopwatchStartedAt = millis();
      stopwatchRunning = true;
    }
    drawButton(stopwatchStartBtn, stopwatchRunning ? "Pause" : "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
    stopwatchDrawTime();
  } else if (stopwatchResetBtn.contains(x, y)) {
    stopwatchElapsed = 0;
    stopwatchRunning = false;
    drawButton(stopwatchStartBtn, "Start", ILI9341_DARKGREEN, ILI9341_WHITE);
    stopwatchDrawTime();
  }
}

static void stopwatchDrag(int16_t x, int16_t y) {}

const App stopwatchApp = {"Stopwatch", rgb565(230, 81, 0), iconStopwatch, stopwatchOpen, stopwatchUpdate, stopwatchTouch, stopwatchDrag};

// ---------------------------------------------------------------------
// Dice - tap to roll a die or flip a coin
// ---------------------------------------------------------------------

static const Rect diceRollBtn = {20, 220, 90, 40};
static const Rect diceCoinBtn = {130, 220, 90, 40};
static const Rect diceDisplay = {(SCREEN_WIDTH - 100) / 2, 70, 100, 100};

static int diceValue = 1;
static bool diceShowCoin = false;
static bool diceCoinHeads = true;

static void diceDrawPips(int value) {
  int16_t s = diceDisplay.w;
  int16_t r = 6;
  int16_t left = diceDisplay.x + s / 4;
  int16_t right = diceDisplay.x + s - s / 4;
  int16_t top = diceDisplay.y + s / 4;
  int16_t bottom = diceDisplay.y + s - s / 4;
  int16_t cx = diceDisplay.x + s / 2;
  int16_t cy = diceDisplay.y + s / 2;

  if (value == 1 || value == 3 || value == 5) {
    tft.fillCircle(cx, cy, r, ILI9341_BLACK);
  }
  if (value >= 2) {
    tft.fillCircle(left, top, r, ILI9341_BLACK);
    tft.fillCircle(right, bottom, r, ILI9341_BLACK);
  }
  if (value >= 4) {
    tft.fillCircle(right, top, r, ILI9341_BLACK);
    tft.fillCircle(left, bottom, r, ILI9341_BLACK);
  }
  if (value == 6) {
    tft.fillCircle(left, cy, r, ILI9341_BLACK);
    tft.fillCircle(right, cy, r, ILI9341_BLACK);
  }
}

static void diceDraw() {
  tft.fillRect(diceDisplay.x, diceDisplay.y, diceDisplay.w, diceDisplay.h, ILI9341_BLACK);

  if (diceShowCoin) {
    Rect r = diceDisplay;
    drawCenteredText(r, diceCoinHeads ? "KOP" : "MUNT", &FreeSansBold24pt7b, ILI9341_WHITE);
  } else {
    tft.fillRoundRect(diceDisplay.x, diceDisplay.y, diceDisplay.w, diceDisplay.h, 10, ILI9341_WHITE);
    diceDrawPips(diceValue);
  }
}

static void diceOpen() {
  randomSeed(micros());
  diceValue = 1;
  diceShowCoin = false;

  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
  diceDraw();
  drawButton(diceRollBtn, "Dobbel", ILI9341_DARKGREEN, ILI9341_WHITE);
  drawButton(diceCoinBtn, "Munt", ILI9341_NAVY, ILI9341_WHITE);
}

static void diceUpdate() {}

static void diceTouch(int16_t x, int16_t y) {
  if (diceRollBtn.contains(x, y)) {
    diceValue = random(1, 7);
    diceShowCoin = false;
    diceDraw();
  } else if (diceCoinBtn.contains(x, y)) {
    diceCoinHeads = random(0, 2) == 0;
    diceShowCoin = true;
    diceDraw();
  }
}

static void diceDrag(int16_t x, int16_t y) {}

const App diceApp = {"Dice", rgb565(93, 64, 55), iconDice, diceOpen, diceUpdate, diceTouch, diceDrag};

// ---------------------------------------------------------------------
// Counter - simple +/- tally counter
// ---------------------------------------------------------------------

static long counterValue = 0;
static const Rect counterDisplay = {10, 90, SCREEN_WIDTH - 20, 80};
static const Rect counterMinusBtn = {10, 220, 66, 40};
static const Rect counterResetBtn = {87, 220, 66, 40};
static const Rect counterPlusBtn = {164, 220, 66, 40};

static void counterDrawValue() {
  char buf[12];
  snprintf(buf, sizeof(buf), "%ld", counterValue);
  drawCenteredText(counterDisplay, buf, &FreeSansBold24pt7b, ILI9341_WHITE);
}

static void counterOpen() {
  counterValue = 0;

  tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, ILI9341_BLACK);
  counterDrawValue();
  drawButton(counterMinusBtn, "-", ILI9341_MAROON, ILI9341_WHITE);
  drawButton(counterResetBtn, "Reset", ILI9341_DARKGREY, ILI9341_WHITE);
  drawButton(counterPlusBtn, "+", ILI9341_DARKGREEN, ILI9341_WHITE);
}

static void counterUpdate() {}

static void counterTouch(int16_t x, int16_t y) {
  if (counterMinusBtn.contains(x, y)) {
    counterValue--;
    counterDrawValue();
  } else if (counterPlusBtn.contains(x, y)) {
    counterValue++;
    counterDrawValue();
  } else if (counterResetBtn.contains(x, y)) {
    counterValue = 0;
    counterDrawValue();
  }
}

static void counterDrag(int16_t x, int16_t y) {}

const App counterApp = {"Counter", rgb565(55, 71, 79), iconCounter, counterOpen, counterUpdate, counterTouch, counterDrag};
