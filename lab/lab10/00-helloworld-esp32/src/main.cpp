/**
 * Board selected: esp32doit-devkit-v1
 * (check `platformio.ini` for info)
 * 
 * TODO: Something not working with serial monitor.
 */
#include <Arduino.h>
#define BAUD_RATE 115200

String message = "Hello, World!";

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(BAUD_RATE);
  int result = myFunction(2, 3);
}

void loop() {
  Serial.println(message);
  delay(200);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}