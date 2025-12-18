#include "api/TempSensor.h"
#include "Arduino.h"
#include "config/Config.h"

TempSensor::TempSensor(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);
}

float TempSensor::getTemperature(){
    float values[NUM_TEMPERATURE_SAMPLES];

    for(int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++){
        int value = analogRead(pin);
        float voltage = value * (VCC / ADC_RESOLUTION);
        float temperature = (voltage - TEMP_SENSOR_OFFSET) * TEMP_SENSOR_SCALE;
        values[i] = temperature;

        if (temperature < MIN_TEMP) {
            values[i] = MIN_TEMP;
        } else if (temperature > MAX_TEMP) {
            values[i] = MAX_TEMP;
        }
    }

    float sum = 0.0;
    float count = 0.0;
    for(int i = 0; i < NUM_TEMPERATURE_SAMPLES; i++){
        sum += values[i];
        count += 1.0;
    }

    if (count == 0.0) {
        return values[0];
    }else{
        return sum / count;
    }

}