#include "kernel/SyncTask.hpp"
#include "model/Context.hpp"

enum DataSenderTaskStates { OFFLINE, ONLINE };
class DataSenderTask: public SyncTask {
public:
  DataSenderTask(Lcd* pLcd, Context* pContext); 
  void tick();

private:  
  void setState(DataSenderTaskStates state);
  
  StateHolder<DataSenderTaskStates>* pTaskState;
  Context* pContext;
};