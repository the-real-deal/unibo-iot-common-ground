#include "SweepingTask.h"
#include <Arduino.h>
#include "kernel/Logger.h"

#define FWD_TIME 2000
#define BWD_TIME 1000
#define START_TIME 100
#define RESET_TIME 500


SweepingTask::SweepingTask(Door* pMotor, Context* pContext, Pir* pDistance, Sonar* pSonar){
    this->pMotor = pMotor;
    this->pContext = pContext;
    this->pDistance = pDistance;
    this->pSonar = pSonar;
} 

  
void SweepingTask::tick(){
    switch (state){    
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] IDLE"));
        }
        if (pmessage->getContent() == "takeOff" || pmessage->getContent() == "landing"  
        || pDistance->isDetected()){
            setState(OPENING);
        }
        break;
    }
    case OPENING: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] OPENING"));
        }

        pMotor->open();

        if(pMotor->isOpen()){
            setState(CLOSING);
        }
        break;       
    }
    case CLOSING: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] CLOSING"));
        }

        pMotor->close();

        if(!pMotor->isOpen()){
            setState(IDLE);
        }
        break;       
    }
    }
}

void SweepingTask::setState(doorState s){
    doorState state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long SweepingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool SweepingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}