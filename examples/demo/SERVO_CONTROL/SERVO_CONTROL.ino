#include <TrioeServo.h>

#define POTENTIOMETER_PIN 1  // GPIO32 (ADC-capable)
#define SERVO_PIN 4           // GPIO4 (PWM-capable)

Servo myServo;  // Create Servo object

void setup() {
    myServo.attach(SERVO_PIN);  // Attach servo to pin GPIO4
    pinMode(POTENTIOMETER_PIN, INPUT);
}

void loop() {
    int potValue = analogRead(POTENTIOMETER_PIN);  // Read potentiometer (0-4095)
    int angle = map(potValue, 0, 4095, 0, 180);    // Convert to servo angle (0-180)

    myServo.write(angle);  // Move servo to the calculated angle
    delay(15);             // Small delay for stability
}
