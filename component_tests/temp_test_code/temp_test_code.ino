#include <OneWire.h>
#include <DallasTemperature.h>

// The GPIO pin where the DS18B20 data wire is connected
#define ONE_WIRE_BUS 4

// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println("DS18B20 Temperature Sensor Test");
  sensors.begin(); // Start up the library
}

void loop() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = sensors.getTempCByIndex(0); // Read temperature in Celsius

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
  } else {
    Serial.println("Error: Could not read temperature data. Check wiring!");
  }

  delay(3000); // Wait one second before reading again
}