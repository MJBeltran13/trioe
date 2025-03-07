# BLE Server

This project implements a **Bluetooth Low Energy (BLE) Server** using an **TRIOE BOARD** microcontroller. The server receives JSON data from a BLE client, parses the data, and processes values such as RSSI, temperature, and a message. It also turns on an LED when a client connects and turns it off when disconnected.

## Features
- Creates a BLE server with a **custom service and characteristic**
- Listens for connections from a BLE client
- Parses JSON data sent by the client (includes RSSI, temperature, and a message)
- Controls the built-in LED based on connection status
- Restarts advertising when a client disconnects

## Hardware Requirements
- TRIOE Board
  ![0001](https://github.com/user-attachments/assets/11ea821c-de2b-461b-b674-91809393be7c)
- USB Programming Cable
  ![Untitled (1920 x 1080 px)](https://github.com/user-attachments/assets/2044463f-6aaf-4db5-b7dc-63f54a27385b)

## BLE Configuration
- **Service UUID**: `12345678-1234-5678-1234-56789abcdef0`
- **Characteristic UUID**: `abcdef01-1234-5678-1234-56789abcdef0`

## Code Overview
### 1. BLE Server Setup
- Initializes BLE and creates a **BLE server**
- Defines a **service** and a **characteristic** for data communication
- Sets up **callbacks** to handle client connections and data reception
- Starts **BLE advertising** to allow clients to find and connect

### 2. Connection Handling
- **onConnect()**: Turns on the LED when a client connects
- **onDisconnect()**: Turns off the LED and restarts BLE advertising when a client disconnects

### 3. Data Handling
- Receives JSON data from the client
- Parses and extracts values for RSSI, temperature, and a message
- Prints received values to the serial monitor

## Installation & Usage
### 1. Install Dependencies
Make sure you have the following libraries installed in the **Arduino IDE**:
- `ArduinoJson`
- `ESP32 BLE Arduino`

### 2. Upload the Code
1. Connect your **TRIOE Board** to your computer via USB.
2. Open the **Arduino IDE** and select the correct board and port.
3. Upload the `TRIOE_BLE_Server` code.
4. Open the Serial Monitor (**115200 baud**) to view logs.

### 3. Connect a BLE Client
- Run the TRIOE BLE Client code on another TRIOE Board.
- The client will connect, send JSON data, and receive responses.

### 4. Monitor the Server Output
Once connected, observe the **Serial Monitor** for incoming data:
```
Client Connected
Received Value: {"rssi":-65,"message":"Hello from ESP32 client","temperature":28.0}
RSSI: -65
Message: Hello from ESP32 client
Temperature: 28.0°C
```

## Expected Behavior
| Event               | LED Status | Serial Output |
|--------------------|-----------|---------------|
| Server starts      | OFF       | "BLE Server Started. Waiting for Client..." |
| Client connects    | ON        | "Client Connected" |
| Client sends data  | ON        | Received JSON data with RSSI, message, and temperature |
| Client disconnects | OFF       | "Client Disconnected", advertising restarts |

## Troubleshooting
- **Client not connecting?**
  - Ensure both TRIOE Board devices are powered on and running.
  - Check UUID values in both client and server code.
  - Verify that BLE is enabled on the TRIOE Board.

- **No data received?**
  - Ensure the client is sending properly formatted JSON.
  - Check for parsing errors in the serial monitor.

