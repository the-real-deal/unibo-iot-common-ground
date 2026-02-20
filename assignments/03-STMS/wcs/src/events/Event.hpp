#ifndef __PEVT__
#define __PEVT__
#include "events/IEvent.hpp"

template <typename D>
class Event : public IEvent {
protected:
    D value;
public:
    Event(D value) { this->value = value; }
    D getValue() { return value; }
};
#endif