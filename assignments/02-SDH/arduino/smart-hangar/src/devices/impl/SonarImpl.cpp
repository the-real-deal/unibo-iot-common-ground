#include "devices/api/Sonar.h"
#include "Arduino.h"
#include "devices/config/Config.h"

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
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);
  
  const float tUS = pulseIn(echoPin, HIGH, timeOut);
  
  if (tUS == 0.0f) {
    return NO_OBJ_DETECTED;  
  }
  
  const float t = (tUS * FROM_US_TO_S) / 2.0f;  // time (in seconds) for one way:
  // The US starts from the sensor and comes back after "hitting" the object so it covers the distance twice.
  const float distance = t * getSoundSpeed(); //in meters!
  
  return distance;
}