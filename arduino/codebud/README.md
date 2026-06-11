# CodeBud (Arduino IDE)

Volledige "kodeOS"-stijl launcher voor ESP32 + ILI9341 (240x320, SPI) met
XPT2046 touch, gebouwd voor de Arduino IDE - geen PlatformIO of
config-bestanden nodig.

## Apps

- **Clock** - uptime klok (HH:MM:SS sinds boot)
- **Pomodoro** - 25 minuten countdown met start/pauze/reset
- **Pong** - sleep om de paddle te bewegen
- **Sensor** - live staafdiagram van een analoge ingang (LDR, potmeter, ...)
- **Messages** - toont het laatste bericht van de companion-app via BLE

## 1. Arduino IDE + ESP32 board support

Zie `../codebud_test/README.md` stap 1-2 als je dit nog niet hebt gedaan
(Arduino IDE installeren + ESP32 boardondersteuning toevoegen).

## 2. Libraries installeren

Ga naar **Tools > Manage Libraries** en installeer:

- **Adafruit GFX Library**
- **Adafruit ILI9341**
- **XPT2046_Touchscreen** (door Paul Stoffregen)

## 3. Sketch openen

Open `codebud.ino` in deze map. De andere bestanden (`.h`/`.cpp`)
verschijnen als tabs in de IDE - hoort zo, niet verwijderen.

## 4. Board en poort instellen

1. **Tools > Board > esp32 > ESP32 Dev Module**
2. **Tools > Port** -> kies de COM-poort van je ESP32

## 5. Uploaden

Klik op **Upload**. Na het flashen toont het scherm de launcher met
5 tegels: Clock, Pomodoro, Pong, Sensor, Messages.

## Bedrading

| Display/touch pin | ESP32 pin |
|--------------------|-----------|
| VCC                | 3V3       |
| GND                | GND       |
| CS                 | GPIO 15   |
| RESET              | GPIO 4    |
| DC                 | GPIO 2    |
| SDI/MOSI           | GPIO 23   |
| SCK                | GPIO 18   |
| LED                | 3V3       |
| SDO/MISO           | GPIO 19   |
| T_CS               | GPIO 21   |
| T_IRQ              | GPIO 22   |
| T_CLK / T_DIN / T_DO | gedeeld met display SCK/MOSI/MISO |

## Touch kalibratie

Werkt touch verkeerd om of verschoven? Pas de `map()`-aanroepen in
`ui.cpp` (`readTouch()`) aan, of probeer `ts.setRotation(...)` in
`codebud.ino` (0-3).

## BLE companion app (momenteel uitgeschakeld)

`ble_service.cpp` bevat nu lege stub-functies. De NimBLE-Arduino 1.4.x
library crasht op nieuwere ESP32 Arduino cores
(`esp_bt_controller_init` -> `ESP_ERR_INVALID_STATE`), dus BLE is
uitgeschakeld zodat de launcher en apps zonder problemen werken.

Om BLE later weer aan te zetten, zal de implementatie moeten worden
bijgewerkt naar de NimBLE-Arduino 2.x API (andere callback-signatures),
of moet je een oudere ESP32 board package (2.x) gebruiken die compatibel
is met NimBLE 1.4.x.

De geplande BLE-service/UUIDs (zelfde als de PlatformIO-versie):

```json
{"cmd": "open", "app": "Pong"}
{"cmd": "notify", "title": "Hi", "msg": "Hello from your phone!"}
```

Service UUID: `a07498ca-ad5b-474e-940d-16f1fbe7e8cd`
