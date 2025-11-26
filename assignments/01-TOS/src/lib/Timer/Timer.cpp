#include "Arduino.h"
#include "lib/Timer/Timer.h"

static unsigned long firstInstant;
static unsigned long timerPeriod;

void startTimer() 
{
    firstInstant = millis();
}

bool hasTimeElapsed() 
{
    return millis() - firstInstant >= timerPeriod; 
}

void setTimerPeriod(unsigned long period) 
{
    timerPeriod = period;
}

void resetTimer() 
{
    startTimer();
}