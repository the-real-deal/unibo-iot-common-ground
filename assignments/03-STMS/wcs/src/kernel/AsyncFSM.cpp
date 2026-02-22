#include "AsyncFSM.hpp"

// #define DEBUG

AsyncFSM::AsyncFSM(Lcd *lcd, Pot *pot, Valve *valve, EventQueue *queue, MsgServiceClass *msgService)
    : state(new StateHolder<SystemState>(UNCONNECTED)),
    lcd(lcd),
    potentiometer(pot),
    valve(valve),
    queue(queue),
    msgService(msgService)
{
    displayState();
}

void log(String msg) { Serial.println(msg); }

void AsyncFSM::handleSerialEvt(SerialEvent *serialEvt)
{
    Msg *receivedMsg = serialEvt->getValue();
    MsgTopic msgTopic = receivedMsg->getTopic();
    switch (msgTopic)
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
            #ifdef DEBUG
            log("unexpected mode received!");
            #endif
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
        #ifdef DEBUG
        log("Unexpected msg topic received!");
        #endif
        break;
    }
    }
}

void AsyncFSM::handleButtonEvt(ButtonEvent *buttonEvt)
{
    SystemState currentSystemState = this->state->getState();
    SystemState newState = currentSystemState;
    switch (currentSystemState)
    {
        case SystemState::UNCONNECTED:
        {
            /* do nothing */
            break;
        }
        case SystemState::AUTOMATIC:
        {
            newState = SystemState::MANUAL;
            break;
        }
        case SystemState::MANUAL:
        {
            newState = SystemState::AUTOMATIC;
            break;
        }
    }

    this->state->setState(newState);
    String newStateStr = "UNCONNECTED";
    if (newState == SystemState::AUTOMATIC) { newStateStr = "AUTOMATIC"; }
    else if (newState == SystemState::MANUAL) { newStateStr = "MANUAL"; }
    msgService->sendMsg("MODE:" + newStateStr);
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
        #ifdef DEBUG
        log("serial event!");
        #endif
        handleSerialEvt(serialEvt);
        delete serialEvt;
        break;
    }
    case EventType::BUTTON_EVT:
    {
        ButtonEvent *buttonEvt = static_cast<ButtonEvent *>(evt);
        #ifdef DEBUG
        log("button event!");
        #endif
        handleButtonEvt(buttonEvt);
        delete buttonEvt;
        break;
    }
    default:
        #ifdef DEBUG
        log("unexpected event received!");
        #endif
        break;
    }

    displayState();
}

void AsyncFSM::displayState()
{
    String msg = "UNCONNECTED";
    switch (state->getState())
    {
        case SystemState::AUTOMATIC: { msg = "AUTOMATIC"; break; }
        case SystemState::MANUAL: { msg = "MANUAL"; break; }
        default: break;
    }
    lcd->print(msg);
    #ifdef DEBUG
    log(msg);
    #endif
}
