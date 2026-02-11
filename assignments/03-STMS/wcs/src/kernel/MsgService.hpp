#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
enum class MsgTopic { VALVE, MODE, Unknown };

class Msg 
{
private:
  MsgTopic topic;
  String content;
  String value;

public:
  Msg(MsgTopic topic, String content):
    topic(topic), content(content), value("") {
  }

  Msg(MsgTopic topic, String content, String value):
    topic(topic), content(content), value(value) {
  }
  
  MsgTopic getTopic() {
    return topic;
  }

  String getContent(){
    return content;
  }

  String getValue(){
    return value;
  }

  String getFormattedMsg() {
    String topicStr;
    switch(topic) {
        case MsgTopic::VALVE: topicStr = "VALVE"; break;
        case MsgTopic::MODE: topicStr = "MODE"; break;
        default: topicStr = "Unknown"; break;
    }

    String out = topicStr + ":" + content;
    if (value != "") {
      out += ":" + value;
    }
    return out;
  }
};

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

extern MsgServiceClass MsgService;

#endif