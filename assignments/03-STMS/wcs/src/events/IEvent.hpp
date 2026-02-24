#ifndef __IEVT__
#define __IEVT__

enum EventType { SERIAL_EVT, BUTTON_EVT, POT_EVT };

class IEvent 
{ 
public:
    virtual EventType getType() const = 0;
    virtual ~IEvent() = default;
};

#endif