#include <Arduino.h>

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Received: ");
    Serial.println(data);
  }
}
