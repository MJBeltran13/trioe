#include <WiFi.h>
#include "Trioe.h"

const char *ssid = "your_SSID", *password = "your_PASSWORD", *serverURL = "http://localhost:3000/api/devices/00001/data";
Trioe trioe;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(1000), Serial.println("Connecting...");
  Serial.println("WiFi connected!");
}

void loop()
{
  if (trioe.postData(serverURL, R"({"streams":[{"name":"Temperature","type":"number","value":27,"unit":"celsius"}]}")) 
    Serial.println("Posted!"); 
  else 
    Serial.println("Failed!");
  delay(3000);
}
