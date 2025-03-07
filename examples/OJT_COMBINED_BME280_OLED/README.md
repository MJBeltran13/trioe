# BME280 Sensor with OLED Screen

The provided code initializes the BME280 sensor, retrieves temperature, humidity, and pressure readings, and displays them on the OLED screen.

## Hardware Required
- **TRIOE Board**
- **TRIOE Board Slice**
- **BME280 Sensor**
- **OLED Screen**
- **Jumper Wires**
- **Female Headers**

## Circuit Connection
The BME280 sensor uses the I2C communication protocol. The following connections are required:

| BME280 Pin | TRIOE Pin |                    
|------------|-----------|
| VCC        | 5 V      |                            
| GND        | GND       |                        
| SCL        | D6        |          
| SDA        | D4        |


 OLED Screen Pin | TRIOE Pin |
|------------|-----------|
| VCC        | 5 V      |
| GND        | GND       |
| SCL        | D38        |
| SDA        | D40        |


## Code Explanation
The provided code initializes the BME280 sensor and retrieves temperature, humidity, and pressure readings, displaying them via the serial monitor.

### Key Features:
- Includes the following libraries:
- Adafruit BusIO
- WireData
- Adafruit_GFX.h
- Adafruit_SSD1327
- Adafruit_BME280
- Reads data via I2C communication
- Displays sensor details upon initialization
- Prints temperature (°C), humidity (%), and pressure (hPa) on the OLED screen along with corresponding icons every second


## Schematic

![18](https://github.com/user-attachments/assets/0beaaa90-93b0-409f-a8c0-a5f7748e8a1e)




