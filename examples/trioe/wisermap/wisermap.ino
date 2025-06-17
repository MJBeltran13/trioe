#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* server_url = "https://api-bucopi.parallaxed.ph/create-user";

WiFiClientSecure client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting ESP8266...");
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
}

void loop() {
  DynamicJsonDocument jsonBuffer(200);
  
  JsonObject values = jsonBuffer.to<JsonObject>();
  values["longitude"] = 13.795401313088945;
  values["latitude"] = 121.00048467851241;
  values["name"] = "name";
  values["address"] = "address";
  values["temp"] = 12.2;
  values["humidity"] = 12.2;
  values["airquality"] = 12.2;
  values["pressure"] = 12.2;
  values["altitude"] = 12.2;

  String jsonStr;
  serializeJson(values, jsonStr);

  Serial.println("Sending data:");
  Serial.println(jsonStr);

  client.setInsecure();

  http.begin(client, server_url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonStr);
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println("Data sent successfully!");
      Serial.print("Response: ");
      Serial.println(payload);
    }
  } else {
    Serial.println("Data send failed!");
    Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  
  delay(5000);
}