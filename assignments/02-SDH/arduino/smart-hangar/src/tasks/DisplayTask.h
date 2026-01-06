#include "kernel/Task.h"
#include "context/Context.h"
#include "devices/api/Lcd.h"
#include <Arduino.h>

#ifndef __DISPLAYTASK__
#define __DISPLAYTASK__

class DisplayTask {
public:
    void display(Lcd* pLcd, Context* pContext);
private:
    Context* pContext;
    Lcd* pLcd;
};

#endif