#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Trioe.h"
#define LED_BUILTIN 2

const char *ssid = "<your-wifi-ssid>";
const char *password = "<your-wifi-password>";
const char *postURL = "http://198.177.125.118:51503/api/devices/<device_id>/data"; 
const char *getURL = "http://198.177.125.118:51503/api/devices/<device_id>/data/microcontroller";

Trioe trioe;

int buttonPressed = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected!");
  String jsonPayload = "{ \"streams\": [";
  jsonPayload += "{ \"name\": \"Button\", \"type\": \"number\", \"value\": " + String(buttonPressed) + ", \"unit\": \"binary\" }";
  jsonPayload += "] }";

  Serial.println("Sending JSON Payload:");
  Serial.println(jsonPayload);
  int responseCode = trioe.postData(postURL, jsonPayload.c_str());
  Serial.print("POST Response Code: ");
  Serial.println(responseCode);
}

void loop() {
 
  fetchAndParseData(); // Fetch LED control

  delay(500);
}

void fetchAndParseData() {
  HTTPClient http;
  http.setTimeout(10000);
  Serial.println("Fetching control data from server...");
  Serial.println(getURL);

  if (!http.begin(getURL)) {
    Serial.println("HTTP begin failed");
    return;
  }

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("GET Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Response: " + response);

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, response);
      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
      }

      // Parse the array of objects
      for (JsonObject obj : doc.as<JsonArray>()) {
        const char* name = obj["name"];
        const char* valueStr = obj["current_value"];
        int value = atoi(valueStr);  // Convert string to integer

        if (strcmp(name, "Button") == 0) {
          digitalWrite(LED_BUILTIN, value ? HIGH : LOW);
          Serial.println(value ? "LED ON" : "LED OFF");
          return; // No need to check more
        }
      }

      Serial.println("Button not found in data.");
    }
  } else {
    Serial.print("HTTP GET Failed, Error: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}