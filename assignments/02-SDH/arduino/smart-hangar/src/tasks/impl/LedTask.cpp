#include "./tasks/api/LedTask.h"
#include <Arduino.h>

LedTask::LedTask(Led* l1, Led* l3, Context* pContex){
    setState(IDLE);
}

LedTask::~LedTask() {
    if (l1 != NULL) {
        l1->switchOff();
    }
    if (l3 != NULL) {
        l3->switchOff();
    }
}

void LedTask::tick(){
    switch (state){
        case IDLE:
            if(pContext->getIsInit() && pContext->getHangarSystemState() == Context::DRONE_INSIDE){
                setState(LED1);
            } else {
                setState(LED3);
            }
        break;

        case LED1:
            l1->switchOn();
            if(!pContext->getIsInit()) {
                l1->switchOff();
                setState(IDLE);
            }
        break;

        case LED3:
            if(pContext->getHangarSystemState() == Context::ALARM){
                l3->switchOn();
            } else{
                l3->switchOff();
                setState(IDLE);
            }
        break;
    }
}

void LedTask::setState(ledState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long LedTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}