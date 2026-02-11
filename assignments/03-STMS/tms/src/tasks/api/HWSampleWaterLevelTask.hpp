#ifndef __SAMPLE_WATER_LEVEL_TASK__
#define __SAMPLE_WATER_LEVEL_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Sonar.hpp"
#include "Arduino.h"

class HWSampleWaterLevelTask : public SyncTask
{

public:
    HWSampleWaterLevelTask(Sonar* pSonar, Context* pContext);
    void tick();

private: 

    Sonar *pSonar;
    StateHolder<float> *pWaterLevel;
    Context *pContext;
};
#endif