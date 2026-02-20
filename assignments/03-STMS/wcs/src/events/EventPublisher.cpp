#include "events/EventPublisher.hpp"

EventPublisher::EventPublisher(EventQueue *queue) : queue(queue) {}

void EventPublisher::publish(IEvent *evt) 
{
    noInterrupts(); // TODO: check
    queue->enqueue(evt);
    interrupts();
}