#include <ESP32Servo.h>

// The GPIO pin where the servo signal wire is connected
#define SERVO_PIN 22

Servo myServo; // Create a servo object

void setup() {
  Serial.begin(115200);
  Serial.println("SG90 Servo Motor Test");
  myServo.attach(SERVO_PIN); // Attach the servo to the pin
}

void loop() {
  Serial.println("Moving to 0 degrees");
  myServo.write(0);
  delay(1000);

  Serial.println("Moving to 90 degrees");
  myServo.write(90);
  delay(1000);

  Serial.println("Moving to 180 degrees");
  myServo.write(180);
  delay(1000);

  Serial.println("Moving to 190 degrees");
  myServo.write(190);
  delay(1000);

  Serial.println("Moving to 190 degrees");
  myServo.write(200);
  delay(1000);
}