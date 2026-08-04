#include <WiFi.h>
#include <Trioe.h>
#include <TRIOE_ROOT_CA.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* STATE_URL =
    "https://hub.trioe.dev/api/v1/devices/YOUR_DEVICE_ID/state/";
const char* API_KEY = "YOUR_DEVICE_API_KEY";

TrioeClient trioe(STATE_URL, API_KEY);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  trioe.setCACert(TRIOE_ROOT_CA);
  trioe.setStateEndpoint(STATE_URL);

  trioe.onReading("temperature", [](float value) {
    Serial.printf("Temperature: %.2f C\n", value);
  });
  trioe.onReading("humidity", [](float value) {
    Serial.printf("Humidity: %.2f %%\n", value);
  });
  trioe.onText("message", [](const char* value) {
    Serial.println(value);
  });
  trioe.onBoolean("pump", [](bool value) {
    Serial.println(value ? "Pump ON" : "Pump OFF");
  });

  trioe.setStatePollInterval(5000);
  if (!trioe.begin()) {
    Serial.println("TRIOE setup failed: check endpoint, API key, and CA certificate.");
  }
}

void loop() {
  trioe.loop();
}
