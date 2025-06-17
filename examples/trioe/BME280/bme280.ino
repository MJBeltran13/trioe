#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "Trioe.h"

// Constants
#define SEALEVELPRESSURE_HPA (1013.25)

// WiFi Credentials
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// Alternative network for testing (uncomment to use mobile hotspot)
// const char *ssid = "YourMobileHotspot";
// const char *password = "YourHotspotPassword";

const char *serverURL = "http://your-server-ip:port/hub/api/devices/DEVICE_ID/data/"; // Replace with your server URL
const char *apiKey = "YOUR_API_KEY"; // Get this from the API Keys section above (e.g., "A1B2C3D4")

// Sensor and Trioe objects
Adafruit_BME280 bme; // Create BME280 object
Trioe trioe;

// Sensor data variables
float temperature, pressure, altitude, humidity;

void setup()
{
    Serial.begin(115200);
    
    Serial.println("Starting Trioe...");
    Serial.println("Initializing BME280 sensor...");

    // Initialize BME280 sensor
    Wire.begin(4, 5);
    if (!bme.begin(0x76))
    { // Use correct I2C address
        Serial.println("Could not find BME280 sensor!");
        while (1)
            ;
    }
    Serial.println("BME280 sensor initialized!");

    // Connect to WiFi
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    randomSeed(analogRead(0));
}

void loop()
{
    BME_Read_data();
    send_data();
    delay(5000);
}

void BME_Read_data()
{
    // Read sensor values into global variables
    temperature = bme.readTemperature();
    pressure = bme.readPressure() / 100.0F;
    altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    humidity = bme.readHumidity();

    // Debug prints
    Serial.println("Sensor Readings:");
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println("°C");

    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" hPa");

    Serial.print("Altitude = ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println("%");
}

void send_data()
{
    // JSON payload - match Postman format exactly
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
}