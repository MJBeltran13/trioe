/*
  Smart_Building_Kit_Beginner_Blink_Act_3
  Learn to to Blink

  This code will turn an LED on and off of an External LEDs sequentially
  connected to pin 8, 9, 10

  This example code is for Smart Building Learning Kit beginner module.

  https://trioe.dev/smarthome%20beginner.html
*/
//Global
const int ledPins[] = {8, 9, 10}; // Array to hold LED pin numbers

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT); // Initialize each LED pin as an output
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], HIGH); // Turn the current LED on
    delay(500); // Wait for half a second
    digitalWrite(ledPins[i], LOW); // Turn the current LED off
  }
}
