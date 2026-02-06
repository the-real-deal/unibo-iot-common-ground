/**
 * Whenever the drone is inside the hangar (whether at rest, during take-off, or during landing), 
 * the temperature monitoring system is active to check for potential problems. 
 * If a temperature ≥ Temp1 is detected for more than T3 seconds, the system enters a pre-alarm state. 
 * In this state, new take-offs and landings are suspended until the system returns to normal operation. 
 * If a take-off or landing is already in progress, it is allowed to complete. 
 * If the temperature drops below Temp1, the system returns to normal operation. 
 * If a temperature Temp2 > Temp1 is detected for more than T4 seconds, 
 * the HD door is closed (if it was open), the L3 indicator light turns on, and the LCD displays ALARM. 
 * If the drone is outside the hangar, the ALARM message is also sent to the drone via DRU. 
 * All operations are suspended until the RESET button is pressed by an operator. 
 * When RESET is pressed, it is assumed that all issues have been resolved, and the system returns to the normal state.
 */

#include "tasks/api/TemperatureMonitoringTask.hpp"

// Constant declared RANDOMLY!!!
 #define TEMP1  30    
 #define TEMP2  5000  
 #define T3     10000l 
 #define T4     8000l 

TemperatureMonitoringTask::TemperatureMonitoringTask(Lcd* plcd, Led* pLed, TempSensor* pTsensor, Context* pContext, Button* pButton) 
    : pLcd(plcd), pLed(pLed), pTsensor(pTsensor), pResetButton(pButton), pContext(pContext)
{
    setState(NORMAL_STATE);
}

/**
 * The function contains the main logic of the task.
 *      - state = NORMAL_STATE: the system keeps monitoring the tempretature until it reaches a threshold (tem > TEMP_THRESHOLD_ONE) and 
 *          it stays that way for more than TIME_THRESHOLD_ONE seconds
 *      - state = PREALARM: the system blocks the possibility of starting new take off or landing, it keeps monitoring the temperature and if it rises 
 *          the system goes into ALARM otherwise it returns into NORMAL_STATE
 *      - state = ALARM: the state requires the intervention of a human to push the reset button to return in normal state
 * Don't know if I should keep a timeout timer: If a certain time elapses without changing state the system actually changes state
 */
void TemperatureMonitoringTask::tick(){
    switch (state)
    {
        case NORMAL_STATE:
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

        case PRE_ALARM:
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

        case ALARM:
            bool shouldNotifyDrone = false;
            if (checkAndSetJustEntered()) {
                shouldNotifyDrone = true;
                pLed->switchOn();
                // TODO: display("ALARM");
            }

            Context::DroneStates droneState = pContext->pDroneState->getState();
            if (droneState == Context::DroneStates::OPERATING && shouldNotifyDrone) {
                // TODO: Serial Comm "SH:ALARM"
                shouldNotifyDrone = false;
            }

            if(pResetButton->isPressed()){
                setState(NORMAL_STATE);
                return;
            }
        break;
    }
}

void TemperatureMonitoringTask::setState(TemperatureMonitoringTaskStates newState) {
    state = newState;
    stateTimestamp = millis();
    justEntered = true;
}

long TemperatureMonitoringTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool TemperatureMonitoringTask::checkAndSetJustEntered(){
    if (justEntered) {
        justEntered = false;
        return true;
    }
    return false;
}
