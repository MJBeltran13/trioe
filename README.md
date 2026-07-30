# TRIOE

**Tinkering Resource for Internet Of Everything**

---

## Overview

TRIOE is a library designed to simplify IoT development using affordable learning kits. It's built for anyone looking to explore the Internet of Things with minimal barriers to entry.

---

## Purpose

### Who is it for?

TRIOE is for:

- **Hobbyists** experimenting with IoT projects.
- **Students** learning the basics of connected devices.
- **Developers** looking for a quick way to prototype IoT solutions.

### What does it do?

This library offers:

- Simplified integration with IoT devices.
- Compatibility with popular learning kits.
- Arduino programming language support for easy implementation.

---

## Dependencies

Make sure you have the following installed or available:

- **Arduino IDE** (latest version recommended).
- Supported TRIOE microcontroller boards (e.g., Trioe, ESP8266).
- Necessary libraries for sensors and actuators included in your kit.

---

## Learn More

For documentation, guides, and updates, visit **[trioe.dev](https://trioe.dev)**.

---

## Secure TRIOE Hub client

Version 3.2.6 adds `TrioeClient` without removing the existing `Trioe` API or
learning-kit examples. It supports:

- HTTPS certificate validation for `hub.trioe.dev`
- Batched number, integer, text, and boolean telemetry
- Reboot-safe delivery sessions and duplicate protection
- Exponential retry and a 16-reading offline queue
- Current-state polling with named typed callbacks
- Change thresholds and configurable reporting intervals

Install **ArduinoJson 6** before compiling the Hub examples.

### Publish telemetry

```cpp
#include <WiFi.h>
#include <Trioe.h>
#include <TRIOE_ROOT_CA.h>

const char* TELEMETRY_URL =
    "https://hub.trioe.dev/api/v1/devices/YOUR_DEVICE_ID/telemetry/";
const char* API_KEY = "YOUR_DEVICE_API_KEY";
TrioeClient trioe(TELEMETRY_URL, API_KEY);

void setup() {
  WiFi.begin("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
  trioe.setCACert(TRIOE_ROOT_CA);
  trioe.setReportingInterval(30000);
  trioe.setChangeThreshold(0.2f);
  trioe.begin();
}

void loop() {
  trioe.addReading("temperature", 25.5f, "C");
  trioe.addReading("pump", true);
  trioe.loop();
}
```

The complete publishing and state-fetch examples are in `examples/hub`.
API keys and Wi-Fi credentials must never be committed to source control.
