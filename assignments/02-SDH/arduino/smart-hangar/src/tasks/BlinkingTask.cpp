#include "BlinkingTask.h"
#include "kernel/Logger.h"
#include "context/Context.h"
#include <Arduino.h>

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
    pContext(pContext), pLed(pLed){
    setState(IDLE);
}
  
void BlinkingTask::tick(){
    switch (state){   
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] IDLE"));

        }
        if (pContext->getHangarSystemState() != Context::HangarSystemState::TAKE_OFF || pContext->getHangarSystemState() != Context::HangarSystemState::LANDING){
            setState(OFF);
        }
        break;
    }
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] OFF"));
        }
        if (pContext->getHangarSystemState() != Context::HangarSystemState::TAKE_OFF || pContext->getHangarSystemState() != Context::HangarSystemState::LANDING){
            setState(IDLE);
        } else {
             setState(ON);
        }
        break;
    }
    case ON: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOn();
            Logger.log(F("[BT] ON"));
        }
        if (pContext->getHangarSystemState() != Context::HangarSystemState::TAKE_OFF || pContext->getHangarSystemState() != Context::HangarSystemState::LANDING){
            setState(IDLE);
        } else {
            setState(OFF);
        }
        break;
    }
    }
}

void BlinkingTask::setState(BlinkingTask::blinkingState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long BlinkingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool BlinkingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}