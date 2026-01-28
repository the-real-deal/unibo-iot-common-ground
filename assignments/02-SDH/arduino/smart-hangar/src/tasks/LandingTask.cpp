/**
 * Landing phase: When the drone approaches the hangar, it sends the opening command (via DRU). 
 * If, upon receiving the command, the DPD detects the presence of the drone, the HD door opens and the 
 * LCD displays LANDING. The system then waits for the drone to enter and land. When the distance measured by the 
 * DDD is less than D2 for more than T2 seconds, it is assumed that the drone has landed, and the door is closed. 
 * The LCD then displays DRONE INSIDE.
 */

#include "LandingTask.h"
#include <kernel/Logger.h>

#define DOOR_OPERATION_TIME 2000
#define DISPLAY_UPDATE_TIME 8000
#define DISTANCE_THRESHOLD 0.2f
#define TIME_THRESHOLD 10000L
#define TIMEOUT 5000

LandingTask::LandingTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext, Pir* pPir, Sonar* pSonar){
    this->pLcd = pLcd;
    this->pLed = pLed;
    this->pDoor = pDoor;
    this->pContext = pContext;
    this->pPir = pPir;
    this->pSonar = pSonar;
    setState(IDLE);
    lastDisplayUpdate = 0;
}

LandingTask::~LandingTask(){
    if (pLed != NULL) {
        pLed->switchOff();
    }
    
    if (pDoor != NULL && pDoor->isOpen()) {
        pDoor->close();
    }
    
    if (pLcd != NULL) {
        pLcd->clear();
    }
}

void LandingTask::tick() {

}

void LandingTask::setState(State newState) {
    state = newState;
    stateTimestamp = millis();
    justEntered = true;
}

long LandingTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool LandingTask::checkAndSetJustEntered() {
    if (justEntered) {
        justEntered = false;
        return true;
    }
    return false;
}

void LandingTask::log(const String& msg) {
    Logger.log(msg);
}

//TO DO: controllare se la condizione sul pir va ad interferire con il task del takeOff
bool LandingTask::shouldStartLanding() {
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        if (msg != NULL) {
            String content = msg->getContent();
            delete msg;
            return content == "landing" || content == "LANDING";
        } else{
            return pPir->isDetected();
        }
    }
    return false;
}

//Funzione che gestisce i compiti del display LCD.
//Copiata e incollata, se quella non funziona, non funziona nemmeno questa
void LandingTask::updateDisplay() {
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