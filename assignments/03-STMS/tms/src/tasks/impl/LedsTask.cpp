#include "tasks/api/LedsTask.hpp"

LedsTask::LedsTask(Context *pContext, Led* pOkLed, Led* pKoLed) :
    pTaskState(new StateHolder<LedsTaskStates>(PING)),
    pContext(pContext),
    pKoLed(pKoLed),
    pOkLed(pOkLed)
{
    setState(PING);
}

void LedsTask::tick()
{
    for(;;){

        LedsTaskStates currentTaskState = pTaskState->getState();
        
        switch (currentTaskState)
        {
            case PING:
            {
                if (!pContext->isNetworkOk) 
                {
                    pKoLed->switchOn();
                    pOkLed->switchOff();
                    setState(PONG);
                }
                break;
            }
            case PONG:
            {
                if (pContext->isNetworkOk && pContext->canSendData) 
                {
                    pOkLed->switchOn();
                    pKoLed->switchOff();
                    setState(PING);
                }
                break;
            }
            default: { break; }
        }
        vTaskDelay(this->getPeriod());
    }
}
    
    void LedsTask::setState(LedsTaskStates state)
    {
        this->justEntered = true;
        this->pTaskState->setState(state);
        this->stateTimestamp = millis();
    }
