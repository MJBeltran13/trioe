# LED Blink Tutorial

In this activity, you will learn how to control multiple external LEDs by blinking them sequentially. The code will turn the LEDs connected to pins 8, 9, and 10 on and off in sequence.

## Table of Contents

- [Introduction](#introduction)
- [Materials Needed](#materials-needed)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

This project demonstrates how to control multiple external LEDs using a microcontroller. The LEDs will turn on and off sequentially with a 1-second interval, providing a basic understanding of controlling multiple digital outputs.

## Materials Needed

- 3 x LEDs
- 3 x Resistors (appropriate value for your LEDs)
- 1 x Microcontroller (e.g., Arduino, Raspberry Pi)
- Jumper wires
- Breadboard

## Installation

1. **Hardware Setup:**

   - Connect the anode (long leg) of the first LED to digital pin 8 on the microcontroller.
   - Connect the anode (long leg) of the second LED to digital pin 9 on the microcontroller.
   - Connect the anode (long leg) of the third LED to digital pin 10 on the microcontroller.
   - Connect the cathode (short leg) of each LED to one end of a resistor.
   - Connect the other end of each resistor to the ground (GND) pin on the microcontroller.

2. **Software Setup:**
   - If using an Arduino, download and install the [Arduino IDE](https://www.arduino.cc/en/software).
   - Connect your Arduino to your computer using a USB cable.

## Usage

1. **Arduino Code:**

   - Open the Arduino IDE.
   - Copy and paste the following code into the IDE:

     ```cpp
     // Pin numbers where the LEDs are connected
     const int ledPins[] = {8, 9, 10};

     void setup() {
       // Initialize the digital pins as outputs
       for (int i = 0; i < 3; i++) {
         pinMode(ledPins[i], OUTPUT);
       }
     }

     void loop() {
       // Turn each LED on and off in sequence
       for (int i = 0; i < 3; i++) {
         digitalWrite(ledPins[i], HIGH);  // Turn the LED on
         delay(1000);                     // Wait for a second
         digitalWrite(ledPins[i], LOW);   // Turn the LED off
         delay(1000);                     // Wait for a second
       }
     }
     ```

   - Select the correct board and port from the Tools menu.
   - Click the Upload button to upload the code to the Arduino.

2. **Running the Code:**
   - Once the code is uploaded, the LEDs should start blinking sequentially with a 1-second interval.

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
