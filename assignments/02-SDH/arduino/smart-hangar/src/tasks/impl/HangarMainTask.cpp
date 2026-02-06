#include "tasks/api/HangarMainTask.hpp";


HangarMainTask::HangarMainTask(Lcd* pLcd, Led* pLed1, Pir* DPD, Sonar* DDD, TempSensor* pTempS, Context* pContext):
    pLcd(pLcd),
    pLed1(pLed1),
    DPD(DPD),
    DDD(DDD),
    pContext(pContext){

        setState(INSIDE);
}

void HangarMainTask::tick(){
    HangarStates currentState = this->state;

    switch (currentState)
    {
    case HangarStates::INSIDE:
        if(checkAndSetJustEntered){
            pLed1->switchOn();
            pLcd->print("DRONE INSIDE");
            pContext->pDroneState->setState(Context::DroneStates::REST);
        }
        if(true && pContext->pHangarState->getState() == Context::HangarStates::NORMAL){ //serial command to do
            setState(TAKING_OFF);
            pContext->pDroneState->setState(Context::DroneStates::TAKING_OFF);
            pLcd->print("TAKING OFF");
            pLed1->switchOff();
        }
        else{
            //serial send KO
        }
        break;
    case HangarStates::TAKING_OFF:
        float currentTemp = pTempS->getTemperature();
        DDD->setTemperature(currentTemp);
        float dist = DDD->getDistance();
        if(pContext->pDoorState->isDoorOpen() && elapsedTimeInState()>=T1*FROM_S_TO_MS && isFarEnough(dist)){
            //serial send
            setState(OPERATING);
        }
        break;
        case HangarStates::OPERATING:
        if(checkAndSetJustEntered()){
            pContext->pDroneState->setState(Context::DroneStates::OPERATING);
            pLcd->print("DRONE OUT");
        }
        if(true && pContext->pHangarState->getState() == Context::HangarStates::NORMAL && DPD->isDetected()){ //serial command idk
            setState(LANDING);
            pContext->pDroneState->setState(Context::DroneStates::LANDING);
            pLcd->print("LANDING");
        }
        else{
            //serial send NO
        }
        break;
        case HangarStates::LANDING:
            //serial send landing + distance
            if(elapsedTimeInState()>=T2 *FROM_S_TO_MS && isNearEnough(dist) && pContext->pDoorState->isDoorOpen()){
                setState(INSIDE);
                //serial send
            }
        break;
    }
}


bool isFarEnough(float dist){
    return dist >= D1;
}

bool isNearEnough(float dist){
    return dist <= D2;
}