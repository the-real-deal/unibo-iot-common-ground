#ifndef __SERIAL_EVT__
#define __SERIAL_EVT__

#include "events/Event.hpp"
#include "kernel/Msg.hpp"
#include "kernel/MsgService.hpp"

class SerialEvent: public Event<Msg*> 
{ 
public:
    SerialEvent(Msg* value);
    ~SerialEvent() override { delete this->value; }
}; 
#endif
