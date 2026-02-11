#include "tasks/api/HWModeSwitcherTask.hpp"

HWModeSwitcherTask::HWModeSwitcherTask(Button *pButton, Context *pContext) : pButton(pButton),
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
                setState(PONG);
            }
            break;
        }
        case PONG: 
        {
            if (pButton->isPressed()) 
            {
                pContext->pGlobalInputMode->setState(Context::InputMode::AUTOMATIC);
                setState(PING);
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
