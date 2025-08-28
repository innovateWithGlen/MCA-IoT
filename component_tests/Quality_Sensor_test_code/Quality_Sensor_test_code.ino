// The GPIO pin where the MQ-135 sensor's analog output is connected
#define MQ135_PIN 35

void setup() {
  Serial.begin(115200);
  Serial.println("MQ-135 Air Quality Sensor Test");
  Serial.println("Allowing sensor to warm up for a minute...");
  delay(60000); // Wait 1 minute for sensor to stabilize
}

void loop() {
  int sensorValue = analogRead(MQ135_PIN);
  Serial.print("Raw Air Quality Value: ");
  Serial.println(sensorValue);

  delay(1000);
}