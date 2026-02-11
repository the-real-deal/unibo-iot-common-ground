#include "tasks/api/ConnectionMonitoringTask.hpp"

ConnectionMonitoringTask::ConnectionMonitoringTask(Context *pContext) :
 pContext(pContext), pTaskState(new StateHolder<ConnectionMonitoringTaskStates>(RUNNING))
{ 
    setState(RUNNING);
}

void ConnectionMonitoringTask::tick()
{
    ConnectionMonitoringTaskStates currentTaskState = pTaskState->getState();
    switch(currentTaskState) 
    {
        case RUNNING: {
            pContext->pGlobalSystemState->setState(Serial.available() ? 
                Context::SystemStates::AVAILABLE : 
                Context::SystemStates::NOT_AVAILABLE);
            break;
        }

        default: {
            break;
        }
    }
}

void ConnectionMonitoringTask::setState(ConnectionMonitoringTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}