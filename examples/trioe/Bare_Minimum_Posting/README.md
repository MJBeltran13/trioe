# Smart Building Kit Beginner - Adjustable LED Blink

In this activity, you will learn to blink an LED with an adjustable blink rate using a potentiometer. This example code is for the Smart Building Learning Kit beginner module.

## Table of Contents

- [Introduction](#introduction)
- [Materials Needed](#materials-needed)
- [Installation](#installation)
- [Usage](#usage)

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
   - First, you need to download the Trioe library. You can find it on GitHub or another repository that hosts it.
   - Install the Trioe library into your Arduino IDE. Here’s how you can do it:
     - Open your Arduino IDE.
     - Go to `Sketch -> Include Library -> Manage Libraries...`.
     - In the Library Manager, search for "Trioe" and install it.
   - After installing the library, you need to open the example code for Smart_Building_Kit_Beginner_Blink_Act_4:
     - Go to `File -> Examples -> Trioe -> Smart_Building_Kit_Beginner_Blink_Act_4`.
   - Connect your Arduino to your computer using a USB cable.

## Usage

1. **Arduino Code:**

   - Select the correct board and port from the Tools menu.
   - Click the Upload button to upload the code to the microcontroller.

2. **Running the Code:**
   - Once the code is uploaded, you can adjust the potentiometer to change the blink rate of the LED. The blink rate will vary between 100ms and 1000ms.
