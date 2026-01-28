#ifndef __LANDING_TASK__
#define __LANDING_TASK__

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

class LandingTask: public Task{
public:
    LandingTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext, Pir* pPir, Sonar* pSonar);

    //TO DO: controllare se il distruttore può limitarsi a spegnere tutto
    ~LandingTask();
    void tick();

private:
    enum State{IDLE, OPENING_DOOR, WAITING_DRONE, DRONE_ENTERING, CLOSING_DOOR, COMPLETED};

    State state;

    void setState(State newState);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);
    void updateDisplay();
    bool shouldStartLanding();

    Lcd* pLcd; 
    Led* pLed;
    Door* pDoor;
    Pir* pPir;
    Sonar* pSonar;
    Context* pContext;
    long stateTimestamp;
    bool justEntered;
    unsigned long lastDisplayUpdate;
};

#endif