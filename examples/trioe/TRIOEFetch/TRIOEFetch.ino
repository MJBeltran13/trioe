#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";
const char *serverURL = "http://198.177.125.118:51505/hub/api/devices/27/data/microcontroller/";
const char *apiKey = "YOUR_8_DIGIT_API_KEY"; // Get this from the API Keys section above (e.g., "A1B2C3D4")

// Function declarations
void connectToWiFi();
void fetchAndParseData();

void setup() {
    Serial.begin(115200);
    connectToWiFi();
    
    // Test network connectivity
    Serial.println("Testing network connectivity...");
    HTTPClient http;
    http.begin("http://httpbin.org/get"); // Simple test endpoint
    int testCode = http.GET();
    Serial.print("Test HTTP request result: ");
    Serial.println(testCode);
    http.end();
}

void connectToWiFi() {
    Serial.println("Starting ESP32...");
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    // Set connection timeout to avoid hanging if WiFi is unavailable
    unsigned long startAttemptTime = millis();
    
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
        delay(500);
        Serial.print(".");
    }
    
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect to WiFi. Please check your credentials and try again.");
        delay(3000);
        ESP.restart();
        return;
    }
    
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
}

void fetchAndParseData() {
    HTTPClient http;
    http.setTimeout(10000); // 10 second timeout
    
    Serial.println("Fetching data from server...");
    Serial.print("URL: ");
    Serial.println(serverURL);
    
    // Start connection and send HTTP header
    if (!http.begin(serverURL)) {
        Serial.println("Failed to begin HTTP connection");
        return;
    }
    
    http.setConnectTimeout(20000);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        
        if (httpResponseCode == HTTP_CODE_OK) {
            String response = http.getString();
            Serial.println("Raw Response:");
            Serial.println(response);
            
            // Use a StaticJsonDocument with enough size (adjust if needed)
            StaticJsonDocument<4096> doc;
            
            DeserializationError error = deserializeJson(doc, response);
            if (error) {
                Serial.print("JSON deserialization failed: ");
                Serial.println(error.c_str());
                return;
            }
            
            // Loop through the array to find specific data
            for (JsonObject obj : doc.as<JsonArray>()) {
                const char* name = obj["name"];
                const char* value = obj["current_value"];
                
                if (strcmp(name, "Temperature") == 0) {
                    Serial.print("Temperature: ");
                    Serial.println(value);
                }
                
                if (strcmp(name, "Humidity") == 0) {
                    Serial.print("Humidity: ");
                    Serial.println(value);
                }
                
                if (strcmp(name, "Pressure") == 0) {
                    Serial.print("Pressure: ");
                    Serial.println(value);
                }
                
                if (strcmp(name, "Altitude") == 0) {
                    Serial.print("Altitude: ");
                    Serial.println(value);
                }
                
                if (strcmp(name, "Angle") == 0) {
                    Serial.print("Angle: ");
                    Serial.println(value);
                }
            }
        }
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(httpResponseCode);
        
        if (httpResponseCode == -1) {
            Serial.println("Connection refused or timeout");
            Serial.println("Possible causes:");
            Serial.println("1. Server URL is incorrect");
            Serial.println("2. Server is not reachable");
            Serial.println("3. Network connectivity issues");
        }
    }
    
    http.end();
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        fetchAndParseData();
    } else {
        Serial.println("WiFi connection lost. Reconnecting...");
        connectToWiFi();
    }
    
    delay(5000); // Wait 5 seconds before next fetch
}