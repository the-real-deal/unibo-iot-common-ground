#ifndef __TEMP_MONITOR__
#define __TEMP_MONITOR__

#include "devices/api/TempSensor.h"
#include "devices/api/Lcd.h"
#include "devices/api/Led.h"
#include "devices/api/Button.h"
#include "model/Context.hpp"
#include "kernel/SyncTask.hpp"

class TempMonitoring: public SyncTask {
public:
    TempMonitoring(Lcd* plcd, Led* pled, TempSensor* pTsensor, Context* pcontext, Button* pButton);
    void tick();

private:
    enum TempMonitoringState { NORMAL_STATE, PRE_ALARM, ALARM } state;

    void setState(TempMonitoringState newState);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Lcd* pLcd;
    Led* pLed;
    Context* pContext;
    TempSensor* pTsensor;
    Button* pResetButton;
    long stateTimestamp;
    bool justEntered;
};

#endif