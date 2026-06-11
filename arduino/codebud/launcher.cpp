#include "launcher.h"
#include "ui.h"
#include "ble_service.h"
#include <Fonts/FreeSansBold12pt7b.h>

static const App *apps[] = {&clockApp, &pomodoroApp, &pongApp, &sensorApp, &notifyApp, &stopwatchApp, &diceApp, &counterApp};
static const int numApps = sizeof(apps) / sizeof(apps[0]);

static const App *activeApp = nullptr;
static bool wasPressed = false;

static const int16_t headerHeight = 40;
static const Rect backBtn = {0, 0, 70, headerHeight};
static const int16_t tileMargin = 10;
static const int16_t tileW = (SCREEN_WIDTH - tileMargin * 3) / 2;
static const int16_t tileH = 60;
static const int16_t tilesTop = headerHeight + 10;

static void drawHeader(const char *title, bool showBack) {
  uint16_t bg = showBack ? ILI9341_DARKGREY : ILI9341_BLACK;
  tft.fillRect(0, 0, SCREEN_WIDTH, headerHeight, bg);

  if (showBack) {
    int16_t cx = 24, cy = headerHeight / 2;
    tft.fillTriangle(cx + 8, cy - 10, cx + 8, cy + 10, cx - 8, cy, ILI9341_WHITE);
  }

  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  int16_t textX = showBack ? 50 : 10;
  tft.setCursor(textX, headerHeight / 2 + (int16_t)h / 2);
  tft.print(title);

  tft.setFont();
}

static Rect tileRect(int index) {
  int col = index % 2;
  int row = index / 2;
  Rect r;
  r.x = tileMargin + col * (tileW + tileMargin);
  r.y = tilesTop + row * (tileH + tileMargin);
  r.w = tileW;
  r.h = tileH;
  return r;
}

void launcherShowHome() {
  activeApp = nullptr;

  tft.fillScreen(ILI9341_BLACK);
  drawHeader("CodeBud", false);

  for (int i = 0; i < numApps; i++) {
    drawTile(tileRect(i), apps[i]->color, apps[i]->drawIcon, apps[i]->name);
  }

  bleSetStatus("home");
}

void launcherOpenApp(const App *app) {
  activeApp = app;

  tft.fillScreen(ILI9341_BLACK);
  drawHeader(app->name, true);

  app->onOpen();
  bleSetStatus(app->name);
}

void launcherOpenAppByName(const char *name) {
  for (int i = 0; i < numApps; i++) {
    if (strcasecmp(apps[i]->name, name) == 0) {
      launcherOpenApp(apps[i]);
      return;
    }
  }
}

void launcherUpdate() {
  if (activeApp) {
    activeApp->onUpdate();
  }

  TouchPoint t = readTouch();

  if (t.pressed && !wasPressed) {
    if (activeApp) {
      if (backBtn.contains(t.x, t.y)) {
        launcherShowHome();
      } else {
        activeApp->onTouch(t.x, t.y);
      }
    } else {
      for (int i = 0; i < numApps; i++) {
        if (tileRect(i).contains(t.x, t.y)) {
          launcherOpenApp(apps[i]);
          break;
        }
      }
    }
  }

  if (activeApp && t.pressed) {
    activeApp->onDrag(t.x, t.y);
  }

  wasPressed = t.pressed;
}
