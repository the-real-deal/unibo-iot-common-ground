#include "tasks/DisplayTask.h"
#include <Arduino.h>

#define READING_TIME 8000

void DisplayTask::display(Lcd* pLcd, Context* pContext){
    switch(pContext->hangarSystemState){
        case DRONE_INSIDE:
            plcd->clear();
            plcd->setCursor(0,0);
            plcd->print("DRONE INSIDE");
            delay(READING_TIME);
        break; 

        case TAKE_OFF:
            plcd->clear();
            plcd->setCursor(0,0);
            plcd->print("TAKE OFF");
            delay(READING_TIME);
        break;

        case DRONE_OUT: 
            plcd->clear();
            plcd->setCursor(0,0);
            plcd->print("DRONE OUT");
            delay(READING_TIME);
        break;

        case LANDING:
            plcd->clear();
            plcd->setCursor(0,0);
            plcd->print("LANDING");
            delay(READING_TIME);
        break;

        case ALARM:
            plcd->clear();
            plcd->setCursor(0,0);
            plcd->print("ALARM");
            delay(READING_TIME);
        break;
    }

}