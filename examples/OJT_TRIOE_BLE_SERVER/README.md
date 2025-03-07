# BLE Client

## Overview
This project implements an TRIOE Board BLE Client that scans for a specific BLE Server, connects to it, and sends JSON-formatted data, including RSSI and temperature readings. 
The client communicates with the server via a predefined BLE service and characteristic UUID.

## Hardware Requirements
- TRIOE Board
  ![0001](https://github.com/user-attachments/assets/11ea821c-de2b-461b-b674-91809393be7c)
- USB Programming Cable
  ![Untitled (1920 x 1080 px)](https://github.com/user-attachments/assets/2044463f-6aaf-4db5-b7dc-63f54a27385b)

## Software Requirements
- Arduino IDE
- ESP32 Board Manager
- Required Libraries:
  - `BLEDevice.h`
  - `BLEUtils.h`
  - `BLEScan.h`
  - `BLEAdvertisedDevice.h`
  - `BLEClient.h`
  - `BLEAddress.h`
  - `BLECharacteristic.h`
  - `ArduinoJson.h`

## UUIDs
- **Service UUID**: `12345678-1234-5678-1234-56789abcdef0`
- **Characteristic UUID**: `abcdef01-1234-5678-1234-56789abcdef0`

## Features
- Scans for BLE devices with the specified Service UUID.
- Connects to the discovered BLE Server.
- Sends JSON-formatted data containing:
  - RSSI value
  - A message string
  - Temperature (set as 28.0°C for demonstration)
- LED (Pin 2) lights up when connected.
- Attempts to reconnect if the connection is lost.

## Code Explanation
### 1. BLE Scanning
- The client scans for BLE devices advertising the specified Service UUID.
- Once found, the scan stops, and the client attempts to connect.

### 2. Connecting to the Server
- Creates a BLE client and attempts to connect to the discovered server.
- If successful, the LED (Pin 2) turns on.

### 3. Sending Data
- JSON data is created with RSSI, a message, and temperature.
- The data is written to the BLE characteristic of the connected server.

### 4. Reconnection Handling
- If the connection is lost, the client attempts to reconnect automatically.
- The LED turns off when disconnected.

## Expected Serial Monitor Output
```
Found BLE Server!
Connecting to Server...
Connected!
RSSI: -45
JSON Data Sent.
ESP32 is not connected.
Reconnecting...
```

## Usage Instructions
1. Upload the BLE Server code to another TRIOE Board device.
2. Upload this BLE Client code to the TRIOE Board client device.
3. Open the Serial Monitor at `115200` baud rate.
4. Observe connection attempts, RSSI values, and JSON data transmission.

## Troubleshooting
- **Cannot find BLE Server?**
  - Ensure the server is powered on and advertising.
  - Verify the UUIDs in both client and server match.
- **Fails to connect?**
  - Restart the client and ensure Bluetooth is enabled.
- **Data not being sent?**
  - Check that the service and characteristic exist on the server.



