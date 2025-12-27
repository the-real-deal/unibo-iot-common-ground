#include "api/TempSensor.h"
#include "config/Config.h"
#include "Arduino.h"

TempSensor::TempSensor(int pin) : pin(pin) {
  pinMode(pin, INPUT);
}

float TempSensor::getTemperature() const {
  float values[NUM_TEMPERATURE_SAMPLES];
  float max = MIN_TEMP;
  float min = MAX_TEMP;
  
  for (int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++) {
    const int adcValue = analogRead(pin);
    
    const float voltage = adcValue * (VCC / ADC_RESOLUTION);
    
    const float temperature = (voltage - TEMP_SENSOR_OFFSET) * TEMP_SENSOR_SCALE;
    
    values[i] = temperature;
    
    if (temperature < min) {
      min = temperature;
    }
    if (temperature > max) {
      max = temperature;
    }
  }
  
  float sum = 0.0f;
  int count = 0;
  
  for (int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++) {
    if (values[i] > min && values[i] < max) {
      sum += values[i];
      count++;
    }
  }
  
  if (count == 0) {
    return values[0];
  }
  
  return sum / static_cast<float>(count);
}