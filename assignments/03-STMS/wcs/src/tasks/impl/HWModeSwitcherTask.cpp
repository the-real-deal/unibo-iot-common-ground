#include "tasks/api/HWModeSwitcherTask.hpp"

HWModeSwitcherTask::HWModeSwitcherTask(Button *pButton, Lcd* pLcd, Context *pContext) : 
    pButton(pButton),
    pLcd(pLcd),
    pTaskState(new StateHolder<HWModeSwitcherTaskStates>(PING)),
    pContext(pContext)
{
    setState(PING);
}

void HWModeSwitcherTask::tick()
{
    HWModeSwitcherTaskStates currentTaskState = pTaskState->getState();
    switch (currentTaskState)
    {
        case PING: 
        {
            if (pButton->isPressed() && pContext->pGlobalInputMode->getState() != Context::InputMode::DISCONNECTED) 
            {
                pContext->pGlobalInputMode->setState(Context::InputMode::MANUAL);
                pLcd->print("MANUAL");
                setState(PONG);
                Msg newMsg(MsgTopic::MODE, "MANUAL");
                MsgService.sendMsg(newMsg.getFormattedMsg());
            }
            break;
        }
        case PONG: 
        {
            if (pButton->isPressed()) 
            {
                pContext->pGlobalInputMode->setState(Context::InputMode::AUTOMATIC);
                pLcd->print("AUTOMATIC");
                setState(PING);
                Msg newMsg(MsgTopic::MODE, "AUTOMATIC");
                MsgService.sendMsg(newMsg.getFormattedMsg());
            }
            break;
        }
        default: { break; }
    }
}

void HWModeSwitcherTask::setState(HWModeSwitcherTaskStates state)
{
    this->justEntered = true;
    this->pTaskState->setState(state);
    this->stateTimestamp = millis();
}
