#include "apps.h"
#include "ui.h"

// ---------------------------------------------------------------------
// Clock - uptime clock (HH:MM:SS since boot, no RTC needed)
// ---------------------------------------------------------------------

static unsigned long clockLastUpdate = 0;

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

  tft.fillRect(20, 140, 200, 40, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 140);
  tft.print(buf);
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

static void pomodoroDrawTime() {
  int m = pomodoroRemaining / 60;
  int s = pomodoroRemaining % 60;
  char buf[8];
  snprintf(buf, sizeof(buf), "%02d:%02d", m, s);

  tft.fillRect(40, 100, 160, 60, ILI9341_BLACK);
  tft.setTextSize(6);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(40, 100);
  tft.print(buf);
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
  snprintf(buf, sizeof(buf), "%4d", value);
  tft.fillRect(40, 60, 100, 30, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(40, 60);
  tft.print(buf);
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
