#ifndef __BUTTON__
#define __BUTTON__

#include "Arduino.h"
#include "events/EventPublisher.hpp"
#include "events/ButtonEvent.hpp"
#include "devices/api/AbstractDevice.hpp"
#include "devices/config/config.hpp"

/* Button Class:
 *  Interface for a digital button connected to a specified pin.
 */
class Button: public AbstractDevice, public EventPublisher
{
public:
  Button(uint8_t pin);

private:
  long lastPressedTime;
  bool justClicked;
};

#endif