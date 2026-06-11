# codebud

A minimal Arduino "boot" sketch for an ESP32 + ILI9341 (SPI) TFT display.
On boot it initializes the display, clears it, and prints "CodeBud booted".

Built with [PlatformIO](https://platformio.org/), Arduino framework, and
[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) as the display driver.

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

If you have a 128x160 ST7735 panel instead of a 240x320 ILI9341, change the driver
and resolution build flags in `platformio.ini` (`ST7735_DRIVER`, `TFT_WIDTH`,
`TFT_HEIGHT`) and update `SCREEN_WIDTH`/`SCREEN_HEIGHT` in `include/config.h`.

## Build & flash

```sh
pio run                 # build
pio run -t upload       # build + flash (USB)
pio device monitor       # serial monitor
```
