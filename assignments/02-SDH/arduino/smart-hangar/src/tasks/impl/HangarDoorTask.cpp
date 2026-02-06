#include "tasks/api/HangarDoorTask.h"

HangarDoorTask::HangarDoorTask(Door* Door, Context* Context) : pDoor(Door), pContext(Context){
    setState(CLOSED);
}

void HangarDoorTask::tick(){
    HangarDoorTaskState currentState = this->state;
    switch (currentState)
    {
    case HangarDoorTaskState::CLOSED:
        if(true && pContext->pHangarState->getState() == Context::HangarStates::NORMAL){ // true = to be serial nfretok
            setState(OPENING_DOOR);
        }
        break;
    case HangarDoorTaskState::OPEN:
        /* code */
        break;
    case HangarDoorTaskState::OPENING_DOOR:
        openDoor();
        if(pDoor->isOpen()){
            pContext->;
            setState(OPEN);
        }
        break;
    case HangarDoorTaskState::CLOSING_DOOR:
        closeDoor();
        if(!pDoor->isOpen()){
            
            Context::DoorState::closeDoor();
            setState(CLOSED);
        }
        break;
    default:
        break;
    }
}