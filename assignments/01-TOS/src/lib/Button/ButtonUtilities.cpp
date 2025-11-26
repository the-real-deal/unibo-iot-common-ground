#include "lib/Button/ButtonUtilities.h"
#include "Arduino.h"

void initButton(const int pin) 
{
    pinMode(pin, INPUT);
}

void initAllButtons(int *pins, const int pinsCount) 
{
    for (int i = 0; i < pinsCount; i++) 
    {
        initButton(pins[i]);
    }
}