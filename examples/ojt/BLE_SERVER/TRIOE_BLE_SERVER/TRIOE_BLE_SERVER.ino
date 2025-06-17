#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>

#define LED_PIN 2  // Built-in LED on Trioe-S3

// BLE Service and Characteristic UUIDs
#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// BLE Server Connection Callback
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Client Connected");
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON when client connects
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Client Disconnected");
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF when client disconnects
    BLEDevice::startAdvertising();  // Restart advertising when disconnected
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue().c_str();
    Serial.println("Received Value: " + value);
    
    // Parse JSON data
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, value);
    if (error) {
      Serial.println("Failed to parse JSON!");
      return;
    }
    
    // Process the JSON data
    int rssi = doc["rssi"];
    const char* message = doc["message"];
    float temperature = doc["temperature"];
    
    Serial.print("RSSI: ");
    Serial.println(rssi);
    Serial.print("Message: ");
    Serial.println(message);
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Initialize LED to OFF

  // Create BLE Device
  BLEDevice::init("Trioe-S3_BLE_Server");

  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Start Service
  pService->start();

  // Start Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE Server Started. Waiting for Client...");
}

void loop() {
  // Server remains idle, waiting for data from client
}
