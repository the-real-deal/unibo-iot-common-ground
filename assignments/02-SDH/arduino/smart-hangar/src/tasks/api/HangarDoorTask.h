#ifndef __HANGAR_DOOR_TASK__
#define __HANGAR_DOOR_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "devices/api/Door.h"
#include "../../Config.hpp"
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

    bool justEntered;
};

#endif