#include "AsyncFSM.hpp"

AsyncFSM::AsyncFSM(Lcd *lcd, Pot *pot, Valve *valve, EventQueue *queue)
    : lcd(lcd),
      potentiometer(pot),
      valve(valve),
      state(new StateHolder<SystemState>(UNCONNECTED)),
      queue(queue)
{
    displayState();
}

void AsyncFSM::handleSerialEvt(SerialEvent *serialEvt)
{
    Msg *receivedMsg = serialEvt->getValue();
    if (receivedMsg->getTopic() == MsgTopic::MODE)
    {
        // new mode received from CUS,
        // decoding which mode was received
        if (receivedMsg->getContent().equals("MANUAL"))
        {
            this->state->setState(SystemState::MANUAL);
            // nothing else to do
            return;
        }
        if (receivedMsg->getContent().equals("UNCONNECTED"))
        {
            this->state->setState(SystemState::UNCONNECTED);
            // nothing else to do
            return;
        }
        // AUTOMATIC mode received, do nothing (as we are already
        // in that mode)
        return;
    }
    // we received something about the valve
    // assuming the opening value arrives already in percentage between [0.0, 1.0]
    this->valve->setOpening(receivedMsg->getContent().toFloat() * 100.0, 0L, 100L);
    return;
}

void AsyncFSM::processEvent()
{
    // Exit early if no events occurred
    if (queue->isEmpty())
        return;

    IEvent *evt = queue->dequeue();
    SystemState currentSystemState = this->state->getState();

    SerialEvent *serialEvt = static_cast<SerialEvent *>(evt);
    if (serialEvt != nullptr)
    {
        handleSerialEvt(serialEvt);
        delete serialEvt;
        return;
    }

    switch (currentSystemState)
    {
    case SystemState::UNCONNECTED:
    {
        /* code */
        break;
    }
    case SystemState::AUTOMATIC:
    {
        ButtonEvent *btnEvt = static_cast<ButtonEvent *>(evt);
        if (btnEvt != nullptr)
        {
            this->state->setState(SystemState::MANUAL);
            delete btnEvt;
        }
        break;
    }
    case SystemState::MANUAL:
    {
        float rawOpening = this->potentiometer->getValue();
        this->valve->setOpening(
            map(rawOpening, POT_MIN, POT_MAX, 0, 100),
            0L, 100L);
        ButtonEvent *btnEvt = static_cast<ButtonEvent *>(evt);
        if (btnEvt != nullptr)
        {
            this->state->setState(SystemState::AUTOMATIC);
            delete btnEvt;
        }
        break;
    }
    }

    displayState();
}

void AsyncFSM::displayState()
{
    if (state->getState() == SystemState::AUTOMATIC)
    {
        lcd->print("AUTOMATIC");
    }
    else if (state->getState() == SystemState::MANUAL)
    {
        lcd->print("MANUAL");
    }
    else
    {
        lcd->print("UNCONNECTED");
    }
}
