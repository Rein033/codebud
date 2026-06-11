#include "ble_service.h"

#include <NimBLEDevice.h>
#include <ArduinoJson.h>

#include "launcher.h"
#include "apps.h"

#define SERVICE_UUID      "a07498ca-ad5b-474e-940d-16f1fbe7e8cd"
#define COMMAND_CHAR_UUID "51ff12bb-3ed8-46e5-b4f9-d64e2fec021b"
#define STATUS_CHAR_UUID  "51ff12bb-3ed8-46e5-b4f9-d64e2fec021c"

static NimBLECharacteristic *statusChar = nullptr;
static volatile bool hasPendingCommand = false;
static String pendingCommand;

class CommandCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *characteristic) override {
    pendingCommand = characteristic->getValue();
    hasPendingCommand = true;
  }
};

void bleInit() {
  NimBLEDevice::init("CodeBud");

  NimBLEServer *server = NimBLEDevice::createServer();
  NimBLEService *service = server->createService(SERVICE_UUID);

  NimBLECharacteristic *cmdChar = service->createCharacteristic(
      COMMAND_CHAR_UUID, NIMBLE_PROPERTY::WRITE);
  cmdChar->setCallbacks(new CommandCallbacks());

  statusChar = service->createCharacteristic(
      STATUS_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  statusChar->setValue("home");

  service->start();

  NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->start();
}

void bleProcess() {
  if (!hasPendingCommand) return;
  hasPendingCommand = false;

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, pendingCommand) != DeserializationError::Ok) return;

  const char *cmd = doc["cmd"];
  if (!cmd) return;

  if (strcmp(cmd, "open") == 0) {
    const char *appName = doc["app"];
    if (appName) launcherOpenAppByName(appName);
  } else if (strcmp(cmd, "notify") == 0) {
    const char *title = doc["title"] | "";
    const char *msg = doc["msg"] | "";
    notifySetMessage(title, msg);
    launcherOpenAppByName("Messages");
  }
}

void bleSetStatus(const char *appName) {
  if (!statusChar) return;
  statusChar->setValue(appName);
  statusChar->notify();
}
