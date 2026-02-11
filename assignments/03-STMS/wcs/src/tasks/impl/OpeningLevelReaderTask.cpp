#include "tasks/api/OpeningLevelReaderTask.hpp"

OpeningLevelReaderTask::OpeningLevelReaderTask(Pot* pPot, Valve* pValve, Context* pContext) :
    pPot(pPot), pValve(pValve), pTaskState(new StateHolder<OpeningLevelReaderTaskStates>(READING)), pContext(pContext)
{ }

void OpeningLevelReaderTask::tick()
{
    if (pContext->pGlobalInputMode->getState() != Context::InputMode::MANUAL) 
    {
        return;
    }

    OpeningLevelReaderTaskStates currentTaskState = this->pTaskState->getState();
    switch (currentTaskState) 
    {
        case READING: 
        {
            float newVal = pPot->getValue();
            if (pContext->pGlobalInputMode->getState() == Context::InputMode::MANUAL) 
            {
                pContext->pGlobalOpeningLevel->setState((float)(map(newVal, 0, 1023, 0, 100) / 100));
                pValve->setOpening((int)map(newVal, 0, 1023, VALVE_CLOSE_POS, VALVE_OPEN_POS));

                Msg newMsg(MsgTopic::VALVE, String(pContext->pGlobalOpeningLevel->getState()));
                MsgService.sendMsg(newMsg.getFormattedMsg());
            }
            break;
        }
        default: { break; }
    }
}

void OpeningLevelReaderTask::setState(OpeningLevelReaderTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}
