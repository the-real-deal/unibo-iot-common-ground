#include "tasks/api/HWSampleWaterLevelTask.hpp"

HWSampleWaterLevelTask::HWSampleWaterLevelTask(Sonar* pSonar, Context* pContext) : 
    pSonar(pSonar), pContext(pContext)
{ }

void HWSampleWaterLevelTask::tick()
{
    pContext->waterLevel = pSonar->getDistance();
}

