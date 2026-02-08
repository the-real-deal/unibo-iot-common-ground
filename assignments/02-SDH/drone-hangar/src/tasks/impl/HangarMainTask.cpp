#include "tasks/api/HangarMainTask.hpp"

HangarMainTask::HangarMainTask(Lcd* pLcd, Led* pLed, Pir* DPD, Sonar* DDD, TempSensor* pTempS, Context* pContext):
    pLcd(pLcd),
    pLed(pLed),
    DPD(DPD),
    DDD(DDD),
    pTempS(pTempS),
    pContext(pContext),
    pTaskState(new StateHolder<HangarMainTaskStates>(INSIDE)) {
    setState(INSIDE);
}

bool isFarEnough(float dist) {
    return dist >= D1;
}

bool isNearEnough(float dist) {
    return dist <= D2;
}

void HangarMainTask::tick() {
    const HangarMainTaskStates currentState = this->pTaskState->getState();
    switch (currentState)
    {
        case HangarMainTaskStates::INSIDE: {
            if(checkAndSetJustEntered()) {
                pLed->switchOn();
                pLcd->print("DRONE INSIDE");
                pContext->pDroneState->setState(Context::DroneStates::REST);
            }
            Msg *pIncomingMsg = MsgService.receiveMsg();
            if (pIncomingMsg == NULL) {
                // if no message received, exit the function;
                return;
            }
            // for the sake of simplicity, let's assume that the incoming messages are relative to the drone
            // (in order to avoid checking for msg.getTopic() correspondance)
            if(pIncomingMsg->getContent() == "TAKING_OFF" && pContext->pHangarState->getState() == Context::HangarStates::NORMAL) {
                setState(TAKING_OFF);
                pContext->pDroneState->setState(Context::DroneStates::TAKING_OFF);
                pLcd->print("TAKING OFF");
                pLed->switchOff();
            }
            else {
                // Taking off not allowed!
                Msg msg(MsgTopic::DRU, "TAKING_OFF", "KO");
                MsgService.sendMsg(msg.getFormattedMsg());
            }
            break;
        }
        case HangarMainTaskStates::TAKING_OFF: {
            float currentTemp = pTempS->getTemperature();
            DDD->setTemperature(currentTemp);
            float dist = DDD->getDistance();
            if(pContext->pDoorState->isDoorOpen() && elapsedTimeInState() >= T1*FROM_S_TO_MS && isFarEnough(dist)) {
                // Taking off succeded!
                Msg msg(MsgTopic::DRU, "TAKING_OFF", "OK");
                MsgService.sendMsg(msg.getFormattedMsg());
                setState(OPERATING);
            }
            break;
        }
        case HangarMainTaskStates::OPERATING: {
            if(checkAndSetJustEntered()) {
                pContext->pDroneState->setState(Context::DroneStates::OPERATING);
                pLcd->print("DRONE OUT");
            }
            Msg *pIncomingMsg = MsgService.receiveMsg();
            if (pIncomingMsg == NULL) {
                // if no message received, exit the function;
                return;
            }
            // for the sake of simplicity, let's assume that the incoming messages are relative to the drone
            // (in order to avoid checking for msg.getTopic() correspondance)
            if(pIncomingMsg->getContent() == "LANDING" && pContext->pHangarState->getState() == Context::HangarStates::NORMAL && DPD->isDetected()) {
                setState(LANDING);
                pContext->pDroneState->setState(Context::DroneStates::LANDING);
                pLcd->print("LANDING");
            }
            else {
                // Landing not allowed!
                Msg msg(MsgTopic::DRU, "LANDING", "KO");
                MsgService.sendMsg(msg.getFormattedMsg());
            }
            break;
        }
        case HangarMainTaskStates::LANDING: {
            // Continuously send distance measured to DRU
            float currentTemp = pTempS->getTemperature();
            DDD->setTemperature(currentTemp);
            float dist = DDD->getDistance();
            Msg msg(MsgTopic::DRU, "LANDING", String(dist));
            MsgService.sendMsg(msg.getFormattedMsg());
            if(elapsedTimeInState() >= T2 * FROM_S_TO_MS && isNearEnough(dist) && pContext->pDoorState->isDoorOpen()) {
                setState(INSIDE);
                Msg msg(MsgTopic::DRU, "LANDING", "OK");
                MsgService.sendMsg(msg.getFormattedMsg());
            }
            break;
        }
    }
}

void HangarMainTask::setState(HangarMainTaskStates newState)
{
    this->pTaskState->setState(newState);
    this->stateTimestamp = millis();
    this->justEntered = true;
}
