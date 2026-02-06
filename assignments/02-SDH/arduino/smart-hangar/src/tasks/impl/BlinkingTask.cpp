#include "kernel/Logger.h"
#include "model/Context.hpp"
#include <Arduino.h>
#include "tasks/api/BlinkingTask.hpp"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
    pContext(pContext), pLed(pLed) {
    setState(OFF);
}

void BlinkingTask::tick(){
    switch (state){   
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
        }
        if (pContext->pDroneState->getState() == Context::DroneStates::TAKING_OFF || 
            pContext->pDroneState->getState() == Context::DroneStates::LANDING) {
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

long BlinkingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

void BlinkingTask::setState(const BlinkingTaskState state) {
    this->justEntered = true;
    this->state = state;
    this->stateTimestamp = millis();
}

bool BlinkingTask::checkAndSetJustEntered() {
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}