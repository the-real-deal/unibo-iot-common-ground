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

 #include "tasks/api/TempMonitoring.h"

 #define TEMP_THRESHOLD_ONE     //this is the Temp1
 #define TEMP_THRESHOLD_TWO     //This is the Temp2
 #define TIME_THRESHOLD_ONE     //This is T3
 #define TIME_THRESHOLD_TWO     //This is T4
 #define DISPLAY_UPDATE_TIME 8000 

TempMonitoring::TempMonitoring(Lcd* plcd, Led* pled, TempSensor* pTsensor, Context* pcontext, Button* pButton){
    this->pLcd = pLcd;
    this->pLed = pLed;
    this->pTsensor = pTsensor;
    this->pContext = pContext;
    this->pButton= pButton;
    lastDisplayUpdate = 0;
    setState(NORMAL_STATE);
}

void TempMonitoring::tick(){
    updateDisplay();
    monitor();

    switch (state)
    {
        case NORMAL_STATE:
            
        break;

        case PREALARM:
            
        break;

        case ALARM:
            
        break;
    }
}

void TempMonitoring::setState(State newState){

}

long TempMonitoring::elapsedTimeInState(){

}

bool TempMonitoring::checkAndSetJustEntered(){

}

void TempMonitoring::log(const String& msg){

}

//Funzione che gestisce i compiti del display LCD. --> Non ne sono convinta perchè alarm non dovrebbe stare tra gli stati dell'hangar
//L'ho fatta durante un attimo di follia, spero vada...
void TempMonitoring::updateDisplay(){
    unsigned long currentTime = millis();

    if (currentTime - lastDisplayUpdate < DISPLAY_UPDATE_TIME) {
        return;
    }

    lastDisplayUpdate = currentTime;

    switch(pContext->getHangarSystemState()) {
        case Context::DRONE_INSIDE:
            pLcd->clear();
            pLcd->print("DRONE INSIDE", 0, 0);
            break;
            
        case Context::TAKE_OFF:
            pLcd->clear();
            pLcd->print("TAKE OFF", 0, 0);
            break;
            
        case Context::DRONE_OUT:
            pLcd->clear();
            pLcd->print("DRONE OUT", 0, 0);
            break;
            
        case Context::LANDING:
            pLcd->clear();
            pLcd->print("LANDING", 0, 0);
            break;
            
        case Context::ALARM:
            pLcd->clear();
            pLcd->print("ALARM", 0, 0);
            break;
    }
}

/**
 * 
 */
void TempMonitoring::monitor(){

}

/*

if (temp > TEMP_THRESHOLD_ONE || (millis() - currentTime) > TIME_THRESHOLD_ONE){
    pContext->setAlarmState(PREALARM);
    setState(PREALARM);
}
*/