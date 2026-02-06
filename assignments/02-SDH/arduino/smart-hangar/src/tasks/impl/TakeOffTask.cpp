/**
 * Take-off phase: The drone activates the hangar door opening command by sending a message 
 * through the DRU subsystem. Upon receiving the command, the HD door opens, the LCD displays 
 * TAKE OFF, and the system waits for the drone to exit. To determine when the drone has left, 
 * the DDD is used: when the measured distance is greater than D1 for more than T1 seconds, it 
 * is assumed that the drone has exited, and the HD door is closed. The LCD then displays 
 * DRONE OUT.
 */

#include "kernel/Logger.h"
#include <kernel/MsgService.h>

#define DOOR_OPERATION_TIME 2000
#define DISPLAY_UPDATE_TIME 8000
#define DELAY_TIME 1000
#define TIME_THRESHOLD 10000L
#define DISTANCE_THRESHOLD 8000.0f
#define TIMEOUT 5000

TakeOffTask::TakeOffTask(Lcd* pLcd, Led* pLed, Door* pDoor, Context* pContext, Sonar* pSonar) {
    this->pLcd = pLcd;
    this->pLed = pLed;
    this->pDoor = pDoor;
    this->pContext = pContext;
    this->pSonar = pSonar;
    //Pir tolto, il controllo della distanza è fatto solo dal sonar
    //Può essere rimesso per il controllo della presenza del drone prima del takeOff
    //this->pPir = pPir
    setState(IDLE);
    lastDisplayUpdate = 0;
}

//TO DO: check if the distructor is correct --It was indeed AI generated
TakeOffTask::~TakeOffTask(){
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

/**
 * The function contains the main logic of the task.
 *      - state = IDLE: if the condition for take off are met the door start opening
 *              - Don't know if I should include a control to check the drone pesence 
 *                (it may give problem with landing)      
 *      - state = OPENING_DOOR: the system awaits in this state for a DOOR_OPERATION_TIME or until the 
 *         door isOpen before to ensure that the door is open
 *      - state =  DRONE_EXITING: the drone is exiting the hangar. State added to enable the control of time 
 *         and distance
 *          - TO DO: check if millis works with TimerOne interrupts
 *          - Don't know if I should keep a timeout timer: If a certain time elapses without changing state the door closes
 *       - state = CLOSING_DOOR: the door closes
 *       - state = COMPLETED: Task setted ad completed             
 */
void TakeOffTask::tick() {
    updateDisplay();
    
    switch(state) {
        case IDLE: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] IDLE - Waiting to start take off"));
                pLed->switchOff();
            }

            if (pContext->getCanFly() && shouldStartTakeOff()) {
                setState(OPENING_DOOR);
            }
            break;
        }
        
        case OPENING_DOOR: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Opening door"));
                pLed->switchOn();
                pDoor->open();
            }
            
            if (elapsedTimeInState() > DOOR_OPERATION_TIME || pDoor->isOpen()) {
                setState(DRONE_EXITING);
            }
            break;
        }
        
        case DRONE_EXITING: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Drone exiting"));
                pContext->setHangarSystemState(Context::HangarSystemState::TAKE_OFF);
            }
            
            float distance = pSonar->getDistance();
            long timestamp = millis();
            if (distance > DISTANCE_THRESHOLD && (millis() - timestamp) > TIME_THRESHOLD) { 
                setState(CLOSING_DOOR);
            }
            
            /* 
            if (elapsedTimeInState() > TIMEOUT) {
                setState(CLOSING_DOOR);
            }
            break;
            */
        }
        
        case CLOSING_DOOR: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Closing door"));
                pDoor->close();
                pContext->setHangarSystemState(Context::HangarSystemState::DRONE_OUT);
            }
            
            if (elapsedTimeInState() > DOOR_OPERATION_TIME || !pDoor->isOpen()) {
                setState(COMPLETED);
            }
            break;
        }
        
        case COMPLETED: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[TOT] Take off completed"));
                pLed->switchOff();
                setCompleted();
            }
            break;
        }
    }
}


void TakeOffTask::setState(State newState) {
    state = newState;
    stateTimestamp = millis();
    justEntered = true;
}

/**
 * Used  to monitor how much time the system is in a certain state
 */
long TakeOffTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

/**
 * Used  to monitor how much time the system is in a certain state
 */
bool TakeOffTask::checkAndSetJustEntered() {
    if (justEntered) {
        justEntered = false;
        return true;
    }
    return false;
}

/**
 * Function used to log the messages
 */
void TakeOffTask::log(const String& msg) {
    Logger.log(msg);
}


/**
 * The function check if the condition for the Take-off are met (it "monitors" the message exchange)
 */
bool TakeOffTask::shouldStartTakeOff(){
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        if (msg != NULL) {
            String content = msg->getContent();
            delete msg;
            return content == "takeOff" || content == "TAKING_OFF";
        }
    }
    return false;
}


/**
 * The function allows to use the LCD functionalities without the LCD task
 *  - It's duplicated in all of the tasks that uses the LCD so there is the problem of code duplication (a bit of a lot :( )
 *  - I don't like the ALARM state included among the states of the Hangar because it doesn't follow the logic --> fixing needed
 */
void TakeOffTask::updateDisplay() {
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