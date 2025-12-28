#include "LedTask.h"
#include <Arduino.h>

void LedTask::LedTask(Led* l1, Led* l3, Context* pContex){
    setState(IDLE);
}

void LedTask::tick(){
    switch (state){
        case IDLE:
            if(pContext->isInit() && pContext->getHangarSystemState == DRONE_INSIDE){
                setState(LED1);
            } else {
                setState(LED3);
            }
        break;

        case LED1:
            l1->switchOn();
            if(!pContext->isInit()) {
                l1->switchOff();
                setState(IDLE);
            }
        break;

        case LED3:
            if(pContext->getHangarSystemState == ALARM){
                l3->switchOn();
            } else{
                l3->switchOff();
                setState(IDLE);
            }
        break;
    }
}


void LedTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long LedTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}