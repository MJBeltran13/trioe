# Smart Building Kit Beginner - Adjustable LED Blink

In this activity, you will learn to blink an LED with an adjustable blink rate using a potentiometer. This example code is for the Smart Building Learning Kit beginner module.

## Table of Contents

- [Introduction](#introduction)
- [Materials Needed](#materials-needed)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

This project demonstrates how to blink an LED with an adjustable rate using a potentiometer connected to a microcontroller. By adjusting the potentiometer, you can control the blink rate of the LED.

## Materials Needed

- 1 x LED
- 1 x Resistor (appropriate value for your LED)
- 1 x Potentiometer
- 1 x Microcontroller (e.g., Arduino, Raspberry Pi)
- Jumper wires
- Breadboard

## Installation

1. **Hardware Setup:**

   - Connect the anode (long leg) of the LED to digital pin 13 on the microcontroller.
   - Connect the cathode (short leg) of the LED to one end of the resistor.
   - Connect the other end of the resistor to the ground (GND) pin on the microcontroller.
   - Connect the middle pin of the potentiometer to the analog pin A0 on the microcontroller.
   - Connect the other two pins of the potentiometer to the 5V and GND pins on the microcontroller.

2. **Software Setup:**
   - If using an Arduino, download and install the [Arduino IDE](https://www.arduino.cc/en/software).
   - Connect your Arduino to your computer using a USB cable.

## Usage

1. **Arduino Code:**

   - Open the Arduino IDE.
   - Copy and paste the following code into the IDE:

     ```cpp
     /*
       Smart_Building_Kit_Beginner_Blink_Act_4
       Learn to to Blink

       This code will turn an LED with Adjustable Blink Rate Using Potentiometer

       connected to an analog pin

       This example code is for Smart Building Learning Kit beginner module.

       https://trioe.dev/smarthome%20beginner.html
     */
     // Global
     const int potPin = A0; // Pin where the potentiometer is connected
     const int ledPin = 13; // Pin where the LED is connected

     // the setup function runs once when you press reset or power the board
     void setup() {
       pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
       pinMode(potPin, INPUT);  // Initialize the potentiometer pin as an input
     }

     void loop() {
       int potValue = analogRead(potPin); // Read the potentiometer value
       int delayTime = map(potValue, 0, 1023, 100, 1000); // Map the potentiometer value to a delay time between 100ms and 1000ms

       digitalWrite(ledPin, HIGH); // Turn the LED on
       delay(delayTime); // Wait for the mapped delay time
       digitalWrite(ledPin, LOW); // Turn the LED off
       delay(delayTime); // Wait for the mapped delay time
     }
     ```

   - Select the correct board and port from the Tools menu.
   - Click the Upload button to upload the code to the Arduino.

2. **Running the Code:**
   - Once the code is uploaded, you can adjust the potentiometer to change the blink rate of the LED. The blink rate will vary between 100ms and 1000ms.

## Contributing

Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -m 'Add some feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Open a Pull Request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, feel free to open an issue or contact us at [your-email@example.com].
