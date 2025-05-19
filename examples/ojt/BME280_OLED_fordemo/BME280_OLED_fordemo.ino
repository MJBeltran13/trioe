#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Adafruit_BME280.h>
#include <images.h>

#define SDA_OLED 14  
#define SCL_OLED 10
#define SCREEN_ADDRESS 0x3C
TwoWire I2C_OLED = TwoWire(0);
Adafruit_SSD1327 display(128, 128, &I2C_OLED, -1);

// BME280 Sensor Configuration on I2C bus 1
#define SDA_BME 4
#define SCL_BME 5
#define BME_ADDRESS 0x76
TwoWire I2C_BME = TwoWire(1);
Adafruit_BME280 bme;



void setup() {
  Serial.begin(9600);

  I2C_OLED.begin(SDA_OLED, SCL_OLED, 400000);
  I2C_BME.begin(SDA_BME, SCL_BME, 400000);

  if (!display.begin(SCREEN_ADDRESS)) {
    Serial.println("SSD1327 allocation failed.");
    while (1);
  }
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();
  delay(1000);

  if (!bme.begin(BME_ADDRESS, &I2C_BME)) {
    Serial.println("BME280 not found!");
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("BME280 Error!");
    display.display();
    while (1);
  }

  Serial.println("BME280 and OLED Initialized Successfully");
  display.clearDisplay();
}

void bme280() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme.getTemperatureSensor()->getEvent(&temp_event);
  bme.getPressureSensor()->getEvent(&pressure_event);
  bme.getHumiditySensor()->getEvent(&humidity_event);
  
  Serial.print("Temperature: "); Serial.println(temp_event.temperature);
  Serial.print("Humidity: "); Serial.println(humidity_event.relative_humidity);
  Serial.print("Pressure: "); Serial.println(pressure_event.pressure);
  
  // display.clearDisplay();
  
//  Temperature 
display.drawBitmap(11, 30, temperature, 16, 16, SSD1327_WHITE);
display.setCursor(33, 33);  
display.print(temp_event.temperature);
display.println(" C");

// Humidity 
display.drawBitmap(11, 57, humidity, 16, 16, SSD1327_WHITE);
display.setCursor(33, 59);  
display.print(humidity_event.relative_humidity);
display.println(" %");

// Pressure 
display.drawBitmap(11, 84, pressure, 16, 16, SSD1327_WHITE);
display.setCursor(33, 86);  
display.print(pressure_event.pressure);
display.println(" hPa");


  // display.display();
  // delay(1000);
}

void loop(){
display.clearDisplay();
delay(1000);
bme280();

display.drawBitmap(28, 3, wifi, 20, 13, SSD1327_WHITE);
display.drawBitmap(0, 0,Arduino_uiui3, 128, 128, SSD1327_WHITE);
display.display();

}
