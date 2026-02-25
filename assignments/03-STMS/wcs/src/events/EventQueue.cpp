#include "events/EventQueue.hpp"

EventQueue sharedQueue;

void EventQueue::init() { queue = new LinkedList<IEvent*>(); }
void EventQueue::enqueue(IEvent *evt) { queue->unshift(evt); }
IEvent *EventQueue::dequeue() { return queue->pop(); }
bool EventQueue::isEmpty() { return queue->size() == 0; }