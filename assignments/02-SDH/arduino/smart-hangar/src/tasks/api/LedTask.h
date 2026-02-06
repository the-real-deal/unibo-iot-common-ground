#ifndef __LED_TASK__
#define __LED_TASK__

#include "devices/api/Led.h"
#include "kernel/SyncTask.hpp"
#include "devices/config/Config.h"
#include "model/Context.hpp"
#include <Arduino.h>

class LedTask: public SyncTask {
public:
    LedTask(Led* l1, Led* l3,  Context* pContext);

    //TO DO: controllare se il distruttore può limitarsi a spegnere tutto
    ~LedTask();
    void tick();

private:
    enum ledState {IDLE, LED1, LED3};

    void setState(ledState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Context* pContext;
    Led* l1;
    Led* l3;
    ledState state;
    unsigned long stateTimestamp;
    bool justEntered;
};

#endif