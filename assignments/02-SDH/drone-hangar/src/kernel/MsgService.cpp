#include "Arduino.h"
#include "MsgService.hpp"

String content;
String topic;

MsgServiceClass MsgService;

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    topic = "";
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  Serial.begin(9600);
  // longest message content: "TAKING_OFF" -> 10 chars, 8 bytes each
  content.reserve(80);
  // possible topics: DU; SH -> 2 chars, 8 bytes each
  topic.reserve(16);
  topic = "";
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void serialEvent() {
  if (!Serial.available()) {
    return;
  }
  bool separatorSurpassed = false;
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch != ':' && !separatorSurpassed) {
      topic += ch;
      continue;
    } else if (ch == ':') {
      separatorSurpassed = true;
      continue;
    } else if (ch != '\n') {
      content += ch;   
    } else {
      MsgTopic decodedTopic = MsgTopic::Unknown;
      if (topic.equalsIgnoreCase("SDH")) {
        decodedTopic = MsgTopic::SDH;
      } else if (topic.equalsIgnoreCase("DRU")) {
        decodedTopic = MsgTopic::DRU;
      }
      MsgService.currentMsg = new Msg(decodedTopic, content);
      MsgService.msgAvailable = true;      
    }
  }
}