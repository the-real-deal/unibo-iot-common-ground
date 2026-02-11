#ifndef __MODESWITCH_TASK__
#define __MODESWITCH_TASK__

#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "devices/api/Button.hpp"
#include "Arduino.h"

enum HWModeSwitcherTaskStates
{
    PING,
    PONG
};

class HWModeSwitcherTask : public SyncTask
{

public:
    HWModeSwitcherTask(Button* pButton, Context* pContext);
    void tick();

private: 
    void setState(HWModeSwitcherTaskStates state);

    Button *pButton;
    StateHolder<HWModeSwitcherTaskStates> *pTaskState;
    Context *pContext;
};
#endif