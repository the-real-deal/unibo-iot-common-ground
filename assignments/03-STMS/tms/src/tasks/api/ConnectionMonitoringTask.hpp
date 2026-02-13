#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "kernel/SyncTask.hpp"
#include "WiFi.h"
#include "PubSubClient.h"

class ConnectionMonitoringTask: public SyncTask 
{
    public:
    void tick();
    void init(int basePeriod);
    ConnectionMonitoringTask(Context *pContext);

    private:
    Context *pContext;
};
