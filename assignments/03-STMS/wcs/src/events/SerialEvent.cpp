#include "events/SerialEvent.hpp"

SerialEvent::SerialEvent(Msg *value): Event(value) { }
EventType SerialEvent::getType() const { return EventType::SERIAL_EVT; }