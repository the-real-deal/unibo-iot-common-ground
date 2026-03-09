#include "tasks/api/HWSampleWaterLevelTask.hpp"

HWSampleWaterLevelTask::HWSampleWaterLevelTask(Sonar *pSonar, Context *pContext) : pSonar(pSonar), pContext(pContext)
{
}

void HWSampleWaterLevelTask::tick()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = this->getPeriod();
    for (;;)
    {
        float lvl = pSonar->getDistance();
        logger.log("Measured: " + String(lvl) + "cm", "SamplingTask");
        pContext->waterLevel = lvl;
        // Quando arriva una chiamata bloccante 
        // lo scheduler passa alla task dopo ("congela" quella attuale).
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
