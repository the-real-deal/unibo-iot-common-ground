#ifndef __POT_EVT__
#define __POT_EVT__

#include "events/Event.hpp"

class PotEvent: public Event<float> 
{
public:
    PotEvent(float value);
    EventType getType() const override;
}; 
#endif