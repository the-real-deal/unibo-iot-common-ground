#ifndef __LED__
#define __LED__

#include "devices/api/Led.h"
#include "kernel/Task.h"
#include "kernel/Config.h"
#include "context/Context.h"
#include <Arduino.h>

class LedTask: public Task {
public:
    LedTask(Led* l1, Led* l3,  Context* pContext);
    void tick();

private:
    enum ledState {IDLE, LED1, LED3};

    void setState(int state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Context* pContext;
    Led* l1 = new Led(LED1_PIN);
    Led* l3;
    ledState state;
};

#endif