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
        pContext->pDoorState->closeDoor();
        if((pContext->pDroneState->getState() == Context::DroneStates::TAKING_OFF || pContext->pDroneState->getState() == Context::DroneStates::LANDING) 
            && pContext->pHangarState->getState() == Context::HangarStates::NORMAL) {
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
        pContext->pDoorState->openDoor();
        if(pContext->pDroneState->getState() == Context::DroneStates::OPERATING || 
            pContext->pDroneState->getState() == Context::DroneStates::REST ||
            pContext->pHangarState->getState() == Context::HangarStates::ALARM) {
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