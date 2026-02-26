#include "tasks/api/DataSenderTask.hpp"

DataSenderTask::DataSenderTask(Context *pContext) :
    pTaskState(new StateHolder<DataSenderTaskStates>(OFFLINE)),
    pContext(pContext)
{
    setState(OFFLINE);
}

void DataSenderTask::tick()
{
    for(;;){

        DataSenderTaskStates currentTaskState = pTaskState->getState();
        switch (currentTaskState)
        {
            case OFFLINE:
            {
                if (pContext->isNetworkOk && pContext->canSendData)
                {
                    setState(ONLINE);
                    return;
                }
                break;
            }
            case ONLINE:
            {
                connectionProvider.mqttSendMsg(pContext->waterLevel);
                if (!(pContext->isNetworkOk && pContext->canSendData))
                {
                    setState(OFFLINE);
                    return;
                }
                break;
            }
            default: { break; }
        }
        vTaskDelay(this->getPeriod());
    }
}
    
    void DataSenderTask::setState(DataSenderTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}