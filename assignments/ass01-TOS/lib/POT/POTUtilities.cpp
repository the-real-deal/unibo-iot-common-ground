#include "POTUtilities.h"
#include <Arduino.h>
#include <math.h>


int readDifficultyFromPOT(int POT_PIN) 
{
    const int potValue = analogRead(POT_PIN);
    return floor(potValue / SCALE);
}

bool hasDifficultyChanged(int previousDifficulty, int currentDifficulty) 
{
    return previousDifficulty != currentDifficulty;
}


