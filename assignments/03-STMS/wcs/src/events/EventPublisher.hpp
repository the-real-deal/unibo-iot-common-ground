#ifndef __EVT_PUB__
#define __EVT_PUB__

#include "IEvent.hpp"
#include "events/EventQueue.hpp"

class EventPublisher 
{
public:
    EventPublisher(EventQueue *queue);
    void publish(IEvent* evt);
private:
    EventQueue* queue;
};
#endif