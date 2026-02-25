#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
#include "kernel/Msg.hpp"
#include "events/EventPublisher.hpp"
#include "events/EventQueue.hpp"
#include "events/SerialEvent.hpp"

class MsgServiceClass
{
public:
  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  void sendMsg(const String& msg);
};

extern MsgServiceClass msgService;

#endif