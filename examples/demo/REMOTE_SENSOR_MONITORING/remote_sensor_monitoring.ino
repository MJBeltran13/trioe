#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Adafruit_BME280.h>
#include "Trioe.h"

// OLED setup
#define SDA_OLED 14
#define SCL_OLED 10
#define SCREEN_ADDRESS 0x3C
TwoWire I2C_OLED = TwoWire(0);
Adafruit_SSD1327 display(128, 128, &I2C_OLED, -1);

// BME280 setup
#define SDA_BME 4
#define SCL_BME 5
#define BME_ADDRESS 0x76
TwoWire I2C_BME = TwoWire(1);
Adafruit_BME280 bme;

// WiFi and server config
const char *ssid = "<your-wifi-ssid>";
const char *password = "<your-wifi-password>";
const char *serverURL = "http://198.177.125.118:51503/api/devices/<sensor_id>/data";

Trioe trioe;

void setup() {
  Serial.begin(115200);

  // Start I2C
  I2C_OLED.begin(SDA_OLED, SCL_OLED, 400000);
  I2C_BME.begin(SDA_BME, SCL_BME, 400000);

  // OLED init
  if (!display.begin(SCREEN_ADDRESS)) {
    Serial.println("SSD1327 OLED init failed.");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();

  // BME280 init
  if (!bme.begin(BME_ADDRESS, &I2C_BME)) {
    Serial.println("BME280 not found!");
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("BME280 Error!");
    display.display();
    while (1);
  }

  // WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi connected!");
  display.display();
  delay(1000);
}

void loop() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme.getTemperatureSensor()->getEvent(&temp_event);
  bme.getPressureSensor()->getEvent(&pressure_event);
  bme.getHumiditySensor()->getEvent(&humidity_event);

  float temp = temp_event.temperature;
  float humid = humidity_event.relative_humidity;
  float press = pressure_event.pressure;

  // Display to OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 30);
  display.print("Humidity: ");
  display.print(humid);
  display.println(" %");

  display.setCursor(0, 50);
  display.print("Pressure: ");
  display.print(press);
  display.println(" hPa");
  display.display();

  // Construct JSON payload
  String jsonPayload = "{ \"streams\": [";
  jsonPayload += "{ \"name\": \"Temperature\", \"type\": \"number\", \"value\": " + String(temp, 2) + ", \"unit\": \"celsius\" },";
  jsonPayload += "{ \"name\": \"Humidity\", \"type\": \"number\", \"value\": " + String(humid, 2) + ", \"unit\": \"%\" },";
  jsonPayload += "{ \"name\": \"Pressure\", \"type\": \"number\", \"value\": " + String(press, 2) + ", \"unit\": \"hPa\" }";
  jsonPayload += "] }";

  Serial.println("Sending payload:");
  Serial.println(jsonPayload);

  // Post to TRIOE server
  int responseCode = trioe.postData(serverURL, jsonPayload.c_str());
  Serial.print("Server response: ");
  Serial.println(responseCode);

  delay(5000);
}