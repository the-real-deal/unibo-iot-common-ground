#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "kernel/SyncTask.hpp"
#include "WiFi.h"
#include "PubSubClient.h"

enum ConnectionState {
    WIFI_CONNECTING,
    SERVER_CONNECTING,
    CONNECTION_ENABLED
};

class ConnectionMonitoringTask: public SyncTask 
{
    public:
    void tick();
    void init(int basePeriod);
    ConnectionMonitoringTask(Context *pContext);

    private:
    void setState(ConnectionState state);

    StateHolder<ConnectionState> *pTaskState;
    Context *pContext;
};
