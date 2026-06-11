#include "ble_service.h"

// BLE is temporarily disabled: NimBLE-Arduino 1.4.x crashes on newer
// ESP32 Arduino cores (esp_bt_controller_init returns
// ESP_ERR_INVALID_STATE). These are no-op stubs so the launcher and
// apps work without the NimBLE/ArduinoJson dependencies. See
// README.md for how to re-enable BLE.

void bleInit() {}
void bleProcess() {}
void bleSetStatus(const char *appName) {}
