#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "Trioe.h"

// Constants
#define SEALEVELPRESSURE_HPA (1013.25)

// WiFi Credentials
const char *ssid = "SSID";
const char *password = "PASSWORD";
const char *serverURL = "https://thub123.vercel.app/api/devices/00001/data";

// Sensor and Trioe objects
Adafruit_BME280 bme; // Create BME280 object
Trioe trioe;

// Sensor data variables
float temperature, pressure, altitude, humidity;

void setup()
{
    Serial.begin(115200);

    // Initialize BME280 sensor
    Wire.begin(4, 5);
    if (!bme.begin(0x76))
    { // Use correct I2C address
        Serial.println("Could not find BME280 sensor!");
        while (1)
            ;
    }

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");
}

void loop()
{
    BME_Read_data();
    send_data();
}

void BME_Read_data()
{
    // Read sensor values into global variables
    temperature = bme.readTemperature();
    pressure = bme.readPressure() / 100.0F;
    altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    humidity = bme.readHumidity();

    // Debug prints
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println("*C");

    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println("hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(altitude);
    Serial.println("m");

    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println("%\n");
}

void send_data()
{
    // Create dynamic JSON payload with actual sensor values
    String jsonPayload = "{";
    jsonPayload += "\"streams\":[";
    jsonPayload += "{\"name\":\"Temperature\",\"type\":\"number\",\"value\":" + String(temperature) + ",\"unit\":\"celsius\"},";
    jsonPayload += "{\"name\":\"Pressure\",\"type\":\"number\",\"value\":" + String(pressure) + ",\"unit\":\"hPa\"},";
    jsonPayload += "{\"name\":\"Altitude\",\"type\":\"number\",\"value\":" + String(altitude) + ",\"unit\":\"m\"},";
    jsonPayload += "{\"name\":\"Humidity\",\"type\":\"number\",\"value\":" + String(humidity) + ",\"unit\":\"%\"}";
    jsonPayload += "]}";

    // Send data
    if (trioe.postData(serverURL, jsonPayload.c_str()))
    {
        Serial.println("Posted!");
    }
    else
    {
        Serial.println("Failed!");
    }

    delay(5000); // 5-second delay between transmissions
}