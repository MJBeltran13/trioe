#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MQ135.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
#define MQ135_PIN A0
MQ135 gasSensor = MQ135(MQ135_PIN);

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

float temperature = 25.0;
float humidity = 60.0;
float globalCorrectedPPM = 0.0;

WiFiClientSecure client;
HTTPClient http;

const char* server_url = "https://api-bucopi.parallaxed.ph/create-user";

void setup() {
  Serial.begin(115200);

  Serial.println("Starting ESP8266...");
  Serial.println("Initializing BME280 sensor...");

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  Serial.println("BME280 sensor initialized!");

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
  bme280();
  airquality();
  sending_to_wiser();
}

void bme280() {
  Serial.println("Sensor Readings:");
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println("°C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  delay(1000);
}

void airquality() {
  float rzero = gasSensor.getRZero();
  float correctedRZero = gasSensor.getCorrectedRZero(temperature, humidity);
  float resistance = gasSensor.getResistance();
  float ppm = gasSensor.getPPM();
  float correctedPPM = gasSensor.getCorrectedPPM(temperature, humidity);

  Serial.println("Air Quality Readings:");
  Serial.print("Sensor Resistance: ");
  Serial.print(resistance);
  Serial.println(" Ohms");

  Serial.print("RZero: ");
  Serial.print(rzero);
  Serial.println(" ppm");

  Serial.print("Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.println(" ppm");

  Serial.print("PPM: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  Serial.print("Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println(" ppm");
  globalCorrectedPPM = correctedPPM;
}

void sending_to_wiser() {
  if (WiFi.status() == WL_CONNECTED) {
    DynamicJsonDocument jsonBuffer(200);

    JsonObject values = jsonBuffer.to<JsonObject>();
    values["longitude"] = 13.795401313088945;
    values["latitude"] = 121.00048467851241;
    values["name"] = "MJ Bauan";
    values["address"] = "Bauan, Batangas";
    values["temp"] = bme.readTemperature();
    values["humidity"] = bme.readHumidity();
    values["airquality"] = globalCorrectedPPM;
    values["pressure"] = bme.readPressure() / 100.0F;
    values["altitude"] = bme.readAltitude(SEALEVELPRESSURE_HPA);

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
}
