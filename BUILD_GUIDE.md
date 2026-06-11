# Bouwgids: van 0 naar 100

Stap-voor-stap gids om je eigen "CodeBud" in elkaar te zetten: hardware
aansluiten, firmware flashen, en koppelen met de companion-app.

## 1. Wat heb je nodig

**Verplicht:**
- ESP32 development board (bv. "ESP32 DevKit V1", 30-pins)
- 2.4" of 2.8" SPI TFT-display met **ILI9341**-driver, 240x320, **met XPT2046
  touch-controller** (zeer gangbare "ILI9341 + touch" modules op AliExpress/
  Amazon)
- Breadboard + jumper-wires (male-male of male-female, afhankelijk van je
  display-pinnen)
- USB-kabel (data, niet alleen lader) voor je ESP32

**Optioneel:**
- LDR (lichtsensor) of potentiometer + weerstand, voor de "Sensor"-app
- Behuizing / 3D-printbox (later, als alles werkt)

**Op je computer:**
- [VS Code](https://code.visualstudio.com/) + [PlatformIO IDE extensie](https://platformio.org/install/ide?install=vscode)
  (of de PlatformIO CLI, zie stap 4)
- Git
- Voor de mobiele app: een Android-telefoon met Bluetooth (de kant-en-klare
  APK is al gedeeld; zelf bouwen kan met Flutter, zie `mobile/README.md`)

> Heb je een ander display (bv. ST7735, kleiner 128x160 schermpje, of geen
> touch)? Dat kan ook, maar vraagt aanpassingen in `platformio.ini` /
> `include/config.h` - zie de "Ander display" sectie onderaan.

## 2. Bedrading

Het display en de touch-controller delen dezelfde SPI-bus. Sluit aan volgens
onderstaande tabel (komt overeen met de defaults in `platformio.ini` en
`include/config.h` - dus geen code-aanpassingen nodig als je dit zo aansluit):

| Display/touch pin | ESP32 pin | Opmerking |
|--------------------|-----------|-----------|
| VCC                | 3V3       | **Niet 5V** tenzij je module dat expliciet ondersteunt |
| GND                | GND       | |
| CS (display, vaak "TFT_CS") | GPIO 15 | |
| RESET / RST        | GPIO 4    | |
| DC / RS            | GPIO 2    | |
| SDI / MOSI         | GPIO 23   | gedeeld met touch |
| SCK / SCLK         | GPIO 18   | gedeeld met touch |
| LED / backlight    | 3V3       | direct aan, of via GPIO als je dimmen wilt |
| SDO / MISO         | GPIO 19   | gedeeld met touch |
| T_CLK (touch)      | GPIO 18   | gedeeld met display SCK |
| T_CS (touch)       | GPIO 21   | |
| T_DIN (touch)      | GPIO 23   | gedeeld met display MOSI |
| T_DO (touch)       | GPIO 19   | gedeeld met display MISO |
| T_IRQ (touch)      | GPIO 22   | |

Optioneel, voor de **Sensor**-app (LDR of potmeter):

| Sensor pin | ESP32 pin |
|------------|-----------|
| Signaal (wiper/output) | GPIO 34 (analoge input) |
| VCC | 3V3 |
| GND | GND |

### Stap voor stap

1. Steek de ESP32 in het breadboard zodat beide rijen pinnen vrij toegankelijk
   zijn (gebruik bij voorkeur een breadboard dat breed genoeg is).
2. Verbind eerst **stroom**: VCC van het display naar 3V3, GND naar GND. Sluit
   nog niets op de ESP32 aan op USB.
3. Verbind de **SPI-bus**: SCK -> GPIO18, MOSI/SDI -> GPIO23, MISO/SDO -> GPIO19.
4. Verbind de **display-besturingspinnen**: CS -> GPIO15, DC/RS -> GPIO2,
   RESET -> GPIO4.
5. Verbind de **touch-pinnen**: T_CS -> GPIO21, T_IRQ -> GPIO22 (T_CLK, T_DIN,
   T_DO delen de SPI-bus met het display, zoals hierboven).
6. (Optioneel) sluit je sensor aan op GPIO34 + 3V3 + GND.
7. Controleer alles nog eens met de tabel - verkeerde bedrading is de meest
   voorkomende oorzaak van een wit/zwart scherm.
8. Sluit de ESP32 nu pas via USB aan op je computer.

## 3. Repo ophalen

```sh
git clone <jouw-fork-of-deze-repo>
cd codebud
git checkout claude/esp32-dit-na-3fiob3   # of main, afhankelijk van waar je werkt
```

## 4. Firmware flashen

### Met VS Code + PlatformIO

1. Open de map `codebud` in VS Code.
2. Wacht tot de PlatformIO-extensie de project automatisch herkent (rechtsonder
   verschijnt een PlatformIO-statusbalk).
3. Klik op het "Build" (vinkje) icoon om te compileren - dit downloadt de
   eerste keer alle libraries en kan een paar minuten duren.
4. Sluit de ESP32 aan via USB, klik op "Upload" (pijltje icoon).
5. Open de "Serial Monitor" (stekker-icoon) op 115200 baud om logoutput te zien.

### Met de command line

```sh
pip install -U platformio
pio run                 # build
pio run -t upload       # build + flash via USB
pio device monitor       # seriële monitor (Ctrl+C om te stoppen)
```

Als `pio run -t upload` de juiste poort niet vindt, geef 'm expliciet mee:

```sh
pio run -t upload --upload-port /dev/ttyUSB0   # Linux
pio run -t upload --upload-port COM5           # Windows
```

## 5. Eerste keer opstarten

Na het flashen herstart de ESP32 vanzelf. Je zou het volgende moeten zien:

1. Kort een leeg/wit scherm (display init).
2. De **launcher**: een grid met tegels - Clock, Pomodoro, Pong, Sensor,
   Messages.
3. Tik op een tegel om de app te openen, en op "< Back" om terug te gaan.

### Niets te zien op het scherm?

- Controleer de bedrading (vooral SCK/MOSI/MISO/CS/DC/RST) - zie stap 2.
- Controleer dat je VCC naar 3V3 gaat, niet 5V.
- Open de seriële monitor (`pio device monitor`) - crashes/reboots zie je hier.
- Probeer `tft.setRotation(0)` in `src/main.cpp` te wijzigen naar `1`, `2` of
  `3` als het beeld omgekeerd/gedraaid is (of leeg blijft door een verkeerde
  resolutie-oriëntatie).

### Touch werkt niet (goed)?

De ruwe touch-waarden verschillen per paneel. Pas in `src/main.cpp` de
`map()`-aanroepen in `touchpad_read()` aan:

```cpp
data->point.x = constrain(map(p.x, 200, 3700, 0, SCREEN_WIDTH), 0, SCREEN_WIDTH - 1);
data->point.y = constrain(map(p.y, 240, 3800, 0, SCREEN_HEIGHT), 0, SCREEN_HEIGHT - 1);
```

Tips:
- Werkt X/Y verwisseld? Wissel de twee regels om, of pas `ts.setRotation(...)`
  aan (probeer 0-3).
- Werkt het geïnverteerd (links/rechts of boven/onder omgekeerd)? Verwissel de
  twee getallen in de bijbehorende `map()`-aanroep, bv. `map(p.x, 3700, 200, 0, SCREEN_WIDTH)`.
- Reageert touch helemaal niet? Controleer T_CS/T_IRQ-bedrading en of T_CLK/
  T_DIN/T_DO echt op dezelfde pinnen zitten als de display-SPI.

## 6. Koppelen met de mobiele app

1. Installeer de gedeelde APK op je Android-telefoon (sta "installeren uit
   onbekende bronnen" toe).
2. Zorg dat de ESP32 aan staat en de launcher toont.
3. Open de app, geef Bluetooth-toestemming, en tik op **Verbinden** - hij
   scant naar een device genaamd "CodeBud".
4. Zodra verbonden zie je het actieve scherm van de ESP32 live in de app.
5. Tik op een app-tegel in de app om die op de ESP32 te openen, of stuur een
   bericht via het formulier (verschijnt in de **Messages**-app op het
   apparaat).

Werkt de verbinding niet?
- Bluetooth aan op je telefoon, en locatie/nabije-apparaten-permissie
  toegestaan (vereist door Android voor BLE-scans).
- Niet al verbonden vanuit een andere app/telefoon - BLE staat meestal maar
  één verbinding tegelijk toe.
- Check de seriële monitor: bij een geslaagde `ble_init()` adverteert de
  ESP32 voortdurend.

## 7. Volgende stappen

- Voeg je eigen app toe: implementeer de `App`-struct in `src/apps/app.h` en
  registreer 'm in `apps[]` in `src/launcher.cpp`.
- Print of knutsel een behuizing zodra de bedrading werkt - test daarna
  opnieuw of alle knoppen/touch nog goed bereikbaar zijn.
- Wil je geen breadboard meer? Soldeer de verbindingen op een stripboard voor
  een steviger geheel.

## Ander display (ST7735 / geen touch / andere pinnen)

- **ST7735 (128x160)**: in `platformio.ini`, vervang `-DILI9341_DRIVER=1` door
  `-DST7735_DRIVER=1` en pas `-DTFT_WIDTH=128` / `-DTFT_HEIGHT=160` aan.
  Update ook `SCREEN_WIDTH`/`SCREEN_HEIGHT` in `include/config.h`. ST7735-modules
  hebben meestal geen touch - verwijder dan de XPT2046-init in
  `src/main.cpp` en registreer geen `lv_indev_drv` (of laat 'm staan; zonder
  touch-hardware doet `ts.touched()` gewoon niets).
- **Andere pinnen**: pas de `-DTFT_*`/`-DTOUCH_CS` build flags in
  `platformio.ini` en `TOUCH_IRQ_PIN`/`SENSOR_PIN` in `include/config.h` aan
  naar jouw bedrading.
