#include "tasks/api/HWSampleWaterLevelTask.hpp"

HWSampleWaterLevelTask::HWSampleWaterLevelTask(Sonar* pSonar, Context* pContext) : 
    pSonar(pSonar), pContext(pContext)
{ }

void HWSampleWaterLevelTask::tick() {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = this->getPeriod(); 

    for(;;) {
        pContext->waterLevel = pSonar->getDistance();
        // Quando arriva una chiamata bloccante lo scheduler passa all task dopo ("congela" quella attuale).
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

