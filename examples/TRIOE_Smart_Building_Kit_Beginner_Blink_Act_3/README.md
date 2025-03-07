# LED Blink Tutorial

In this activity, you will learn how to control multiple external LEDs by blinking them sequentially. The code will turn the LEDs connected to pins 8, 9, and 10 on and off in sequence.

## Table of Contents

- [Introduction](#introduction)
- [Materials Needed](#materials-needed)
- [Installation](#installation)
- [Usage](#usage)

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
   - First, you need to download the Trioe library. You can find it on GitHub or another repository that hosts it.
   - Install the Trioe library into your Arduino IDE. Here’s how you can do it:
     - Open your Arduino IDE.
     - Go to `Sketch -> Include Library -> Manage Libraries...`.
     - In the Library Manager, search for "Trioe" and install it.
   - After installing the library, you need to open the example code for Smart_Building_Kit_Beginner_Blink_Act_3:
     - Go to `File -> Examples -> Trioe -> Smart_Building_Kit_Beginner_Blink_Act_3`.
   - Connect your Arduino to your computer using a USB cable.

## Usage

1. **Arduino Code:**

   - Select the correct board and port from the Tools menu.
   - Click the Upload button to upload the code to the microcontroller.

2. **Running the Code:**
   - Once the code is uploaded, the LEDs should start blinking sequentially with a 1-second interval.
