#ifndef __LEDS_TASK__
#define __LEDS_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Led.hpp"
#include "Arduino.h"

enum LedsTaskStates
{
    OK,
    KO
};

class LedsTask : public SyncTask
{

public:
    LedsTask(Context* pContext, Led* pOkLed, Led* pKoLed);
    void tick();

private: 
    void setState(LedsTaskStates state);

    StateHolder<LedsTaskStates> *pTaskState;
    Led *pOkLed;
    Led *pKoLed;
    Context *pContext;
};
#endif