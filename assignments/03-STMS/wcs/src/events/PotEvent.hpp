#ifndef __POT_EVT__
#define __POT_EVT__

#include "events/Event.hpp"

class PotEvent: public Event<int> //changed to int
{
public:
    PotEvent(int value); //changed to int
    EventType getType() const override;
}; 
#endif