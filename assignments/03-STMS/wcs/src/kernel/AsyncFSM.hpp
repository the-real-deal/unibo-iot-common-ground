#ifndef __ASYNC_FSM__
#define __ASYNC_FSM__

#include "kernel/MsgService.hpp"
#include "model/HWPlatform.hpp"
#include "model/StateHolder.hpp"
#include "events/EventQueue.hpp"
#include "events/ButtonEvent.hpp"
#include "events/SerialEvent.hpp"
#include "events/PotEvent.hpp"
#include "devices/config/config.hpp"

enum SystemState { UNCONNECTED, AUTOMATIC, MANUAL };

class AsyncFSM 
{
public:
    AsyncFSM(Lcd *lcd, Pot *pot, Valve *valve);
    void checkAndProcessEvent();
    StateHolder<SystemState> *state;
private:
    Lcd *lcd;
    Pot *potentiometer;
    Valve *valve;
    void displayState();

    void handleSerialEvt(SerialEvent *serialEvt);
    void handleButtonEvt(ButtonEvent *buttonEvt);
    void handlePotEvt(PotEvent *potEvt);
};

#endif