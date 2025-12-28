#include "devices/api/TempSensor.h"
#include "devices/config/Config.h"
#include "Arduino.h"

/*
 * TempSensor Constructor
 */
TempSensor::TempSensor(int pin) : pin(pin) {
  pinMode(pin, INPUT);
}

/*
 * Get the temperature from the sensor.
 */ 
float TempSensor::getTemperature() const {
  float values[NUM_TEMPERATURE_SAMPLES];
  float max = MIN_TEMP;
  float min = MAX_TEMP;
  
  for (int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++) {
    // Read ADC value from pin and convert to temp:
    const int adcValue = analogRead(pin);
    const float voltage = adcValue * (VCC / ADC_RESOLUTION);
    const float temperature = (voltage - TEMP_SENSOR_OFFSET) * TEMP_SENSOR_SCALE;
    values[i] = temperature;
    
    // Update of min and max temp
    if (temperature < min) {
      min = temperature;
    }
    if (temperature > max) {
      max = temperature;
    }
  }

  // Calculate average (excluding min and max)
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
  
  return sum / count;
}