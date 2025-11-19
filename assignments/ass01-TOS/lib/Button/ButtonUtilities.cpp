#include <ButtonUtilities.h>
#include <Arduino.h>

int indexOfButton(int *pins, const int nrButtons, const int buttonPin) 
{
    for (int i = 0; i < nrButtons; i++) 
    {
        if (pins[i] == buttonPin) 
        {
            return i;
        }
    }

    return -1;
}

int detectButtonPressed(int *pins, const int nrButtons) 
{
    for (int i = 0; i < nrButtons; i++) 
    {
        const int value = digitalRead(pins[i]);
        if (value == HIGH) 
        {
            return pins[i];
        }
    }
    return -1;
}

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