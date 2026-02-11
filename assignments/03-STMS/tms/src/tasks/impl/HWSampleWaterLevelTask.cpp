#include "tasks/api/HWSampleWaterLevelTask.hpp"

HWSampleWaterLevelTask::HWSampleWaterLevelTask(Sonar* pSonar, Context* pContext) : pSonar(pSonar),
    pContext(pContext)
{}

void HWSampleWaterLevelTask::tick()
{
    pWaterLevel = pSonar->getDistance();
    pContext->waterLevel = pWaterLevel;
}

