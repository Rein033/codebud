# CodeBud companion app

Flutter app that talks to the CodeBud ESP32 launcher over Bluetooth Low
Energy (BLE), using [flutter_blue_plus](https://pub.dev/packages/flutter_blue_plus).

## Features

- Scans for and connects to a "CodeBud" device
- Shows the currently active screen/app on the device (live, via BLE notify)
- Open any app on the device remotely (Clock, Pomodoro, Pong, Sensor, Messages)
- Send a title + message that's shown in the device's "Messages" app

## Setup

This directory contains `pubspec.yaml` and `lib/`, but not the generated
`android/`/`ios/` platform projects (they need to be created with your local
Flutter SDK version):

```sh
cd mobile
flutter create .          # generates android/, ios/, etc. for this project
flutter pub get
```

### Android permissions

BLE scanning/connecting on Android 12+ requires runtime permissions. Add to
`android/app/src/main/AndroidManifest.xml`:

```xml
<uses-permission android:name="android.permission.BLUETOOTH_SCAN" android:usesPermissionFlags="neverForLocation" />
<uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />
```

### iOS permissions

Add to `ios/Runner/Info.plist`:

```xml
<key>NSBluetoothAlwaysUsageDescription</key>
<string>CodeBud gebruikt Bluetooth om met je device te verbinden.</string>
```

## Run

```sh
flutter run
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
