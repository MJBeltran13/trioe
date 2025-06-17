#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>
#include <BLEAddress.h>
#include <BLECharacteristic.h>
#include <ArduinoJson.h>

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"
#define LED_PIN 2  // Built-in LED on Trioe-S3

BLEClient* pClient;
BLEAdvertisedDevice* myDevice;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(BLEUUID(SERVICE_UUID))) {
      Serial.println("Found BLE Server!");
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      advertisedDevice.getScan()->stop();
    }
  }
};

void connectToServer() {
  Serial.println("Connecting to Server...");
  pClient = BLEDevice::createClient();
  if (pClient->connect(myDevice)) {
    Serial.println("Connected!");
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON when connected
  } else {
    Serial.println("Failed to connect.");
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF if failed to connect
  }
}

void sendData() {
  if (pClient && pClient->isConnected()) {
    int rssi = myDevice->getRSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);

    // Create JSON data with RSSI and temperature (28°C)
    StaticJsonDocument<200> doc;
    doc["rssi"] = rssi;
    doc["message"] = "Hello from Trioe client";
    doc["temperature"] = 28.0;  // Adding temperature value

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Retrieve the service and characteristic
    BLERemoteService* pRemoteService = pClient->getService(BLEUUID(SERVICE_UUID));
    if (pRemoteService) {
      BLERemoteCharacteristic* pCharacteristic = pRemoteService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
      if (pCharacteristic) {
        // Write JSON data to the server
        pCharacteristic->writeValue(jsonBuffer, sizeof(jsonBuffer));
        Serial.println("JSON Data Sent.");
      } else {
        Serial.println("Failed to find characteristic.");
      }
    } else {
      Serial.println("Failed to find service.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Initialize LED to OFF

  BLEDevice::init("Trioe-S3_BLE_Client");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, false);

  if (myDevice) {
    connectToServer();
  }
}

void loop() {
  if (pClient && pClient->isConnected()) {
    sendData();
  } else {
    Serial.println("Trioe is not connected.");
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF when not connected
    if (myDevice) {
      connectToServer();  // Reconnect if not connected
    }
  }
  delay(1000);  // Send data every second
}
