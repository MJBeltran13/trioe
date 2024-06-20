# LED Blink Tutorial

In this activity, you will learn how to control an external LED by blinking it. The code will turn the LED connected to pin 13 on and off.

## Table of Contents

- [Introduction](#introduction)
- [Materials Needed](#materials-needed)
- [Installation](#installation)
- [Usage](#usage)

## Introduction

This project demonstrates how to control an external LED using a microcontroller. The LED will turn on and off with a 1-second interval, providing a basic understanding of controlling digital outputs.

## Materials Needed

- 1 x LED
- 1 x Resistor (appropriate value for your LED)
- 1 x TRIOE Microcontroller
- Jumper wires
- Breadboard

## Installation

1. **Hardware Setup:**

   - Connect the anode (long leg) of the LED to digital pin 13 on the microcontroller.
   - Connect the cathode (short leg) of the LED to one end of the resistor.
   - Connect the other end of the resistor to the ground (GND) pin on the microcontroller.

2. **Software Setup:**
   - First, you need to download the Trioe library. You can find it on GitHub or another repository that hosts it.
   - Install the Trioe library into your Arduino IDE. Here’s how you can do it:
     - Open your Arduino IDE.
     - Go to `Sketch -> Include Library -> Manage Libraries...`.
     - In the Library Manager, search for "Trioe" and install it.
   - After installing the library, you need to open the example code for Smart_Building_Kit_Beginner_Blink_Act_2:
     - Go to `File -> Examples -> Trioe -> Smart_Building_Kit_Beginner_Blink_Act_2`.
   - Connect your Arduino to your computer using a USB cable.

## Usage

1. **Arduino Code:**

   - Select the correct board and port from the Tools menu.
   - Click the Upload button to upload the code to the microcontroller.

2. **Running the Code:**
   - Once the code is uploaded, the external LED should start blinking with a 1-second interval.
