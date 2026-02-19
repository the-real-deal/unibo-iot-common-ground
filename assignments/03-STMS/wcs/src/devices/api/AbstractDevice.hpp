#ifndef __DEVICE__
#define __DEVICE__

class AbstractDevice {
protected:
    uint8_t pin;
public:
    AbstractDevice(uint8_t pin) { this->pin = pin; }
    uint8_t getPin() { return this->pin; }
    virtual ~AbstractDevice() = default;
};

#endif