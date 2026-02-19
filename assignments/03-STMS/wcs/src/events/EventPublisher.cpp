#include "EventPublisher.hpp"

EventPublisher::EventPublisher(EventQueue *queue) : queue(queue) {}
void EventPublisher::publish(IEvent *evt) { queue->enqueue(evt); }