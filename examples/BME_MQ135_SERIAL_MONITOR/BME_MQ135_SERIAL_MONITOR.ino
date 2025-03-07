#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Define I2C pins for BME280
#define SDA_PIN 5
#define SCL_PIN 4

// Define MQ-135 pin (Analog input)
#define MQ135_PIN 1  // GPIO1 (ADC1_CH0)

// MQ-135 Constants (Datasheet-based values for CO2 detection)
#define RL 10.0  // Load resistance in kΩ (adjust if different)
#define R0_CLEAN_AIR 76.63 // This needs calibration in a known clean air environment

// Create a BME280 object
Adafruit_BME280 bme;

float R0 = R0_CLEAN_AIR;  // Initial R0 estimate (will be recalibrated)

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 & MQ-135 Sensor Test"));

    // Initialize I2C with custom pins
    Wire.begin(SDA_PIN, SCL_PIN);

    // Check if BME280 is detected
    if (!bme.begin(0x76)) { // 0x76 or 0x77 depending on your module
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    // Calibrate MQ-135 in clean air
    Serial.println("Calibrating MQ-135 sensor... Please ensure the sensor is in clean air.");
    delay(5000);
    R0 = calibrateMQ135();
    Serial.print("Calibration Complete. R0 = ");
    Serial.println(R0);
}

void loop() {
    // Read BME280 Data
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F; // Convert to hPa

    // Read MQ-135 Sensor Value (Analog)
    int mq135_value = analogRead(MQ135_PIN);
    float rs = getSensorResistance(mq135_value);
    float ppm = getPPM(rs);

    // Print Sensor Data
    Serial.println("---------- Sensor Readings ----------");
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    Serial.print("MQ-135 Raw Value: ");
    Serial.println(mq135_value);

    Serial.print("MQ-135 Resistance: ");
    Serial.print(rs);
    Serial.println(" kΩ");

    Serial.print("Estimated CO2 PPM: ");
    Serial.print(ppm);
    Serial.println(" ppm");

    Serial.println("-------------------------------------");

    delay(2000); // Delay for readability
}

/**
 * Function to calibrate the MQ-135 sensor in clean air.
 * This function assumes clean air contains ~400 PPM CO2.
 */
float calibrateMQ135() {
    float sum = 0;
    int samples = 50;

    for (int i = 0; i < samples; i++) {
        int adc_value = analogRead(MQ135_PIN);
        float rs = getSensorResistance(adc_value);
        sum += rs;
        delay(100);
    }
    float avg_rs = sum / samples;
    return avg_rs / (pow(400.0 / 116.6020682, (1 / -2.769034857))); // Calibration formula for CO2
}

/**
 * Function to compute the sensor resistance (Rs) based on ADC value.
 */
float getSensorResistance(int adc_value) {
    if (adc_value == 0) return 9999; // Avoid division by zero
    return ((1023.0 * RL) / adc_value) - RL;
}

/**
 * Function to estimate PPM using the MQ-135 equation.
 * Equation from datasheet: PPM = 116.6020682 * (Rs/R0)^-2.769034857
 */
float getPPM(float rs) {
    return 116.6020682 * pow((rs / R0), -2.769034857);
}
