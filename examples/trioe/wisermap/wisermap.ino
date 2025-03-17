#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "BatStateU ICT";
const char* password = "Leading Innovations, Transforming Lives";
const char* server_url = "https://api-bucopi.parallaxed.ph/create-user"; // Server endpoint for creating user

// Set up the client object
WiFiClientSecure client; // WiFiClientSecure for HTTPS
HTTPClient http;

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  DynamicJsonDocument jsonBuffer(200);
  
  JsonObject values = jsonBuffer.to<JsonObject>();
  values["longitude"] = 13.795401313088945;
  values["latitude"] = 121.00048467851241;
  values["name"] = "MJ Bauan";
  values["address"] = "Bauan, Batangas";
  values["temp"] = 12.2;
  values["humidity"] = 12.2;
  values["airquality"] = 12.2;
  values["pressure"] = 12.2;
  values["altitude"] = 12.2;

  // Serialize JSON to string
  String jsonStr;
  serializeJson(values, jsonStr);

  client.setInsecure(); // Ignore SSL certificate verification

  http.begin(client, server_url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonStr);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.print("Response: ");
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  
  delay(5000);
}