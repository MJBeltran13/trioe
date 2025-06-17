#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "ssid";  // Replace with your WiFi network name
const char* password = "password";      // Replace with your WiFi password

// LED pin
const int ledPin = 2;
bool ledState = false;

// Web server on port 80
WebServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize LED pin as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
  delay(10);
}

// Handle root URL
void handleRoot() {
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>TRIOE LED Control</title>"
                "<style>"
                "body {"
                "  font-family: Arial, sans-serif;"
                "  display: flex;"
                "  flex-direction: column;"
                "  justify-content: center;"
                "  align-items: center;"
                "  height: 100vh;"
                "  margin: 0;"
                "  background-color: #f4f4f4;"
                "}"
                "h1 {"
                "  color: #0066cc;"
                "  margin-bottom: 20px;"
                "}"
                ".button {"
                "  background-color: "
                + String(ledState ? "green" : "#0066cc") + ";"
                                                           "  color: white;"
                                                           "  padding: 15px 32px;"
                                                           "  text-align: center;"
                                                           "  display: inline-block;"
                                                           "  font-size: 16px;"
                                                           "  margin: 20px;"
                                                           "  cursor: pointer;"
                                                           "  border: none;"
                                                           "  border-radius: 8px;"
                                                           "}"
                                                           "</style>"
                                                           "</head>"
                                                           "<body>"
                                                           "<h1>Trioe-S3 LED Control</h1>"
                                                           "<p>LED is currently "
                + String(ledState ? "ON" : "OFF") + "</p>"
                                                    "<a href='/toggle'><button class='button'>"
                + String(ledState ? "Turn OFF" : "Turn ON") + "</button></a>"
                                                              "</body>"
                                                              "</html>";

  server.send(200, "text/html", html);
}

// Handle toggle request
void handleToggle() {
  ledState = !ledState;                // Toggle the LED state
  digitalWrite(ledPin, ledState);      // Update the LED
  server.sendHeader("Location", "/");  // Redirect back to the root page
  server.send(303);                    // HTTP response code for "See Other"
}

// Handle not found
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
