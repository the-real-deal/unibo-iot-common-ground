#include "tasks/api/LedsTask.hpp"

LedsTask::LedsTask(Context *pContext, Led* pOkLed, Led* pKoLed) : 
    pContext(pContext),
    pKoLed(pKoLed),
    pOkLed(pOkLed)
{
    setState(OK);
}

void LedsTask::tick()
{
    LedsTaskStates currentTaskState = pTaskState->getState();
    
    switch (currentTaskState)
    {
        case OK: 
        {
            if (!pContext->isNetworkOk) 
            {
                pKoLed->switchOn();
                pOkLed->switchOff();
                setState(KO);
            }
            break;
        }
        case KO: 
        {
            if (pContext->isNetworkOk && pContext->canSendData) 
            {
                pOkLed->switchOn();
                pKoLed->switchOff();
                setState(OK);
            }
            break;
        }
        default: { break; }
    }
}

void LedTaskStates::setState(LedsTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}
