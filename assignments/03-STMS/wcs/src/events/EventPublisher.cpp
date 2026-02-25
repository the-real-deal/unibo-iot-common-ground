#include "events/EventPublisher.hpp"

EventPublisher::EventPublisher() {}

void EventPublisher::publish(IEvent *evt) 
{
    // noInterrupts(); // TODO: check
    sharedQueue.enqueue(evt);
    // interrupts();
}