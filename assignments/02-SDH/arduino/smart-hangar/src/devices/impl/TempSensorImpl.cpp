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
  float sum = 0.0f;
  float maxT = MIN_TEMP;
  float minT = MAX_TEMP;
  
  for (int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++) {
    // Read ADC value from pin and convert to temp:
    const int adcValue = analogRead(pin);
    const float voltage = adcValue * (VCC / ADC_RESOLUTION);
    const float temperature = (voltage - TEMP_SENSOR_OFFSET) * TEMP_SENSOR_SCALE;
    
    sum += temperature;
    
    // Update of min and max temp
    if (temperature < minT) {
      minT = temperature;
    }
    if (temperature > maxT) {
      maxT = temperature;
    }

    delay(1); 
  }

  // Calculate trimmed avarage (excluding min and max)
  

  
  if (NUM_TEMPERATURE_SAMPLES > 2) {
    return (sum - minT - maxT) / (NUM_TEMPERATURE_SAMPLES - 2); //trim and remove extremities
  } else {
    return sum / NUM_TEMPERATURE_SAMPLES;
  }
}