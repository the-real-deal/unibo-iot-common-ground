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
    HangarDoorTask(Door* pDoor, Context* pContext);

    void tick();

private:
    enum HangarDoorTaskState { CLOSED, OPENING_DOOR, OPEN, CLOSING_DOOR } state;

    void openDoor();
    void closeDoor();
    void setState(HangarDoorTaskState newState);

    long elapsedTimeInState();
    bool checkAndSetJustEntered();

    Door* pDoor;
    Context* pContext;

    long stateTimestamp;
    bool justEntered;
};

#endif