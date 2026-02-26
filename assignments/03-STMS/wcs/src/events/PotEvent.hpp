#ifndef __POT_EVT__
#define __POT_EVT__

#include "events/Event.hpp"

class PotEvent: public Event<int>
{
public:
    PotEvent(int value); 
    EventType getType() const override;
}; 
#endif