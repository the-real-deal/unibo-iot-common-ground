#include "events/EventPublisher.hpp"

EventPublisher::EventPublisher() {}

void EventPublisher::publish(IEvent *evt) 
{
    noInterrupts();
    sharedQueue.enqueue(evt);
    interrupts();
}