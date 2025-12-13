#ifndef __BUTTON_IMPL__
#define __BUTTON_IMPL__

#include "api/Button.h"


class ButtonImpl : public Button {
public:
  ButtonImpl(int pin);
  
  bool isPressed() override;

private:
  int pin;
};

#endif