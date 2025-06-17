#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; 
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting Trioe...");
  Serial.println("Initializing BME280 sensor...");

  Wire.begin(4, 5); // SDA = D4, SCL = D5

  if (!bme.begin(0x76, &Wire)) { 
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) delay(10);
  }
  
  Serial.println("BME280 sensor initialized!");
  
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
}

void loop() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  Serial.println("Sensor Readings:");
  Serial.print("Temperature = ");
  Serial.print(temp_event.temperature);
  Serial.println(" °C");

  Serial.print("Humidity = ");
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(1000);
}
