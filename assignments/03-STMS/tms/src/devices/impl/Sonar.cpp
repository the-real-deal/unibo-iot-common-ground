#include "devices/api/Sonar.hpp"

/*
* Internal constants
*/
namespace {
  constexpr float SOUND_SPEED = 331.5f;      
  constexpr float SOUND_SPEED_TEMP_COEFF = 0.6f;  
}

/* 
* Sonar Constructor
*/
Sonar::Sonar(int echoP, int trigP, long maxTime)
  : echoPin(echoP), trigPin(trigP), timeOut(maxTime), temperature(20.0f) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW); 
}

/*
* Get distance measurement from the sonar sensor.
*/
void Sonar::setTemperature(float temp) {
  temperature = temp;
}

/*
* Calculate speed of sound based on current temperature.
*/
float Sonar::getSoundSpeed() const {
  return SOUND_SPEED + (SOUND_SPEED_TEMP_COEFF * temperature);
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