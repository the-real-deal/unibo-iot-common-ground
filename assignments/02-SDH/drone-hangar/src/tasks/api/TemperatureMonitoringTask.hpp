#ifndef __TEMP_MONITOR__
#define __TEMP_MONITOR__

#include "devices/api/TempSensor.hpp"
#include "devices/api/Lcd.hpp"
#include "devices/api/Led.hpp"
#include "devices/api/Button.hpp"
#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "kernel/SyncTask.hpp"
#include "kernel/MsgService.hpp"
#include "config.hpp"

enum TemperatureMonitoringTaskStates { NORMAL_STATE, FIRST_SAMPLING, SECOND_SAMPLING, ALARM };
class TemperatureMonitoringTask: public SyncTask {
public:
    TemperatureMonitoringTask(Lcd* plcd, Led* pled, TempSensor* pTsensor, Button* pButton, Context* pcontext);
    void tick();
    const long SAMPLING_INTERVAL = 1L;
private:
    void setState(TemperatureMonitoringTaskStates newState);
    
    Lcd* pLcd;
    Led* pLed;
    TempSensor* pTsensor;
    Button* pResetButton;
    Context* pContext;
    StateHolder<TemperatureMonitoringTaskStates>* pTaskState;
};

#endif