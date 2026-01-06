#include "tasks/DisplayTask.h"
#include "kernel/Config.h"
#include "context/Context.h"
#include "devices/api/Lcd.h"
#include <Arduino.h>

#define READING_TIME 8000

void DisplayTask::display(Lcd* pLcd, Context* pContext){
    switch(pContext->getHangarSystemState()){
        case Context::DRONE_INSIDE:
            pLcd->clear();
            pLcd->print("DRONE INSIDE", 0, 0);
            delay(READING_TIME);
        break; 

        case Context::TAKE_OFF:
            pLcd->clear();
            pLcd->print("TAKE OFF", 0, 0);
            delay(READING_TIME);
        break;

        case Context::DRONE_OUT: 
            pLcd->clear();
            pLcd->print("DRONE OUT", 0, 0);
            delay(READING_TIME);
        break;

        case Context::LANDING:
            pLcd->clear();
            pLcd->print("LANDING", 0, 0);
            delay(READING_TIME);
        break;

        case Context::ALARM:
            pLcd->clear();
            pLcd->print("ALARM", 0, 0);
            delay(READING_TIME);
        break;
    }

}