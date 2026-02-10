#include "tasks/api/BlinkingTask.hpp"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
    pTaskState(new StateHolder<BlinkingTaskStates>(OFF)),  pContext(pContext), pLed(pLed) {
    setState(OFF);
}

void BlinkingTask::tick(){
    BlinkingTaskStates currentTaskState = this->pTaskState->getState();
    switch (currentTaskState) {   
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
        }
        if (pContext->pSharedDroneState->getState() == Context::DroneStates::TAKING_OFF || 
            pContext->pSharedDroneState->getState() == Context::DroneStates::LANDING) {
            setState(ON);
        }
        break;
    }
    case ON: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOn();
        }
        setState(OFF);
        break;
    }
    }
}

void BlinkingTask::setState(const BlinkingTaskStates state) {
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}