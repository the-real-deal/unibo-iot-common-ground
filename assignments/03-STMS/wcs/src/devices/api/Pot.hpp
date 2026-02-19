#ifndef __POT__
#define __POT__

#include "Arduino.h"
#include "devices/api/AbstractDevice.hpp"
#include "devices/config/config.hpp"
#include "events/EventPublisher.hpp"
#include "events/PotEvent.hpp"

/* Potentiometer Class:
 *  Interface for a potentiometer connected to an analog pin.
 */
class Pot: public AbstractDevice, public EventPublisher 
{
public:
  Pot(uint8_t pin, EventQueue *queue);
  float getValue();
};

#endif