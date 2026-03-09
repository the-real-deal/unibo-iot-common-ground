#include "devices/api/Sonar.hpp"

/* 
* Sonar Constructor
*/
Sonar::Sonar(int echoP, int trigP)
  : echoPin(echoP), trigPin(trigP) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW); 
}

/*
* Get distance measurement from the sonar sensor.
*/
float Sonar::getDistance() {
  pinMode(trigPin, OUTPUT);  // Clear the trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723 * pulseIn(echoPin, HIGH);
}