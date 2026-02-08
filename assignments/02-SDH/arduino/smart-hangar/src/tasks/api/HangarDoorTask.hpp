#ifndef __HANGAR_DOOR_TASK__
#define __HANGAR_DOOR_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Door.hpp"
#include "config.hpp"
#include "Arduino.h"

using HangarDoorTaskStates = enum HangarDoorTaskStates { CLOSED, OPENING_DOOR, OPEN, CLOSING_DOOR };
class HangarDoorTask: public SyncTask {

public:
    HangarDoorTask(Door* pDoor, Context* pContext);
    void tick();

private:
    void openDoor();
    void closeDoor();
    void setState(HangarDoorTaskStates newState);

    Door* pDoor;
    Context* pContext;
    StateHolder<HangarDoorTaskStates>* pTaskState;
};

#endif