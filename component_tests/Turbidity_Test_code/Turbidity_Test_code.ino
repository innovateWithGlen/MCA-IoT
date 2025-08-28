// The GPIO pin where the turbidity sensor's analog output is connected
#define TURBIDITY_PIN 34

void setup() {
  Serial.begin(115200);
  Serial.println("Turbidity Sensor Test");
}

void loop() {
  int sensorValue = analogRead(TURBIDITY_PIN);
  Serial.print("Raw Turbidity Value: ");
  Serial.println(sensorValue); // Lower value = more turbid/cloudy

  delay(500);
}