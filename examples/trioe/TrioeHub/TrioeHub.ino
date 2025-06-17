#include <WiFi.h>
#include <HTTPClient.h>
#include "Trioe.h"

const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// Alternative network for testing (uncomment to use mobile hotspot)
// const char *ssid = "YourMobileHotspot";
// const char *password = "YourHotspotPassword";

const char *serverURL = "http://your-server-ip:port/hub/api/devices/DEVICE_ID/data/"; // Replace with your server URL
const char *apiKey = "YOUR_API_KEY"; // Get this from the API Keys section above (e.g., "A1B2C3D4")

Trioe trioe;

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting TRIOE...");
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  randomSeed(analogRead(0));
}

void loop() {
  // Randomized Sensor Values
  int temperature = random(20, 35);
  int pressure = random(990, 1025);
  int altitude = random(30, 100);
  int humidity = random(40, 80);

  // JSON payload
  String jsonPayload = "{\n  \"streams\": [\n";
  jsonPayload += "    {\n      \"name\": \"Temperature\",\n      \"type\": \"number\",\n      \"value\": " + String(temperature) + ",\n      \"unit\": \"celsius\"\n    },\n";
  jsonPayload += "    {\n      \"name\": \"Pressure\",\n      \"type\": \"number\",\n      \"value\": " + String(pressure) + ",\n      \"unit\": \"hPa\"\n    },\n";
  jsonPayload += "    {\n      \"name\": \"Altitude\",\n      \"type\": \"number\",\n      \"value\": " + String(altitude) + ",\n      \"unit\": \"m\"\n    },\n";
  jsonPayload += "    {\n      \"name\": \"Humidity\",\n      \"type\": \"number\",\n      \"value\": " + String(humidity) + ",\n      \"unit\": \"%\"\n    }\n";
  jsonPayload += "  ]\n}";

  Serial.println("Sending data with API Key:");
  Serial.println(jsonPayload);

  // Send Data with API Key
  int responseCode = trioe.postDataWithApiKey(serverURL, jsonPayload.c_str(), apiKey);
  
  if (responseCode == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Data send failed!");
    Serial.print("Response Code: ");
    Serial.println(responseCode);
  }

  delay(5000);
}