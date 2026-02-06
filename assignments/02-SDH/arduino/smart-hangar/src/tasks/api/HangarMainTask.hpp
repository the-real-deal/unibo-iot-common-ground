#ifndef __HANGAR_MAIN_TASK__
#define __HANGAR_MAIN_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "devices/api/Lcd.h"
#include "devices/api/Led.h"
#include "devices/api/Pir.h"
#include "devices/api/Sonar.h"
#include "devices/api/TempSensor.h"
#include "devices/config/Config.h"
#include <Arduino.h>

class HangarMainTask: public SyncTask
{
public:
    HangarMainTask(Lcd* pLcd, Led* pLed1, Pir* DPD, Sonar* DDD, TempSensor* pTempS, Context* pContext);
    void tick();
private:
    enum HangarStates {INSIDE, TAKING_OFF, OPERATING, LANDING} state;

    void setState(HangarStates newState);

    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Led* pLed1;
    Lcd* pLcd;
    Pir* DPD;
    Sonar* DDD;
    TempSensor* pTempS;
    Context* pContext;
};  


#endif