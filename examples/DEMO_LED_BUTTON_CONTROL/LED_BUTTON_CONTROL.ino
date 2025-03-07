#define BUTTON_PIN 1  // Button connected to D1
#define LED_PIN 5     // LED connected to D5

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Initially turn off LED

    Serial.begin(115200);  // Start serial communication
    Serial.println("System Initialized...");
}

void loop() {
    bool buttonState = digitalRead(BUTTON_PIN);  // Read button state

    if (buttonState == LOW) {  // Button pressed (LOW due to INPUT_PULLUP)
        digitalWrite(LED_PIN, HIGH);  // Turn LED ON
        Serial.println("Button Pressed: LED ON");
    } else {
        digitalWrite(LED_PIN, LOW);  // Turn LED OFF
        Serial.println("Button Released: LED OFF");
    }

    delay(50);  // Small delay to reduce serial spam
}
