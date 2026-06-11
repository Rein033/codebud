import 'dart:async';
import 'dart:convert';

import 'package:flutter_blue_plus/flutter_blue_plus.dart';

/// BLE protocol for the CodeBud launcher firmware (see firmware
/// `src/ble_service.cpp` for the device-side implementation).
class CodeBudBle {
  static final Guid serviceUuid = Guid('a07498ca-ad5b-474e-940d-16f1fbe7e8cd');
  static final Guid commandCharUuid = Guid('51ff12bb-3ed8-46e5-b4f9-d64e2fec021b');
  static final Guid statusCharUuid = Guid('51ff12bb-3ed8-46e5-b4f9-d64e2fec021c');

  BluetoothDevice? _device;
  BluetoothCharacteristic? _commandChar;
  BluetoothCharacteristic? _statusChar;
  StreamSubscription<List<int>>? _statusSub;
  StreamSubscription<BluetoothConnectionState>? _connectionSub;

  final _statusController = StreamController<String>.broadcast();
  final _connectionController = StreamController<bool>.broadcast();

  /// Emits the name of the screen/app currently shown on the device.
  Stream<String> get status => _statusController.stream;

  /// Emits true/false when the device connects/disconnects.
  Stream<bool> get connectionState => _connectionController.stream;

  bool get isConnected => _device != null;

  /// Scans for a CodeBud device advertising [serviceUuid] and connects to it.
  /// Returns true on success.
  Future<bool> scanAndConnect({Duration timeout = const Duration(seconds: 10)}) async {
    final completer = Completer<bool>();
    late StreamSubscription<List<ScanResult>> sub;

    sub = FlutterBluePlus.scanResults.listen((results) async {
      for (final result in results) {
        if (result.advertisementData.serviceUuids.contains(serviceUuid)) {
          await FlutterBluePlus.stopScan();
          await sub.cancel();
          final ok = await _connect(result.device);
          if (!completer.isCompleted) completer.complete(ok);
          return;
        }
      }
    });

    await FlutterBluePlus.startScan(timeout: timeout, withServices: [serviceUuid]);

    Future.delayed(timeout, () async {
      if (!completer.isCompleted) {
        await sub.cancel();
        completer.complete(false);
      }
    });

    return completer.future;
  }

  Future<bool> _connect(BluetoothDevice device) async {
    try {
      await device.connect(timeout: const Duration(seconds: 10));
      _device = device;

      _connectionSub = device.connectionState.listen((state) {
        final connected = state == BluetoothConnectionState.connected;
        _connectionController.add(connected);
        if (!connected) _cleanup();
      });

      final services = await device.discoverServices();
      final service = services.firstWhere((s) => s.uuid == serviceUuid);

      for (final c in service.characteristics) {
        if (c.uuid == commandCharUuid) {
          _commandChar = c;
        } else if (c.uuid == statusCharUuid) {
          _statusChar = c;
          await c.setNotifyValue(true);
          _statusSub = c.lastValueStream.listen((value) {
            _statusController.add(utf8.decode(value, allowMalformed: true));
          });
          // Read the initial status.
          final initial = await c.read();
          _statusController.add(utf8.decode(initial, allowMalformed: true));
        }
      }

      _connectionController.add(true);
      return true;
    } catch (_) {
      await disconnect();
      return false;
    }
  }

  void _cleanup() {
    _statusSub?.cancel();
    _statusSub = null;
    _commandChar = null;
    _statusChar = null;
    _device = null;
  }

  Future<void> disconnect() async {
    await _connectionSub?.cancel();
    _connectionSub = null;
    await _device?.disconnect();
    _cleanup();
  }

  /// Asks the device to open the app with the given display name
  /// (e.g. "Clock", "Pong", "Messages") - matches `App.name` in the firmware.
  Future<void> openApp(String name) {
    return _send({'cmd': 'open', 'app': name});
  }

  /// Pushes a notification to the device's "Messages" app and opens it.
  Future<void> sendNotification(String title, String message) {
    return _send({'cmd': 'notify', 'title': title, 'msg': message});
  }

  Future<void> _send(Map<String, dynamic> payload) async {
    final char = _commandChar;
    if (char == null) throw StateError('Not connected to a CodeBud device');
    await char.write(utf8.encode(jsonEncode(payload)), withoutResponse: false);
  }
}
