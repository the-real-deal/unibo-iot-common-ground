#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
using MsgTopic = enum { DRU, SDH, Unknown };

class Msg 
{
private:
  MsgTopic topic;
  String content;

public:
  Msg(MsgTopic topic, String content){
    this->topic = topic;
    this->content = content;
  }
  
  MsgTopic getTopic() {
    return topic;
  }

  String getContent(){
    return content;
  }
};

class Pattern 
{

public:
  virtual boolean match(const Msg& m) = 0;  
};

class MsgServiceClass 
{
public: 
  
  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  
  void sendMsg(const String& msg);
};

extern MsgServiceClass MsgService;

#endif