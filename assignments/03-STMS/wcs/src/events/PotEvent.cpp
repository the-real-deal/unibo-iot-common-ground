#include "events/PotEvent.hpp"

PotEvent::PotEvent(float value) : Event(value) { }
EventType PotEvent::getType() const { return EventType::POT_EVT; }