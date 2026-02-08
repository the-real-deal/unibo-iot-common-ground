#include "tasks/api/TemperatureMonitoringTask.hpp"

TemperatureMonitoringTask::TemperatureMonitoringTask(Lcd* pLcd, Led* pLed, TempSensor* pTsensor, Button* pButton, Context* pContext) 
    : pLcd(pLcd), pLed(pLed), pTsensor(pTsensor), pResetButton(pButton), pContext(pContext), pTaskState(new StateHolder<TemperatureMonitoringTaskStates>(NORMAL_STATE))
{
    setState(NORMAL_STATE);
}

void TemperatureMonitoringTask::tick(){
    TemperatureMonitoringTaskStates currentTaskState = pTaskState->getState();
    switch (currentTaskState)
    {
        case NORMAL_STATE: {
            if (checkAndSetJustEntered()) {
                pContext->pHangarState->setState(Context::HangarStates::NORMAL);
                pLed->switchOff();
            }

            const Context::DroneStates droneState = pContext->pDroneState->getState();
            if (!(droneState == Context::DroneStates::REST ||
                droneState == Context::DroneStates::TAKING_OFF ||
                droneState == Context::DroneStates::LANDING)) {
                    return;
            }

            float temp = pTsensor->getTemperature();
            if (temp >= TEMP1 && elapsedTimeInState() >= T3){
                setState(PRE_ALARM);
            }
            break;
        }
        case PRE_ALARM: {
            if (checkAndSetJustEntered()) {
                pContext->pHangarState->setState(Context::HangarStates::PRE_ALARM);
            }

            float temp = pTsensor->getTemperature();
            if (temp < TEMP1) {
                setState(NORMAL_STATE);
                return;
            }

            if (temp >= TEMP2 && elapsedTimeInState() >= T4) {
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