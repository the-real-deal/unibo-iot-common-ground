#include "AsyncFSM.hpp"

AsyncFSM::AsyncFSM(Lcd *lcd, Pot *pot, Valve *valve, EventQueue *queue)
    : state(new StateHolder<SystemState>(UNCONNECTED)),
    lcd(lcd),
    potentiometer(pot),
    valve(valve),
    queue(queue)
{
    displayState();
}

void log(String msg) { Serial.println(msg); }

void AsyncFSM::handleSerialEvt(SerialEvent *serialEvt)
{
    Msg *receivedMsg = serialEvt->getValue();
    switch (receivedMsg->getTopic())
    {
    case MsgTopic::MODE: 
    {
        // new mode received from CUS,
        // decoding which mode was received
        if (receivedMsg->getContent().equals("MANUAL"))
        {
            this->state->setState(SystemState::MANUAL);
            // nothing else to do

        } else if (receivedMsg->getContent().equals("UNCONNECTED"))
        {
            this->state->setState(SystemState::UNCONNECTED);
            // nothing else to do   
        } 
        else if (receivedMsg->getContent().equals("AUTOMATIC"))
        {
            // AUTOMATIC mode received
            this->state->setState(SystemState::AUTOMATIC);
        } 
        else 
        {
            log("unexpected mode received!");
        }
        break;
    }
    case MsgTopic::VALVE: 
    {
        if (this->state->getState() == SystemState::AUTOMATIC) 
        {
            // we received something about the valve
            // assuming the opening value arrives already in percentage between [0.0, 1.0]
            this->valve->setOpening(receivedMsg->getContent().toFloat() * 100.0, 0L, 100L);
        }
        break;
    }
    default: 
    {
        log("Unexpected msg topic received!");
        break;
    }
    }
}

void AsyncFSM::handleButtonEvt(ButtonEvent *buttonEvt)
{
    SystemState currentSystemState = this->state->getState();
    switch (currentSystemState)
    {
        case SystemState::UNCONNECTED:
        {
            /* do nothing */
            break;
        }
        case SystemState::AUTOMATIC:
        {
            this->state->setState(SystemState::MANUAL);
            break;
        }
        case SystemState::MANUAL:
        {
            this->state->setState(SystemState::AUTOMATIC);
            break;
        }
    }
}

void AsyncFSM::checkAndProcessEvent()
{
    noInterrupts();
    bool isEmpty = queue->isEmpty();
    interrupts();
    
    // Exit early if no events occurred
    if (isEmpty) { return; }
    
    noInterrupts();
    IEvent *evt = queue->dequeue();
    interrupts();

    switch (evt->getType())
    {
    case EventType::SERIAL_EVT: 
    {
        SerialEvent *serialEvt = static_cast<SerialEvent *>(evt);
        log("serial event!");
        handleSerialEvt(serialEvt);
        delete serialEvt;
        break;
    }
    case EventType::BUTTON_EVT:
    {
        ButtonEvent *buttonEvt = static_cast<ButtonEvent *>(evt);
        log("button event!");
        handleButtonEvt(buttonEvt);
        delete buttonEvt;
    }
    default:
        log("unexpected event received!");
        break;
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
