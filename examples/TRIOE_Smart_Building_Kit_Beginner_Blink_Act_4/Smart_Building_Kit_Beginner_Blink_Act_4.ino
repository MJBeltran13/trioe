/*
  Smart_Building_Kit_Beginner_Blink_Act_4
  Learn to to Blink

  This code will turn an LED with Adjustable Blink Rate Using Potentiometer

  connected to an analog pin

  This example code is for Smart Building Learning Kit beginner module.

  https://trioe.dev/smarthome%20beginner.html
*/
//Global
const int potPin = A0; // Pin where the potentiometer is connected
const int ledPin = 13; // Pin where the LED is connected

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
  pinMode(potPin, INPUT); // Initialize the potentiometer pin as an input
}

void loop() {
  int potValue = analogRead(potPin); // Read the potentiometer value
  int delayTime = map(potValue, 0, 1023, 100, 1000); // Map the potentiometer value to a delay time between 100ms and 1000ms

  digitalWrite(ledPin, HIGH); // Turn the LED on
  delay(delayTime); // Wait for the mapped delay time
  digitalWrite(ledPin, LOW); // Turn the LED off
  delay(delayTime); // Wait for the mapped delay time
}
