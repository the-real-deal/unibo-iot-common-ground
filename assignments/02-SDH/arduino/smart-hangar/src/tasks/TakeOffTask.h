#ifndef __TAKEOFF_TASK__
#define __TAKEOFF_TASK__

#include "kernel/Task.h"
#include "context/Context.h"
#include "devices/api/Led.h"
#include "devices/api/Lcd.h"
#include "devices/api/Door.h"
#include "devices/api/Pir.h"
#include "devices/api/Sonar.h"
#include "kernel/Config.h"
#include "kernel/MsgService.h"
#include <Arduino.h>

class TakeOffTask: public Task{

public:
    TakeOffTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext, Sonar* pSonar);

    //TO DO: controllare se il distruttore può limitarsi a spegnere tutto
    ~TakeOffTask();
    void tick();

private:
    enum State { IDLE, OPENING_DOOR, DRONE_EXITING, CLOSING_DOOR, COMPLETED };
    State state;

    void setState(State newState);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);
    void updateDisplay();
    bool shouldStartTakeOff();

    Lcd* pLcd;
    Led* pLed;
    Door* pDoor;
    Pir* pDistance;
    Sonar* pSonar;

    Context* pContext;

    long stateTimestamp;
    bool justEntered;
    unsigned long lastDisplayUpdate;
};

#endif