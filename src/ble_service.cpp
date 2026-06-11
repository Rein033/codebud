#include "ble_service.h"

#include <ArduinoJson.h>
#include <NimBLEDevice.h>

#include "apps/notify_app.h"
#include "launcher.h"

// BLE protocol (see README.md for details):
//
//   Service UUID:   a07498ca-ad5b-474e-940d-16f1fbe7e8cd
//   Command char:   51ff12bb-3ed8-46e5-b4f9-d64e2fec021b (write, JSON)
//     {"cmd":"open","app":"Pong"}
//     {"cmd":"notify","title":"Title","msg":"Message text"}
//   Status char:    51ff12bb-3ed8-46e5-b4f9-d64e2fec021c (read/notify)
//     plain text name of the currently active app/screen

namespace {
constexpr char kServiceUuid[] = "a07498ca-ad5b-474e-940d-16f1fbe7e8cd";
constexpr char kCommandCharUuid[] = "51ff12bb-3ed8-46e5-b4f9-d64e2fec021b";
constexpr char kStatusCharUuid[] = "51ff12bb-3ed8-46e5-b4f9-d64e2fec021c";

NimBLECharacteristic *status_char = nullptr;

struct PendingCommand {
    enum Type { NONE, OPEN_APP, NOTIFY } type = NONE;
    String app;
    String title;
    String message;
};

volatile bool command_pending = false;
PendingCommand pending;

class CommandCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *characteristic) override {
        std::string value = characteristic->getValue();

        StaticJsonDocument<256> doc;
        if (deserializeJson(doc, value) != DeserializationError::Ok) return;

        const char *cmd = doc["cmd"] | "";
        if (strcmp(cmd, "open") == 0) {
            pending.type = PendingCommand::OPEN_APP;
            pending.app = String(static_cast<const char *>(doc["app"] | ""));
            command_pending = true;
        } else if (strcmp(cmd, "notify") == 0) {
            pending.type = PendingCommand::NOTIFY;
            pending.title = String(static_cast<const char *>(doc["title"] | ""));
            pending.message = String(static_cast<const char *>(doc["msg"] | ""));
            command_pending = true;
        }
    }
};
}  // namespace

void ble_init() {
    NimBLEDevice::init("CodeBud");

    NimBLEServer *server = NimBLEDevice::createServer();
    NimBLEService *service = server->createService(kServiceUuid);

    NimBLECharacteristic *command_char =
        service->createCharacteristic(kCommandCharUuid, NIMBLE_PROPERTY::WRITE);
    command_char->setCallbacks(new CommandCallbacks());

    status_char = service->createCharacteristic(
        kStatusCharUuid, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    status_char->setValue("home");

    service->start();

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    advertising->addServiceUUID(kServiceUuid);
    advertising->start();
}

void ble_process() {
    if (!command_pending) return;
    command_pending = false;

    if (pending.type == PendingCommand::OPEN_APP) {
        launcher_open_app_by_name(pending.app.c_str());
    } else if (pending.type == PendingCommand::NOTIFY) {
        notify_app_set_message(pending.title.c_str(), pending.message.c_str());
        launcher_open_app_by_name(notify_app.name);
    }
    pending.type = PendingCommand::NONE;
}

void ble_set_status(const char *app_name) {
    if (!status_char) return;
    status_char->setValue(app_name);
    status_char->notify();
}
