#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#include <time.h>

// WiFi and Blynk Credentials
char auth[] = "";
char ssid[] = "";
char pass[] = ""; 

// Pin Definitions
#define ONE_WIRE_BUS 4       // DS18B20 Temperature Sensor
#define TURBIDITY_PIN 34     // Turbidity Sensor
#define MQ135_PIN 35         // MQ-135 Gas/Ammonia Sensor
#define RELAY_PIN 23         // Relay for LED Lights
#define SERVO_PIN 22         // SG90 Servo for Feeder
#define IR_SENSOR_PIN 14     // IR Sensor for food level

// NTP Server for Time
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // GMT+5:30 for India
const int daylightOffset_sec = 0;

// Global objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Servo feederServo;
BlynkTimer timer;

bool lightState = false; // Keep track of light status

// --- BLYNK VIRTUAL PIN HANDLERS ---

// V0: Button to toggle lights manually
BLYNK_WRITE(V0) {
  lightState = param.asInt();
  digitalWrite(RELAY_PIN, lightState);
  Blynk.virtualWrite(V1, lightState); 
}

// V2: Button to feed the fish
BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    String logMsg = "Fish feeding sequence initiated...\n";
    Serial.print(logMsg);
    Blynk.virtualWrite(V20, logMsg);
    
    feederServo.write(90);
    delay(150);
    feederServo.write(0);

    logMsg = "Fish fed successfully.\n";
    Serial.print(logMsg);
    Blynk.virtualWrite(V20, logMsg);

    // After feeding, check the food level
    checkFoodLevel();
  }
}

// --- SENSOR READING FUNCTIONS ---

void checkFoodLevel() {
  int foodSensorState = digitalRead(IR_SENSOR_PIN);
  String logMsg;

  // IR sensor is LOW when it detects an object (food).
  // If it's HIGH, it means no food is detected below the sensor.
  if (foodSensorState == HIGH) {
    logMsg = "ALERT: Fish food is low!\n";
    // Trigger the "low_food_alert" event you created in the Blynk Console
    Blynk.logEvent("low_food_alert"); 
  } else {
    logMsg = "Food level is OK.\n";
  }
  Serial.print(logMsg);
  Blynk.virtualWrite(V20, logMsg);
}

void sendAllSensorData() {
  String logMsg;

  // 1. Read Temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    Blynk.virtualWrite(V10, tempC);
    logMsg = "Temp: " + String(tempC) + " C\n";
  } else {
    logMsg = "Error: Temp sensor failed!\n";
  }
  Serial.print(logMsg);
  Blynk.virtualWrite(V20, logMsg);

  // 2. Read Turbidity
  int turbidityValue = analogRead(TURBIDITY_PIN);
  Blynk.virtualWrite(V11, turbidityValue);
  logMsg = "Turbidity: " + String(turbidityValue) + "\n";
  Serial.print(logMsg);
  Blynk.virtualWrite(V20, logMsg);
  
  // 3. Read MQ-135
  int mq135Value = analogRead(MQ135_PIN);
  Blynk.virtualWrite(V12, mq135Value);
  logMsg = "Ammonia Level: " + String(mq135Value) + "\n";
  Serial.print(logMsg);
  Blynk.virtualWrite(V20, logMsg);

  // 4. Check Battery (Simulated)
  int batteryLevel = 85; 
  Blynk.virtualWrite(V13, batteryLevel);
  Blynk.virtualWrite(V14, "Charging");
  logMsg = "Battery: " + String(batteryLevel) + "% (Charging)\n";
  Serial.print(logMsg);
  Blynk.virtualWrite(V20, logMsg);
}

// --- TIME-BASED LOGIC ---

void checkTimeForLights() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    String logMsg = "Error: Failed to get NTP time.\n";
    Serial.print(logMsg);
    Blynk.virtualWrite(V20, logMsg);
    return;
  }

  // Turn on lights automatically after 6 PM (18:00)
  if (timeinfo.tm_hour >= 18 && !lightState) {
    String logMsg = "Lights turned on automatically.\n";
    Serial.print(logMsg);
    Blynk.virtualWrite(V20, logMsg);
    
    lightState = true;
    digitalWrite(RELAY_PIN, lightState);
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, HIGH);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  sensors.begin();
  feederServo.attach(SERVO_PIN);
  feederServo.write(0);

  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V20, "Device rebooted and connected.\n");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  timer.setInterval(1000L, sendAllSensorData);    // Update all sensors every 10 seconds
  timer.setInterval(60000L, checkTimeForLights);   // Check time every minute
  timer.setInterval(3600L, checkFoodLevel);     // Check food level every hour as a backup

  Blynk.syncVirtual(V0);
}

void loop() {
  Blynk.run();
  timer.run();
}
