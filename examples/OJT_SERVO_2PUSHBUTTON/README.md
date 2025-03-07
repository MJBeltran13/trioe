# Servo Motor with Push Button 

This project is an automated car entrance system where pressing Button 1 moves the servo to 90 degrees, and pressing Button 2 moves it back to 0 degrees.

## Hardware Required
- **TRIOE Board**
- **TRIOE Board Slice**
- **Servo Motor**
- **Resistor**
- **Jumper Wires**
- **Female Headers**

## Circuit Connection
The Servo motor. The following connections are required:

| Servo Pin | TRIOE Pin |                    
|------------|-----------|
| VCC        | 5V      |                            
| GND        | GND       |                        
| SIGNAL PIN        | D4        |          

 Push Button | TRIOE Pin |
|------------|-----------|
| LEG 1        | 5V      |
| LEG 2       | GND  (via resistor)      |
| LEG 1 (jumper wire)        |  D2       |



## Code Explanation
The provided code controls a servo motor based on button inputs. When Button 1 is pressed, the servo rotates to 90 degrees. When Button 2 is pressed, it returns to 0 degrees.

### Key Features:
- Includes the following library:
- ESP32Servo
- Controls servo movement based on button inputs
- Enables precise angle adjustment for automated access control


## Schematic
![WHOLE 2](https://github.com/user-attachments/assets/1ac15882-b2f1-496a-baf7-2d3d63201132)





