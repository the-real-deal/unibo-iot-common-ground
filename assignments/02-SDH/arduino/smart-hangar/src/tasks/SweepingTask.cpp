#include "SweepingTask.h"
#include <Arduino.h>
#include "kernel/Logger.h"

#define FWD_TIME 2000
#define BWD_TIME 1000
#define START_TIME 100
#define RESET_TIME 500


SweepingTask::SweepingTask(ServoTimer2* pMotor, Context* pContext): 
    pMotor(pMotor), pContext(pContext){
    setState(IDLE);
}

void SweepingTask::tick(){
    switch (state){    
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] IDLE"));
        }
        break;
    }
    case SWEEPING_FWD: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] SWEEPING_FWD"));
        }
        
        long dt = elapsedTimeInState();
        currentPos = (int)(((float)dt / FWD_TIME) * 180);
        if (currentPos > 180) currentPos = 180; 
        pMotor->write(currentPos);

        if (dt >= FWD_TIME){
            setState(SWEEPING_BWD);
        }
        break;       
    }
    case SWEEPING_BWD: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] SWEEPING_BWD"));
        }

        long dt = elapsedTimeInState();
        currentPos = 180 - (int)(((float)dt / BWD_TIME) * 180);
        if (currentPos < 0) currentPos = 0;
        pMotor->write(currentPos);
        
        if (dt >= BWD_TIME){
            if (!toBeStopped){
                setState(SWEEPING_FWD);
            } else {
                setState(RESETTING);    
            }
        }
        break;       
    }
    case STARTING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] STARTING"));
        }
        if (elapsedTimeInState() > START_TIME){
            currentPos = 0;
            pMotor->write(currentPos);
            toBeStopped = false;
            setState(SWEEPING_FWD);
        }
        break;
    }
    case RESETTING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[SWT] RESETTING"));
        }
        pMotor->write(0);
        if (elapsedTimeInState() > RESET_TIME){
            setState(IDLE);
        }
        break;
    }
    }
}

void SweepingTask::setState(State s){
    state = s;
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