#include "launcher.h"
#include "ui.h"
#include "ble_service.h"

static const App *apps[] = {&clockApp, &pomodoroApp, &pongApp, &sensorApp, &notifyApp};
static const int numApps = sizeof(apps) / sizeof(apps[0]);

static const App *activeApp = nullptr;
static bool wasPressed = false;

static const Rect backBtn = {5, 5, 70, 30};
static const int16_t tileMargin = 10;
static const int16_t tileW = (SCREEN_WIDTH - tileMargin * 3) / 2;
static const int16_t tileH = 70;
static const int16_t tilesTop = 50;

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
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 10);
  tft.println("CodeBud");

  for (int i = 0; i < numApps; i++) {
    drawButton(tileRect(i), apps[i]->name, ILI9341_NAVY, ILI9341_WHITE);
  }

  bleSetStatus("home");
}

void launcherOpenApp(const App *app) {
  activeApp = app;

  tft.fillScreen(ILI9341_BLACK);
  drawButton(backBtn, "< Back", ILI9341_DARKGREY, ILI9341_WHITE);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(90, 14);
  tft.println(app->name);

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
