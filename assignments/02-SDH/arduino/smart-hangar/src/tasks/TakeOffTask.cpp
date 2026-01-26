#include "TakeOffTask.h"
#include "kernel/Logger.h"
#define DELAY_TIME 1000

TakeOffTask::TakeOffTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext,  Pir* pDistance, Sonar* pSonar){
    this->pLcd = pLcd;
    this->pLed = pLed;
    this->pDoor = pDoor; 
    this->pContext = pContext;
    pSweepingTask = new SweepingTask(pDoor, pContext, pDistance, pSonar);
    pDisplayTask = new DisplayTask();
    //pLedTask = new LedTask();
    setState(IDLE);
}

void TakeOffTask::tick() {
    switch(state) {
        case IDLE: {
            if (checkAndSetJustEntered()) { // to log just one time when converts to a new State
                Logger.log(F("[TOT] IDLE - Witing to statr take off"));
            }

            if (pContext->getCanFly()) {
                setState(OPENING_DOOR);
            }
            break;
        }
        
        case OPENING_DOOR: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Opening door"));
                pDoor->open();
                pLed->switchOn();
            }
            if (elapsedTimeInState() > DELAY_TIME) { 
                pSweepingTask->init();
            }
            break;
        }
        
        case CLOSING_DOOR: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Closing door"));
                pDoor->close();
            }
            if (elapsedTimeInState() < DELAY_TIME){
                setState(COMPLETED);
                setCompleted(); // Discard task
            }
            break;
        }
    }
    pDisplayTask->display(pLcd,pContext); // print the context??? maybe just one time
}

void TakeOffTask::setState(State newState) {
    state = newState;
    stateTimestamp = millis();
    justEntered = true;
}

long TakeOffTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool TakeOffTask::checkAndSetJustEntered() {
    if (justEntered) {
        justEntered = false;
        return true;
    }
    return false;
}

void TakeOffTask::log(const String& msg) {
    Logger.log(msg);
}