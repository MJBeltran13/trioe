/*
  Smart_Building_Kit_Beginner_Blink_Act_2
  Learn to to Blink

  This code will turn an LED on and off of an External LED connected to pin 13.

  This example code is for Smart Building Learning Kit beginner module.

  https://trioe.dev/smarthome%20beginner.html
*/
//Global
const int ledPin = 13; // Pin where the LED is connected

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
}

void loop() {
  digitalWrite(ledPin, HIGH); // Turn the LED on
  delay(1000); // Wait for a second
  digitalWrite(ledPin, LOW); // Turn the LED off
  delay(1000); // Wait for a second
}
