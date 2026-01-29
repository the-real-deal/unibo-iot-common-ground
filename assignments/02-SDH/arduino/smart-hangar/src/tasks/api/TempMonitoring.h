#ifndef __TEMP_MONITOR__
#define __TEMP_MONITOR__

#include "devices/api/TempSensor.h"
#include "devices/api/Lcd.h"
#include "devices/api/Led.h"
#include "devices/api/Button.h"
#include "context/Context.h"
#include "kernel/Task.h"

class TempMonitoring: public Task{
public:
    TempMonitoring(Lcd* plcd, Led* pled, TempSensor* pTsensor, Context* pcontext, Button* pButton);
    ~TempMonitoring();
    void tick();

private:
    enum State{NORMAL_STATE, PREALARM, ALARM};

    State state;

    void setState(State newState);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);
    void updateDisplay();
    void monitor();

    Lcd* pLcd; 
    Led* pLed;
    Context* pContext;
    TempSensor* pTsensor;
    Button* pButton;
    long stateTimestamp;
    bool justEntered;
    unsigned long lastDisplayUpdate;
};

#endif