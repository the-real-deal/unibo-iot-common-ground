#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "kernel/Logger.hpp"
#include "kernel/SyncTask.hpp"
#include "WiFiNINA.h"
#include "ArduinoMqttClient.h"
#include "secrets.hpp"

class ConnectionMonitoringTask: public SyncTask 
{
    public:
    void tick();
    void init(int basePeriod);
    ConnectionMonitoringTask(Context *pContext);

    private:
    Context *pContext;
};
