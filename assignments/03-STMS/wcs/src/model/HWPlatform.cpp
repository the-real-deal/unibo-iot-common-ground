#include "HWPlatform.hpp"

Button *HWPlatform::getModeToggleButton() { return get<Button *>(); }
Valve *HWPlatform::getValve() { return get<Valve *>(); }
Lcd *HWPlatform::getOperatorLCD() { return get<Lcd *>(); }
Pot *HWPlatform::getPotentiometer() { return get<Pot *>(); }

void HWPlatform::add(AbstractDevice *device)
{
    if (lastDevice >= NUM_DEVICES || device == nullptr)
    {
      return;
    }
    devices[lastDevice++] = device;
}
