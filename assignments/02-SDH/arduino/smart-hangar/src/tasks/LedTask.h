#ifndef __LED__
#define __LED__

#include "devices/Led.h"
#include "kernel/Task.h"
#include "context/Context.h"
#include <Arduino.h>

class LedTask: public Task {
public:
    LedTask(Led* l1, Led* l3,  Context* pContext);
    void tick();

private:
    void setState(int state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Context* pContext;
    Led* l1;
    Led* l3;
    enum {IDLE, LED1, LED3} state;
};

#endif