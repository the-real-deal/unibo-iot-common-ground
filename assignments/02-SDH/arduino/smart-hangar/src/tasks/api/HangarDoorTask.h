#ifndef __HANGAR_DOOR_TASK__
#define __HANGAR_DOOR_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "devices/api/Led.h"
#include "devices/api/Lcd.h"
#include "devices/api/Door.h"
#include "devices/api/Pir.h"
#include "devices/api/Sonar.h"
#include "../../Config.hpp"
#include "kernel/MsgService.h"
#include <Arduino.h>

class HangarDoorTask: public SyncTask{

public:
    HangarDoorTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext, Sonar* pSonar);

    void tick();

private:
    enum State { CLOSED, OPENING_DOOR, OPEN, CLOSING_DOOR };
    State state;

    void openDoor();
    void closeDoor();
    void setState(State newState);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();

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