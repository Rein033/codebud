#pragma once

// Starts BLE advertising and the GATT service used by the companion mobile app.
void ble_init();

// Applies any command received over BLE since the last call. Call from loop().
void ble_process();

// Publishes the name of the currently active screen/app over BLE (notify).
void ble_set_status(const char *app_name);
