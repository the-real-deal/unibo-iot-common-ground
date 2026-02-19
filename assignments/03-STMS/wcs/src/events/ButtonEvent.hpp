#ifndef __BTN_EVT__
#define __BTN_EVT__

#include "events/Event.hpp"

class ButtonEvent: public Event<bool> 
{
public:
    ButtonEvent(bool value);
}; 
#endif