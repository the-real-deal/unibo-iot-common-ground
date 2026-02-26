#include "events/PotEvent.hpp"

PotEvent::PotEvent(int value) : Event(value) { }
EventType PotEvent::getType() const { return EventType::POT_EVT; }