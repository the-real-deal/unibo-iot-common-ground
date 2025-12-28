#include "kernel/Task.h"
#include "context/Context.h"
#include "devices/LCD"
#include <Arduino.h>

#ifndef __DISPLAYTASK__
#define __DISPLAYTASK__

class DisplayTask {
public:
    void diplay(Lcd* pLcd, Context* pContext);
private:
    Context* pContext;
    Lcd* pLcd;

};

#endif