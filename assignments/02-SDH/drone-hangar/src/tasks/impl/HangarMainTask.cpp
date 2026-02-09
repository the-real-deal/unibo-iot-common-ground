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
    const HangarMainTaskStates currentTaskState = this->pTaskState->getState();
    static unsigned long lastSampleTime = millis();
    switch (currentTaskState)
    {
        case HangarMainTaskStates::INSIDE: {
            if(checkAndSetJustEntered()) {
                pLed->switchOn();
                pLcd->print("DRONE INSIDE", true);
                pContext->pSharedDroneState->setState(Context::DroneStates::REST);
            }
            Msg *pIncomingMsg = MsgService.receiveMsg();
            if (pIncomingMsg == NULL) {
                // if no message received, exit the function;
                return;
            }
            // for the sake of simplicity, let's assume that the incoming messages are relative to the drone
            // (in order to avoid checking for msg.getTopic() correspondance)
            if (pIncomingMsg->getContent() != "TAKING_OFF") {
                return;
            }
            if (pContext->pSharedHangarState->getState() == Context::HangarStates::NORMAL) {
                setState(TAKING_OFF);
                pContext->pSharedDroneState->setState(Context::DroneStates::TAKING_OFF);
                pLcd->print("TAKING OFF", true);
                pLed->switchOff();
            }
            else {
                // Taking off not allowed!
                Msg msg(MsgTopic::DRU, "TAKING_OFF", "KO");
                MsgService.sendMsg(msg.getFormattedMsg());
            }
            delete pIncomingMsg;
            break;
        }
        case HangarMainTaskStates::TAKING_OFF: {
            if (checkAndSetJustEntered()) {
                lastSampleTime = millis();
            }
            float currentTemp = pTempS->getTemperature();
            DDD->setTemperature(currentTemp);
            float dist = DDD->getDistance();
            if (!isFarEnough(dist)) {
                lastSampleTime = millis();
            }
            unsigned long now = millis();
            float elapsed = (now - lastSampleTime) * FROM_MS_TO_S;
            if (pContext->pSharedDoorState->isDoorOpen() && elapsed >= T1 && isFarEnough(dist)) {
                // Taking off succeded!
                Msg msg(MsgTopic::DRU, "TAKING_OFF", "OK");
                MsgService.sendMsg(msg.getFormattedMsg());
                setState(OPERATING);
            }
            break;
        }
        case HangarMainTaskStates::OPERATING: {
            if(checkAndSetJustEntered()) {
                pContext->pSharedDroneState->setState(Context::DroneStates::OPERATING);
                pLcd->print("DRONE OUT", true);
            }
            Msg *pIncomingMsg = MsgService.receiveMsg();
            if (pIncomingMsg == NULL) {
                // if no message received, exit the function;
                return;
            }
            // for the sake of simplicity, let's assume that the incoming messages are relative to the drone
            // (in order to avoid checking for msg.getTopic() correspondance)
            if (pIncomingMsg->getContent() != "LANDING") {
                return;
            }
            if (pContext->pSharedHangarState->getState() == Context::HangarStates::NORMAL && DPD->isDetected()) {
                setState(LANDING);
                pContext->pSharedDroneState->setState(Context::DroneStates::LANDING);
                pLcd->print("LANDING", true);
            }
            else {
                // Landing not allowed!
                Msg msg(MsgTopic::DRU, "LANDING", "KO");
                MsgService.sendMsg(msg.getFormattedMsg());
            }
            delete pIncomingMsg;
            break;
        }
        case HangarMainTaskStates::LANDING: {
            // Continuously send distance measured to DRU
            float currentTemp = pTempS->getTemperature();
            DDD->setTemperature(currentTemp);
            float dist = DDD->getDistance();
            Msg msg(MsgTopic::DRU, "LANDING", String(dist));
            MsgService.sendMsg(msg.getFormattedMsg());
            if (!isNearEnough(dist)) {
                lastSampleTime = millis();
            }
            unsigned long now = millis();
            float elapsed = (now - lastSampleTime) * FROM_MS_TO_S;
            if (elapsed >= T2 && isNearEnough(dist) && pContext->pSharedDoorState->isDoorOpen()) {
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
