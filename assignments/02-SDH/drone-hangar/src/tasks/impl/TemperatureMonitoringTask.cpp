#include "tasks/api/TemperatureMonitoringTask.hpp"

TemperatureMonitoringTask::TemperatureMonitoringTask(Lcd* pLcd, Led* pLed, TempSensor* pTsensor, Button* pButton, Context* pContext) 
    : pLcd(pLcd), pLed(pLed), pTsensor(pTsensor), pResetButton(pButton), pContext(pContext), pTaskState(new StateHolder<TemperatureMonitoringTaskStates>(NORMAL_STATE))
{
    setState(NORMAL_STATE);
}

void TemperatureMonitoringTask::tick(){
    static float counter = 0.0f;
    static long lastSampleTime = -1;
    TemperatureMonitoringTaskStates currentTaskState = pTaskState->getState();
    switch (currentTaskState)
    {
        case NORMAL_STATE: {
            if (checkAndSetJustEntered()) {
                pContext->pHangarState->setState(Context::HangarStates::NORMAL);
                pLed->switchOff();
            }
            const Context::DroneStates droneState = pContext->pDroneState->getState();
            if (droneState == Context::DroneStates::REST ||
                droneState == Context::DroneStates::TAKING_OFF ||
                droneState == Context::DroneStates::LANDING) {
                    counter = 0.0f;
                    setState(FIRST_SAMPLING);
            }
            break;
        }
        case FIRST_SAMPLING: {
            if (checkAndSetJustEntered()) {
                lastSampleTime = millis();
            }
                
            long now = millis();
            if (now - lastSampleTime < SAMPLING_INTERVAL) {
                return;
            }
            float temp = pTsensor->getTemperature();
            lastSampleTime = millis();
            if (temp > TEMP1) {
                counter += 1.0f;
            } else {
                counter = 0;
            }
            if (temp > TEMP1 && counter > T3) {
                counter = 0;
                pContext->pHangarState->setState(Context::HangarStates::PRE_ALARM);
                setState(SECOND_SAMPLING);
            }
            break;
        }
        case SECOND_SAMPLING: {
            if (checkAndSetJustEntered()) {
                lastSampleTime = millis();
            }
                
            long now = millis();
            if (now - lastSampleTime < SAMPLING_INTERVAL) {
                return;
            }
            float temp = pTsensor->getTemperature();
            lastSampleTime = millis();
            if (temp < TEMP1) {
                
            }
            if (temp > TEMP2) {
                counter += 1.0f;
            } else {
                counter = 0;
            }
            if (temp > TEMP2 && counter > T4) {
                pContext->pHangarState->setState(Context::HangarStates::PRE_ALARM);
                setState(SECOND_SAMPLING);
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

            Context::DroneStates droneState = pContext->pDroneState->getState();
            if (droneState == Context::DroneStates::OPERATING && shouldNotifyDrone) {
                Msg msg(MsgTopic::SDH, "ALARM");
                MsgService.sendMsg(msg.getFormattedMsg());
                shouldNotifyDrone = false;
            }

            if(pResetButton->isPressed()){
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