/**
 * Landing phase: When the drone approaches the hangar, it sends the opening command (via DRU). 
 * If, upon receiving the command, the DPD detects the presence of the drone, the HD door opens and the 
 * LCD displays LANDING. The system then waits for the drone to enter and land. When the distance measured by the 
 * DDD is less than D2 for more than T2 seconds, it is assumed that the drone has landed, and the door is closed. 
 * The LCD then displays DRONE INSIDE.
 */

#include "./tasks/api/LandingTask.h"
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

//TO DO: check if the distructor is correct --It was indeed AI generated
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

/**
 * The function contains the main logic of the task.
 *      - state = IDLE: if the condition for take off are met the door start opening
 *              - Don't know if I should include a control to check the drone pesence 
 *                (it may give problem with landing)      
 *      - state = OPENING_DOOR: the system awaits in this state for a DOOR_OPERATION_TIME or until the 
 *         door isOpen before to ensure that the door is open
 *      - state = WAITING_DRONE state added in the case that the serial send the message for landing but the drone is 
 *        not yet detected
 *             - Don't know if I should keep a timeout timer: If a certain time elapses without changing state the door closes
 *      - state =  DRONE_ENTERING: the drone is entering the hangar. State added to enable the control of time 
 *         and distance
 *          - TO DO: check if millis works with TimerOne interrupts
 *          - Don't know if I should keep a timeout timer: If a certain time elapses without changing state the door closes
 *       - state = CLOSING_DOOR: the door closes
 *       - state = COMPLETED: Task setted ad completed             
 */
void LandingTask::tick() {
    updateDisplay();
    
    switch(state) {
        case IDLE: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[LT] IDLE - Waiting for landing request"));
            }

            if (shouldStartLanding()) {
                setState(OPENING_DOOR);
            }
            break;
        }
        
        case OPENING_DOOR: {
            if (checkAndSetJustEntered() && pContext->getCanFly()) {
                Logger.log(F("[LT] Opening door for landing"));
                pDoor->open();
                pContext->setHangarSystemState(Context::HangarSystemState::LANDING);
            }
            
            if (elapsedTimeInState() > DOOR_OPERATION_TIME || pDoor->isOpen()) {
                setState(WAITING_DRONE);
            }
            break;
        }
        
        case WAITING_DRONE: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[LT] Waiting for drone to enter"));
            }
            
            if (pPir->isDetected()) {
                setState(DRONE_ENTERING);
            }
            
            if (elapsedTimeInState() > 10000) {
                Logger.log(F("[LT] Timeout waiting for drone"));
                setState(CLOSING_DOOR);
            }
            break;
        }
        
        case DRONE_ENTERING: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[LT] Drone entering hangar"));
            }
            
            float distance = pSonar->getDistance();
            long timestamp = millis();
            if (distance < DISTANCE_THRESHOLD && (millis() - timestamp) > TIME_THRESHOLD) {
                setState(CLOSING_DOOR);
            }
            
            /*
            if (elapsedTimeInState() > 5000) {
                setState(CLOSING_DOOR);
            }
            */
            break;
        }
        
        case CLOSING_DOOR: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[LT] Closing door"));
                pDoor->close();
            }
            
            if (elapsedTimeInState() > DOOR_OPERATION_TIME || !pDoor->isOpen()) {
                setState(COMPLETED);
            }
            break;
        }
        
        case COMPLETED: {
            if (checkAndSetJustEntered()) {
                Logger.log(F("[LT] Landing completed"));
                pLed->switchOff();
                pContext->setHangarSystemState(Context::HangarSystemState::DRONE_INSIDE);
                setCompleted();
            }
            break;
        }
    }
}

void LandingTask::setState(State newState) {
    state = newState;
    stateTimestamp = millis();
    justEntered = true;
}

long LandingTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

/**
 * Used  to monitor how much time the system is in a certain state
 */
bool LandingTask::checkAndSetJustEntered() {
    if (justEntered) {
        justEntered = false;
        return true;
    }
    return false;
}

/**
 * Function used to log the messages
 */
void LandingTask::log(const String& msg) {
    Logger.log(msg);
}

/**
 * The function check if the condition for the landing are met (it "monitors" the message exchange)
 * 
 *  - TO DO: check if the pir->isDetected() condition interfers with take off task
 */
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

/**
 * The function allows to use the LCD functionalities without the LCD task
 *  - It's duplicated in all of the tasks that uses the LCD so there is the problem of code duplication (a bit of a lot :( )
 *  - I don't like the ALARM state included among the states of the Hangar because it doesn't follow the logic --> fixing needed
 */
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