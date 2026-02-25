#include "devices/api/Button.hpp"
#include "EnableInterrupt.h"

// Static member for ISR callback
static Button* currentInstance = nullptr;

inline void onButtonPressed() {
  if (currentInstance == nullptr) return;
  currentInstance->publish(new ButtonEvent(true));
}

/*
* Button Constructor
*/
Button::Button(uint8_t pin, EventQueue *queue) : 
  AbstractDevice(pin), 
  EventPublisher(queue),
  lastPressedTime(0), 
  justClicked(false) 
{
  pinMode(pin, INPUT);
  currentInstance = this;
  enableInterrupt(pin, onButtonPressed, RISING);
}
