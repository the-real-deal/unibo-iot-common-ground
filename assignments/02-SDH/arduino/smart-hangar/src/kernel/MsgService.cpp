#include "Arduino.h"
#include "MsgService.h"

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
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  Serial.begin(115200);
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
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch != ':' && topic == "") {
      topic += ch;
      continue;
    }
    if (ch == '\n'){
      MsgTopic decodedTopic = MsgTopic::Unknown;
      if (topic.equalsIgnoreCase("SDH")) {
        decodedTopic = MsgTopic::SDH;
      } else if (topic.equalsIgnoreCase("DRU")) {
        decodedTopic = MsgTopic::DRU;
      }
      MsgService.currentMsg = new Msg(decodedTopic, content);
      MsgService.msgAvailable = true;      
    } else {
      content += ch;      
    }
  }
}

bool MsgServiceClass::isMsgAvailable(Pattern& pattern){
  return (msgAvailable && pattern.match(*currentMsg));
}

Msg* MsgServiceClass::receiveMsg(Pattern& pattern){
  if (msgAvailable && pattern.match(*currentMsg)){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  } 
}