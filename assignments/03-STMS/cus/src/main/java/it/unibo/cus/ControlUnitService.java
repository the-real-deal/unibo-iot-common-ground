package it.unibo.cus;

import io.vertx.core.AbstractVerticle;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import org.eclipse.paho.client.mqttv3.*;

public class ControlUnitService extends AbstractVerticle {
    private DataState state = new DataState();
    private MqttClient mqttClient;
    private SerialManager serial;
    private long t1_TimerID = -1;

    private final double L1 = 40.0, L2 = 80.0;
    private final long T1 = 5000, T2 = 10000;

    @Override
    public void start() throws Exception {
        try {
            serial = new SerialManager("COM3");
        } catch (Exception e) {
            System.err.println("[CUS] Serial not found on COM3.");
        }
        
        setupMQTT();

        Router router = Router.router(vertx);
        router.route().handler(CorsHandler.create("*")); 
        router.route().handler(BodyHandler.create());
        
        //Status of the system
        router.get("/api/status").handler(ctx -> {
            ctx.response().putHeader("content-type", "application/json").end(state.toJson().encode());
        });

        //Input mode of the system
        router.post("/api/mode").handler(ctx -> {
            state.mode = ctx.getBodyAsJson().getString("mode");
            ctx.response().end();
        });

        vertx.createHttpServer().requestHandler(router).listen(8081, ar -> {
            if (ar.succeeded()) {
                System.out.println("[CUS] HTTP Server started on port 8081");
            }
        });

        // Watchdog per UNCONNECTED visto che non vaaaaa
        vertx.setPeriodic(1000, id -> {
            if (System.currentTimeMillis() - state.lastSeenTMS > T2) {
                state.status = "UNCONNECTED";
            }
        });
    }

    private void setupMQTT() throws Exception {
        this.mqttClient = new MqttClient("tcp://broker.mqtt-dashboard.com:1883", "CUS-" + System.currentTimeMillis());
        mqttClient.setCallback(new MqttCallback() {
            @Override
            public void messageArrived(String topic, MqttMessage msg) {
                vertx.runOnContext(v -> {
                    state.lastSeenTMS = System.currentTimeMillis(); // Aggiorna tempo
                    state.status = "NORMAL";                       // Torna Normal
                    state.waterLevel = Double.parseDouble(new String(msg.getPayload()));
                    processLogic();
                });
            }
            @Override public void connectionLost(Throwable t) {}
            @Override public void deliveryComplete(IMqttDeliveryToken t) {}
        });
        mqttClient.connect();
        mqttClient.subscribe("it/unibo/esiot/2026/tank/level");
    }

    private void processLogic() {
        if (state.mode.equals("MANUAL")) return;

        if (state.waterLevel >= L2) {
            cancelT1();
            updateValve(100);
        } else if (state.waterLevel >= L1) {
            if (t1_TimerID == -1) { 
                t1_TimerID = vertx.setTimer(T1, id -> {
                    updateValve(50);
                    t1_TimerID = -1; 
                });
            }
        } else {
            cancelT1();
            updateValve(0);
        }
    }

    private void updateValve(int val) {
        state.valveValue = val;
        if (serial != null) serial.sendValveValue(val);
    }

    private void cancelT1() {
        if (t1_TimerID != -1) {
            vertx.cancelTimer(t1_TimerID);
            t1_TimerID = -1;
        }
    }

    @Override
    public void stop() throws Exception {
        if (mqttClient != null && mqttClient.isConnected()) mqttClient.disconnect();
    }
}