# CodeBud display test (Arduino IDE)

Geen terminal of PlatformIO nodig - alles via de Arduino IDE met
duidelijke knoppen.

## 1. Arduino IDE installeren

Download en installeer de Arduino IDE: https://www.arduino.cc/en/software

## 2. ESP32 board-ondersteuning toevoegen

1. Open de Arduino IDE.
2. Ga naar **File > Preferences** (Bestand > Voorkeuren).
3. Bij "Additional boards manager URLs" voeg toe:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Ga naar **Tools > Board > Boards Manager**, zoek naar "esp32" en
   installeer "esp32 by Espressif Systems".

## 3. Benodigde libraries installeren

Ga naar **Tools > Manage Libraries** (Sketch > Include Library > Manage
Libraries) en installeer:

- **Adafruit GFX Library**
- **Adafruit ILI9341**

(Installeer eventuele afhankelijkheden mee als de IDE daarom vraagt.)

## 4. Sketch openen

Open `codebud_test.ino` (in deze map) in de Arduino IDE.

## 5. Board en poort instellen

1. **Tools > Board > esp32 > ESP32 Dev Module**
2. **Tools > Port** -> kies de COM-poort van je ESP32 (zichtbaar zodra je
    'm via USB aansluit).

## 6. Uploaden

Klik op de **Upload**-knop (pijltje rechtsboven). De IDE compileert en
flasht de sketch.

## 7. Resultaat

Het scherm doorloopt continu: **rood -> groen -> blauw -> "CodeBud
booted"**, elk 1-2 seconden. Open ook **Tools > Serial Monitor** (115200
baud) om te zien of de ESP32 wel degelijk draait, ook als het scherm
niets laat zien.

## Bedrading

| TFT pin | ESP32 pin |
|---------|-----------|
| VCC     | 3V3       |
| GND     | GND       |
| CS      | GPIO 15   |
| RST     | GPIO 4    |
| DC      | GPIO 2    |
| MOSI/SDI| GPIO 23   |
| SCK     | GPIO 18   |
| MISO/SDO| GPIO 19   |
| LED     | 3V3       |

Als het scherm helemaal niets laat zien (geen kleuren), controleer dan
eerst de bedrading hierboven en dat VCC naar 3V3 gaat (niet 5V).
