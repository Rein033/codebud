# codebud

A small "kodeOS"-style launcher for an ESP32 + ILI9341 (SPI) TFT display, inspired
by [Kode Dot](https://www.kode.diy). It boots into a home screen with app tiles;
tapping a tile opens that app full-screen with a "Back" button to return.

Built with [PlatformIO](https://platformio.org/), Arduino framework, [LVGL](https://lvgl.io/) v8
for the UI, [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) as the display driver and
[XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) for touch input.

## Included demo apps

- **Clock** - uptime clock (HH:MM:SS since boot, no RTC needed)
- **Pomodoro** - 25 minute countdown timer with start/pause/reset
- **Pong** - tiny single-player Pong, drag to move the paddle
- **Sensor** - live bar graph of an analog input (LDR, potentiometer, ...)

Add new apps by implementing the `App` struct in `src/apps/app.h` and registering
it in the `apps[]` array in `src/launcher.cpp`.

## Wiring

Default pins (set as build flags in `platformio.ini`, no `User_Setup.h` editing needed):

| Signal      | ESP32 pin |
|-------------|-----------|
| TFT_MISO    | 19        |
| TFT_MOSI    | 23        |
| TFT_SCLK    | 18        |
| TFT_CS      | 15        |
| TFT_DC      | 2         |
| TFT_RST     | 4         |
| TOUCH_CS    | 21        |
| TOUCH_IRQ   | 22 (`include/config.h`) |
| SENSOR_PIN  | 34 (`include/config.h`, analog input) |

The display and touch controller share the same SPI bus (VSPI: pins 18/19/23),
each with their own chip-select pin. Adjust the pins in `platformio.ini`
(`-DTFT_*` / `-DTOUCH_CS`) and `include/config.h` to match your wiring.

If you have a 128x160 ST7735 panel instead of a 240x320 ILI9341, change the driver
and resolution build flags in `platformio.ini` (`ST7735_DRIVER`, `TFT_WIDTH`,
`TFT_HEIGHT`) and update `SCREEN_WIDTH`/`SCREEN_HEIGHT` in `include/config.h`.

## Touch calibration

`src/main.cpp` maps raw XPT2046 readings to screen coordinates using rough
default values (`200-3700` for X, `240-3800` for Y). If touches feel off or
inverted, tweak the `map()` calls in `touchpad_read()` or the touch
`setRotation()` value to match your panel's orientation.

## Build & flash

```sh
pio run                 # build
pio run -t upload       # build + flash (USB)
pio device monitor       # serial monitor
```
