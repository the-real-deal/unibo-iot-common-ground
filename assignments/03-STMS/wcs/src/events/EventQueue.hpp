#ifndef __EVT_QUEUE__
#define __EVT_QUEUE__

#include "events/IEvent.hpp"
#include "LinkedList.h"

class EventQueue 
{
public:
    void init();
    void enqueue(IEvent* evt);
    IEvent* dequeue();
    bool isEmpty();
private:
    LinkedList<IEvent*>* queue;
};

extern EventQueue sharedQueue;

#endif