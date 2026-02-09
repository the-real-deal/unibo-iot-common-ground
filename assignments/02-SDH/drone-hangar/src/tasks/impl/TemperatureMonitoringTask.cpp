#include "tasks/api/TemperatureMonitoringTask.hpp"

TemperatureMonitoringTask::TemperatureMonitoringTask(Lcd* pLcd, Led* pLed, TempSensor* pTsensor, Button* pButton, Context* pContext) 
    : pLcd(pLcd), pLed(pLed), pTsensor(pTsensor), pResetButton(pButton), pContext(pContext), pTaskState(new StateHolder<TemperatureMonitoringTaskStates>(NORMAL_STATE))
{
    setState(NORMAL_STATE);
}

void TemperatureMonitoringTask::tick(){
    static unsigned long lastSampleTime = millis();
    TemperatureMonitoringTaskStates currentTaskState = pTaskState->getState();
    switch (currentTaskState)
    {
        case NORMAL_STATE: {
            if (checkAndSetJustEntered()) {
                pContext->pSharedHangarState->setState(Context::HangarStates::NORMAL);
                pLed->switchOff();
            }
            const Context::DroneStates droneState = pContext->pSharedDroneState->getState();
            if (droneState != Context::DroneStates::OPERATING) {
                setState(FIRST_SAMPLING);
            }
            break;
        }
        case FIRST_SAMPLING: {
            if (checkAndSetJustEntered()) {
                lastSampleTime = millis();
            }
            float temp = pTsensor->getTemperature();
            if (temp <= TEMP1) {
                lastSampleTime = millis();
            }
            unsigned long now = millis();
            float elapsed = (now - lastSampleTime) * FROM_MS_TO_S;
            if (pContext->pSharedDroneState->getState() != Context::DroneStates::OPERATING && temp >= TEMP1 && elapsed >= T3) {
                pContext->pSharedHangarState->setState(Context::HangarStates::PRE_ALARM);
                setState(SECOND_SAMPLING);
                return;
            }
            if (pContext->pSharedDroneState->getState() == Context::DroneStates::OPERATING) {
                setState(NORMAL_STATE);
                return;
            }
            break;
        }
        case SECOND_SAMPLING: {
            if (checkAndSetJustEntered()) {
                lastSampleTime = millis();
            }
            float temp = pTsensor->getTemperature();
            if (temp < TEMP1) {
                setState(NORMAL_STATE);
                return;
            }
            if (temp <= TEMP2) {
                lastSampleTime = millis();
            }
            unsigned long now = millis();
            float elapsed = (now - lastSampleTime) * FROM_MS_TO_S;
            if (temp >= TEMP2 && elapsed >= T4) {
                pContext->pSharedHangarState->setState(Context::HangarStates::PRE_ALARM);
                setState(ALARM);
            }
            break;
        }
        case ALARM: {
            bool shouldNotifyDrone = false;
            if (checkAndSetJustEntered()) {
                shouldNotifyDrone = true;
                pLed->switchOn();
                this->pLcd->print("ALARM");
            }

            Context::DroneStates droneState = pContext->pSharedDroneState->getState();
            if (droneState == Context::DroneStates::OPERATING && shouldNotifyDrone) {
                Msg msg(MsgTopic::SDH, "ALARM");
                MsgService.sendMsg(msg.getFormattedMsg());
                shouldNotifyDrone = false;
            }

            if(pResetButton->isPressed()){
                pLcd->clear(true);
                setState(NORMAL_STATE);
                return;
            }
            break;
        }
    }
}

void TemperatureMonitoringTask::setState(TemperatureMonitoringTaskStates newState) {
    this->pTaskState->setState(newState);
    this->stateTimestamp = millis();
    this->justEntered = true;
}