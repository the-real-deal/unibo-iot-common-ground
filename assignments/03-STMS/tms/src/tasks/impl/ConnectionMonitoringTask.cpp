#include "tasks/api/ConnectionMonitoringTask.hpp"

void ConnectionMonitoringTask::init(int period)
{
    SyncTask::init(period);
    WiFi.disconnect();
    WiFi.setHostname("TMS - Tank Monitoring Subsystem");
    WiFi.begin("NickolausenMoPho", "password");
    WiFi.mode(WIFI_STA);
    // logger.log();
}

ConnectionMonitoringTask::ConnectionMonitoringTask(Context *pContext)
{
}
void ConnectionMonitoringTask::tick()
{
    // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED)
    {
        // failed, retry
        Serial.print(".");
        delay(5000);
    }

    logger.log("You're connected to the network");
    Serial.println();
    // Attempting to connect to the MQTT broker
    if (!mqttClient.connect(broker, port))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());

        while (1);
    }
    pContext->pGlobalNetworkOk->setState(WiFi.status() == WL_CONNECTED);

    client.setServer(mqtt_server, SERVER_PORT);
    while (true)
    {
        switch (state)
        {
        case WIFI_CONNECTING:
            if (WiFi.status() == WL_CONNECTED)
            {
                state = WIFI_CONNECTED;
            }
            else
            {
                vTaskDelay(500 / TICK_PERIOD_MS);
            }
            break;

        case WIFI_CONNECTED:
            state = MQTT_SERVER_CONNECTING;
            break;

        case MQTT_SERVER_CONNECTING:
            if (client.connect("ESP32_Tank_Client"))
            {
                state = MQTT_SERVER_CONNECTED;
                ledGreen->switchOn();
                ledRed->switchOff();
            }
            else
            {
                state = NET_ERROR;
            }
            break;

        case MQTT_SERVER_CONNECTED:
            if (!client.connected())
            {
                state = MQTT_SERVER_CONNECTING;
                break;
            }
            client.loop();

            float level;
            if (pContext->popDistance(&level))
            {
                char msg[50];
                snprintf(msg, sizeof(msg), "{\"level\": %.2f}", level);
                client.publish("tank/level", msg);
            }
            vTaskDelay(50 / portTICK_PERIOD_MS);
            break;
            vTaskDelay(100 / portTICK_PERIOD_MS);
            break;

        case NET_ERROR:
            ledRed->switchOn();
            ledGreen->switchOff();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            state = WIFI_CONNECTING;
            break;
        }
    }
}