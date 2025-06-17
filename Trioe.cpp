#include "Trioe.h"

Trioe::Trioe()
{
    // Constructor does nothing since no initialization is needed
}

bool Trioe::postData(const char *serverURL, const char *jsonPayload)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected");
        return false;
    }

    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);
    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
    return httpResponseCode > 0;
}

int Trioe::postDataWithApiKey(const char *serverURL, const char *jsonPayload, const char *apiKey)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected");
        return -1;
    }

    // Test basic connectivity first
    HTTPClient testHttp;
    testHttp.setTimeout(5000);
    Serial.println("🔍 Testing server connectivity...");
    
    // Extract just the base URL for testing
    String baseUrl = String(serverURL);
    int pathStart = baseUrl.indexOf("/hub");
    if (pathStart > 0) {
        baseUrl = baseUrl.substring(0, pathStart) + "/hub";
        testHttp.begin(baseUrl.c_str());
        int testResponse = testHttp.GET();
        Serial.print("🌐 Server health check: ");
        Serial.println(testResponse);
        testHttp.end();
    }

    HTTPClient http;
    
    // Enable following redirects
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    // Set timeout to prevent hanging
    http.setTimeout(10000); // 10 seconds
    
    // Add more debugging
    Serial.print("🌐 Connecting to: ");
    Serial.println(serverURL);
    
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    
    // Create Authorization header with Bearer format
    String authHeader = "Bearer ";
    authHeader += apiKey;
    http.addHeader("Authorization", authHeader);
    
    Serial.println("📡 Sending POST request...");
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0)
    {
        Serial.print("✅ HTTP Response code: ");
        Serial.println(httpResponseCode);
        
        // Print response body for debugging
        String response = http.getString();
        if (response.length() > 0) {
            Serial.print("📄 Response: ");
            Serial.println(response);
        }
    }
    else
    {
        Serial.print("❌ Error code: ");
        Serial.println(httpResponseCode);
        
        // Print more detailed error information
        if (httpResponseCode == HTTPC_ERROR_CONNECTION_REFUSED) {
            Serial.println("🔍 Connection refused - check server URL and port");
        } else if (httpResponseCode == HTTPC_ERROR_CONNECTION_LOST) {
            Serial.println("🔍 Connection lost - network issue");
        } else if (httpResponseCode == HTTPC_ERROR_NO_HTTP_SERVER) {
            Serial.println("🔍 No HTTP server found at URL");
        } else {
            Serial.println("🔍 Network connection error - check URL and connectivity");
        }
    }

    http.end();
    return httpResponseCode;
} 