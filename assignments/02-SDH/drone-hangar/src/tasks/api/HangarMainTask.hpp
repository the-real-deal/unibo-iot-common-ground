#ifndef __HANGAR_MAIN_TASK__
#define __HANGAR_MAIN_TASK__

#include "Arduino.h"
#include "kernel/SyncTask.hpp"
#include "kernel/MsgService.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Lcd.hpp"
#include "devices/api/Led.hpp"
#include "devices/api/Pir.hpp"
#include "devices/api/Sonar.hpp"
#include "devices/api/TempSensor.hpp"
#include "devices/config/config.hpp"
#include "config.hpp"

enum HangarMainTaskStates {INSIDE, TAKING_OFF, OPERATING, LANDING};

class HangarMainTask: public SyncTask
{
    public:
        HangarMainTask(Lcd* pLcd, Led* pLed, Pir* DPD, Sonar* DDD, TempSensor* pTempS, Context* pContext);
        void tick();
    private:
        void setState(HangarMainTaskStates newState);    
        const long SAMPLING_INTERVAL = 1L;
        
        Lcd* pLcd;
        Led* pLed;
        Pir* DPD;
        Sonar* DDD;
        TempSensor* pTempS;
        Context* pContext;
        StateHolder<HangarMainTaskStates>* pTaskState; 
};  


#endif