#include "tasks/api/SerialReaderTask.hpp"

SerialReaderTask::SerialReaderTask(Lcd *pLcd, Context *pContext) : pLcd(pLcd),
                                                                   pTaskState(new StateHolder<SerialReaderTaskStates>(OFFLINE)),
                                                                   pContext(pContext)
{
    setState(OFFLINE);
}

void SerialReaderTask::tick()
{
    SerialReaderTaskStates currentTaskState = pTaskState->getState();
    switch (currentTaskState)
    {
    case OFFLINE:
    {
        if (pContext->pGlobalSystemState->getState() != Context::SystemStates::NOT_AVAILABLE)
        {
            setState(ONLINE);
            return;
        }
        break;
    }
    case ONLINE:
    {
        if (pContext->pGlobalSystemState->getState() != Context::SystemStates::AVAILABLE)
        {
            setState(OFFLINE);
            return;
        }

        Msg *pMsg = MsgService.receiveMsg();
        if (pMsg == nullptr)
        {
            // no new message
            return;
        }
        if (pMsg->getTopic() == MsgTopic::VALVE)
        {
            pContext->pGlobalOpeningLevel->setState(pMsg->getContent().toFloat());
        } else if (pMsg->getTopic() == MsgTopic::MODE) 
        {
            Context::InputMode newMode = Context::InputMode::DISCONNECTED;
            if (pMsg->getContent() == "AUTOMATIC") { newMode = Context::InputMode::AUTOMATIC; }
            else if (pMsg->getContent() == "MANUAL") { newMode = Context::InputMode::MANUAL; }
            pContext->pGlobalInputMode->setState(newMode);
            pLcd->print(pMsg->getContent());
        }
        delete pMsg;
        break;
    }
    default: { break; }
    }
}

void SerialReaderTask::setState(SerialReaderTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}