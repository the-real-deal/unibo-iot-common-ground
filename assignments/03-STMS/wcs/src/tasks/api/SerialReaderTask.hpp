#include "kernel/SyncTask.hpp"
#include "kernel/MsgService.hpp"
#include "model/Context.hpp"
#include "devices/api/Lcd.hpp"

enum SerialReaderTaskStates { OFFLINE, ONLINE };
class SerialReaderTask: public SyncTask {
public:
  SerialReaderTask(Lcd* pLcd, Context* pContext); 
  void tick();

private:  
  void setState(SerialReaderTaskStates state);
  
  Lcd* pLcd;
  StateHolder<SerialReaderTaskStates>* pTaskState;
  Context* pContext;
};