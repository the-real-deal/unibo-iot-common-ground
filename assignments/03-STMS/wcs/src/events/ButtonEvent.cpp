#include "events/ButtonEvent.hpp"

ButtonEvent::ButtonEvent(bool value) : Event(value) { }

EventType ButtonEvent::getType() const {
    return EventType::BUTTON_EVT;
}
