#include "tasks/api/HangarDoorTask.hpp"

HangarDoorTask::HangarDoorTask(Door* Door, Context* Context) : 
    pDoor(Door), pContext(Context), pTaskState(new StateHolder<HangarDoorTaskStates>(CLOSED)) {
    setState(CLOSED);
}

void HangarDoorTask::tick(){
    HangarDoorTaskStates currentTaskState = this->pTaskState->getState();
    switch (currentTaskState)
    {
    case HangarDoorTaskStates::CLOSED:
        pContext->pSharedDoorState->closeDoor();
        if((pContext->pSharedDroneState->getState() == Context::DroneStates::TAKING_OFF || pContext->pSharedDroneState->getState() == Context::DroneStates::LANDING) 
            && pContext->pSharedHangarState->getState() == Context::HangarStates::NORMAL) {
            setState(OPENING_DOOR);
        }
        break;
    case HangarDoorTaskStates::OPENING_DOOR:
        pDoor->open();
        if(pDoor->isOpen()){
            setState(OPEN);
        }
        break;
    case HangarDoorTaskStates::OPEN:
        pContext->pSharedDoorState->openDoor();
        if(pContext->pSharedDroneState->getState() == Context::DroneStates::OPERATING || 
            pContext->pSharedDroneState->getState() == Context::DroneStates::REST ||
            pContext->pSharedHangarState->getState() == Context::HangarStates::ALARM) {
            setState(CLOSING_DOOR);
        }
        break;
    case HangarDoorTaskStates::CLOSING_DOOR:
        pDoor->close();
        if(pDoor->isClosed()){
            setState(CLOSED);
        }
        break;
    }
}

void HangarDoorTask::setState(HangarDoorTaskStates newState)
{
    this->pTaskState->setState(newState);
    this->stateTimestamp = millis();
    this->justEntered = true;
}