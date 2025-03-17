# BME280 Sensor 

This project reads temperature, humidity, and pressure using the BME280 sensor with an TRIOE board.

## Hardware Required
- **TRIOE Board**
- **BME280 Sensor**
- **Jumper Wires**
- **Female Headers**

## Circuit Connection
The BME280 sensor uses the I2C communication protocol. The following connections are required:

| BME280 Pin | TRIOE Pin |
|------------|-----------|
| VCC        | 3.3V      |
| GND        | GND       |
| SDA        | D4        |
| SCL        | D5        |

## Code Explanation
The provided code initializes the BME280 sensor and retrieves temperature, humidity, and pressure readings, displaying them via the serial monitor.

### Key Features:
- Uses the `Adafruit_BME280` library
- Reads data via I2C communication
- Displays sensor details upon initialization
- Prints temperature (*C), humidity (%), and pressure (hPa) every second

## Schematic

![Untitled (1920 x 1080 px) (2)](https://github.com/user-attachments/assets/1dbcd836-1ebb-44bd-a125-3a6d435eaf93)





