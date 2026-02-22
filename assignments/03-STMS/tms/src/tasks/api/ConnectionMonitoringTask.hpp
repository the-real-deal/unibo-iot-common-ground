#include "model/Context.hpp"
#include "model/StateHolder.hpp"
#include "model/ConnectionProvider.hpp"
#include "kernel/Logger.hpp"
#include "kernel/SyncTask.hpp"

enum ConnectionState {
    WIFI_CONNECTING,
    SERVER_CONNECTING,
    CONNECTION_ENABLED
};

class ConnectionMonitoringTask: public SyncTask 
{
public:
    void tick();
    ConnectionMonitoringTask(Context *pContext);

private:
    void setState(ConnectionState state);
    StateHolder<ConnectionState> *pTaskState;
    Context *pContext;
};
