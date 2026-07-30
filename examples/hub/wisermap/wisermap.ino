#include <WiFi.h>
#include <Trioe.h>
#include <TRIOE_ROOT_CA.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* TELEMETRY_URL =
    "https://hub.trioe.dev/api/v1/devices/YOUR_DEVICE_ID/telemetry/";
const char* API_KEY = "YOUR_DEVICE_API_KEY";
TrioeClient trioe(TELEMETRY_URL, API_KEY);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  trioe.setCACert(TRIOE_ROOT_CA);
  trioe.setReportingInterval(30000);
  trioe.setChangeThreshold(0.2f);
  if (!trioe.begin()) {
    Serial.println("TRIOE setup failed: check endpoint, API key, and CA certificate.");
  }
}

void loop() {
  trioe.addReading("temperature", 25.5f, "C");
  trioe.addReading("humidity", 60.0f, "%");
  trioe.addReading("message", "Hello from TRIOE");
  trioe.addReading("pump", true);
  trioe.loop();
}
