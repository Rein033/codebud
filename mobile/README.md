# CodeBud companion app

Flutter app that talks to the CodeBud ESP32 launcher over Bluetooth Low
Energy (BLE), using [flutter_blue_plus](https://pub.dev/packages/flutter_blue_plus).

## Features

- Scans for and connects to a "CodeBud" device
- Shows the currently active screen/app on the device (live, via BLE notify)
- Open any app on the device remotely (Clock, Pomodoro, Pong, Sensor, Messages)
- Send a title + message that's shown in the device's "Messages" app

## Setup

The Android and iOS platform projects are included. Required BLE permissions
are already set up:

- Android: `BLUETOOTH_SCAN`/`BLUETOOTH_CONNECT` (Android 12+) and legacy
  `BLUETOOTH`/`BLUETOOTH_ADMIN`/`ACCESS_FINE_LOCATION` (Android <= 11) in
  `android/app/src/main/AndroidManifest.xml`
- iOS: `NSBluetoothAlwaysUsageDescription` in `ios/Runner/Info.plist`

```sh
cd mobile
flutter pub get
```

## Run

```sh
flutter run                 # debug, on a connected device/emulator
flutter build apk --release # release APK: build/app/outputs/flutter-apk/app-release.apk
```

## BLE protocol

Matches `src/ble_service.cpp` in the firmware:

| | UUID |
|---|---|
| Service | `a07498ca-ad5b-474e-940d-16f1fbe7e8cd` |
| Command characteristic (write) | `51ff12bb-3ed8-46e5-b4f9-d64e2fec021b` |
| Status characteristic (read/notify) | `51ff12bb-3ed8-46e5-b4f9-d64e2fec021c` |

Commands are JSON written to the command characteristic:

```json
{"cmd": "open", "app": "Pong"}
{"cmd": "notify", "title": "Hi", "msg": "Hello from your phone!"}
```

The status characteristic holds the plain-text name of the app/screen
currently shown on the device (e.g. `home`, `Clock`, `Messages`).
