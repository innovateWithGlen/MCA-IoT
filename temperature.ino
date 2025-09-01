#define BLYNK_TEMPLATE_ID "TMPL3dythRJpg"
#define BLYNK_TEMPLATE_NAME "DS18B20 TEMP SENSOR"
#define BLYNK_AUTH_TOKEN "5MlmBVMzyNO9GhcYyCQFHKPtpiTFniRu"

// Enables printing of debug information to the Serial Monitor
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

  // --- UPDATE THESE VALUES ---
  char auth[] = ""; // Your Blynk Auth Token
  char ssid[] = "";        // Your WiFi network name
  char pass[] = "";    // Your WiFi password
// --------------------------

// Define the GPIO pin where the DS18B20 data wire is connected
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Timer object for sending data periodically
BlynkTimer timer;

// This function sends the current temperature to Blynk
void sendSensorData() {
  // Request temperature from the sensor
  sensors.requestTemperatures(); 
  
  // Read temperature in Celsius
  float tempC = sensors.getTempCByIndex(0);

  // Check if the sensor reading is valid
  if (tempC != DEVICE_DISCONNECTED_C) {
    // Convert Celsius to Fahrenheit
    float tempF = (tempC * 9.0 / 5.0) + 32.0;

    // Print both values to the Serial Monitor for debugging
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print(" °C / ");
    Serial.print(tempF);
    Serial.println(" °F");

    // Send Celsius to Blynk Virtual Pin V5
    Blynk.virtualWrite(V0, tempC);
    
    // Send Fahrenheit to Blynk Virtual Pin V6
    Blynk.virtualWrite(V1, tempF);

  } else {
    Serial.println("Error: Could not read temperature data");
  }
}

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(115200);
  delay(1000);

  // Start up the DallasTemperature library
  sensors.begin();

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // Setup a timer to call sendSensorData() every 2 seconds (2000 milliseconds)
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  // Run Blynk and timer functions
  Blynk.run();
  timer.run();
}
