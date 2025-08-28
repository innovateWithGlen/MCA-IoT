// Define the GPIO pin the IR sensor's OUT pin is connected to
#define IR_SENSOR_PIN 14

void setup() {
  // Start the Serial Monitor to display results
  Serial.begin(115200);
  
  // Set the sensor pin as an input
  pinMode(IR_SENSOR_PIN, INPUT);
  
  Serial.println("IR Sensor Test Initialized. Waiting for detection...");
}

void loop() {
  // Read the digital state of the sensor pin (HIGH or LOW)
  int sensorState = digitalRead(IR_SENSOR_PIN);
  
  // Most IR sensors output LOW when an object is detected
  if (sensorState == LOW) {
    Serial.println("Object Detected!");
  } else {
    Serial.println("Clear");
  }
  
  // Wait a short time before the next reading
  delay(100); 
}