/*
  Smart_Building_Kit_Beginner_Blink_Act_1
  Learn to to Blink

  This code will turn an LED on and off with a 1-second interval.

  This example code is for Smart Building Learning Kit beginner module.


  https://trioe.dev/smarthome%20beginner.html
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
