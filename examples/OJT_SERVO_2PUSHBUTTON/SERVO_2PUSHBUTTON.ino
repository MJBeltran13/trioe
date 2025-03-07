#include <ESP32Servo.h>

Servo myservo;

const int servoPin = 4;   // Servo control pin
const int upButton = 5;   // Button for UP movement
const int downButton = 41; // Button for DOWN movement

void setup() {
  myservo.attach(servoPin);
  pinMode(upButton, INPUT_PULLUP);   
  pinMode(downButton, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(upButton) == HIGH) { 
    myservo.write(90);  // Move servo to 90° (UP)
  }
  
  if (digitalRead(downButton) == HIGH) { 
    myservo.write(0);   // Move servo to 0° (DOWN)
  }
}