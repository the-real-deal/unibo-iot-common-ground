#ifndef __ASYNC_FSM__
#define __ASYNC_FSM__

#include "model/HWPlatform.hpp"
#include "model/StateHolder.hpp"
#include "events/EventQueue.hpp"
#include "events/ButtonEvent.hpp"
#include "events/SerialEvent.hpp"
#include "devices/config/config.hpp"

enum SystemState { UNCONNECTED, AUTOMATIC, MANUAL };

class AsyncFSM 
{
public:
    AsyncFSM(Lcd *lcd, Pot *pot, Valve *valve, EventQueue *queue);
    void checkAndProcessEvent();
private:
    Lcd *lcd;
    Pot *potentiometer;
    Valve *valve;
    StateHolder<SystemState> *state;
    EventQueue *queue;
    void displayState();

    void handleSerialEvt(SerialEvent *serialEvt);
};

#endif