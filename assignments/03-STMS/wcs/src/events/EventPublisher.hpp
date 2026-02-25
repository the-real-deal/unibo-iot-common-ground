#ifndef __EVT_PUB__
#define __EVT_PUB__

#include "Arduino.h"
#include "events/IEvent.hpp"
#include "events/EventQueue.hpp"

class EventPublisher 
{
public:
    EventPublisher();
    void publish(IEvent* evt);
};
#endif