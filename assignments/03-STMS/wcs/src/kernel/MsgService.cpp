#include "kernel/MsgService.hpp"

static String content;
static String topic;
static MsgServiceClass *instance = nullptr;

MsgServiceClass::MsgServiceClass(EventQueue* queue) : EventPublisher(queue) 
{
  instance = this;
  this->init();
}

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
  content.reserve(80);
  topic.reserve(16);
  topic = "";
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceClass::sendMsg(const String& msg){
  if (!Serial) 
  {
    publish(new SerialEvent(new Msg(MsgTopic::MODE, "UNCONNECTED")));
    return;
  }
  int availableBytes = Serial.availableForWrite();
  if (availableBytes < (int)msg.length()) 
  {
    publish(new SerialEvent(new Msg(MsgTopic::MODE, "UNCONNECTED")));
    return;
  }
  Serial.println(msg);
  // waits to send data
  Serial.flush();  
}

void serialEvent() {
  if (instance == nullptr) return;

  if (Serial.available() <= 0) 
  {
    instance->publish(new SerialEvent(new Msg(MsgTopic::MODE, "UNCONNECTED")));
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
      topic.trim();
      topic.toUpperCase();
      if (topic.equalsIgnoreCase("VALVE")) {
        decodedTopic = MsgTopic::VALVE;
      } else if (topic.equalsIgnoreCase("MODE")) {
        decodedTopic = MsgTopic::MODE;
      }
      instance->publish(
        new SerialEvent(new Msg(decodedTopic, content))
      );
      instance->currentMsg = new Msg(decodedTopic, content);
      instance->msgAvailable = true;      
    }
  }
}