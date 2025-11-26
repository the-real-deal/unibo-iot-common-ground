#include "lib/LED/LEDUtilities.h"
#include "Arduino.h"

void fadeLed(const int pin, const int fadeAmount) 
{
    static int currentIntensity = MIN_LED_INTENSITY;
    static int fadeAmountAndDirection = fadeAmount;
    currentIntensity += fadeAmountAndDirection;
    if (currentIntensity <= MIN_LED_INTENSITY || currentIntensity >= MAX_LED_INTENSITY)
    {
        fadeAmountAndDirection = -fadeAmountAndDirection;
    }
    analogWrite(pin, currentIntensity);
    delay(50);
}

void fadeLed(const int pin) 
{
    const int DEFAULT_FADE_AMOUNT = 5; 
    fadeLed(pin, DEFAULT_FADE_AMOUNT);
}

void initLed(const int pin) 
{
    pinMode(pin, OUTPUT);
}

void initAllLeds(int *pins, const int pinsCount) 
{
    for (int i = 0; i < pinsCount; i++) 
    {
        initLed(pins[i]);
    }
}

void turnOn(int pin) 
{
    digitalWrite(pin, HIGH);
}

void turnOff(int pin) 
{
    digitalWrite(pin, LOW);
}

void turnOnFor(int pin, int seconds)
{
    turnOn(pin);
    delay(seconds * 1000);
    turnOff(pin);
}

void turnOffAllLeds(int *pins, const int pinsCount) 
{
    for (int i = 0; i < pinsCount; i++) 
    {
        turnOff(pins[i]);
    }
}