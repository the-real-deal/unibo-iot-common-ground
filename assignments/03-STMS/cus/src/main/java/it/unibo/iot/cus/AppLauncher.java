package it.unibo.iot.cus;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import io.vertx.core.Vertx;
import io.vertx.core.eventbus.EventBus;
import it.unibo.iot.cus.agents.CoreAgent;
import it.unibo.iot.cus.agents.HttpAgent;
import it.unibo.iot.cus.agents.MqttAgent;
import it.unibo.iot.cus.agents.MqttBrokerAgent;
import it.unibo.iot.cus.agents.SerialAgent;
import it.unibo.iot.cus.model.Context;
import it.unibo.iot.cus.model.InputMode;
import it.unibo.iot.cus.model.WaterLevelSampleData;

public final class AppLauncher {

    private static final Logger logger = LoggerFactory.getLogger(AppLauncher.class);

    private AppLauncher() {
    }

    // Broadcast to all deployed agents the initial system state
    private static void broadcast(Context ctx, EventBus bus) {
        bus.publish("system.inputmode",
                "AppLauncher:" + String.valueOf(ctx.getInputMode()));
        bus.publish("tank.waterlevel",
                "AppLauncher:" + String.valueOf(ctx.getLastWaterLevelSample()));
        bus.publish("tank.valveopening",
                "AppLauncher:" + String.valueOf(ctx.getValveOpeningPercentage()));
    }

    /**
     * Launches the application.
     */
    public static void main(final String[] args) {
        logger.info("Starting application...");
        Optional<JsonObject> config = Optional.empty();
        try {
            InputStream is = AppLauncher.class.getClassLoader().getResourceAsStream("config.json");
            String content = new String(is.readAllBytes(), StandardCharsets.UTF_8);
            config = Optional.of(new Gson().fromJson(content, JsonObject.class));
        } catch (Exception e) {
            logger.error("Could not load configuration file: " + e.getMessage());
            System.exit(1);
        }
        final var loadedConf = config.get();
        final var vertx = Vertx.vertx();
        final var sharedData = new Context(
                new WaterLevelSampleData(0),
                InputMode.AUTOMATIC,
                0);

        final var mqttProps = loadedConf.get("mqtt").getAsJsonObject();
        final var httpProps = loadedConf.get("http-server").getAsJsonObject();
        final var sysProps = loadedConf.get("sys-params").getAsJsonObject();
        final var serialProps = loadedConf.get("serial").getAsJsonObject();

        // Deployment stage
        vertx.deployVerticle(new MqttBrokerAgent(mqttProps.get("port").getAsInt()))
                .onSuccess(vid -> {
                    logger.info("Deployed MqttBroker agent");

                    vertx.deployVerticle(new CoreAgent(
                            sysProps.get("T1").getAsLong(),
                            sysProps.get("T2").getAsLong(),
                            sysProps.get("L1").getAsDouble(),
                            sysProps.get("L2").getAsDouble(),
                            sharedData.getCopy())).onSuccess(aid -> {
                                logger.info("Deployed CoreAgent");
                                broadcast(sharedData, vertx.eventBus());
                            }).onFailure(msg -> {
                                logger.error("Cannot deploy CoreAgent :/ -" + msg.getMessage());
                            });

                    vertx.deployVerticle(new MqttAgent(
                            mqttProps.get("clientID").getAsString(),
                            mqttProps.get("topic").getAsString(),
                            mqttProps.get("port").getAsInt(),
                            sharedData.getCopy())).onSuccess(aid -> {
                                logger.info("Deployed MqttAgent");
                                broadcast(sharedData, vertx.eventBus());
                            }).onFailure(msg -> {
                                logger.error("Cannot deploy MqttAgent :/ -" + msg.getMessage());
                            });

                    vertx.deployVerticle(new HttpAgent(
                            httpProps.get("port").getAsInt(),
                            sharedData.getCopy())).onSuccess(aid -> {
                                logger.info("Deployed HttpAgent");
                                broadcast(sharedData, vertx.eventBus());
                            }).onFailure(msg -> {
                                logger.error("Cannot deploy HttpAgent :/ -" + msg.getMessage());
                            });

                    vertx.deployVerticle(new SerialAgent(
                            serialProps.get("port").getAsString(),
                            sharedData.getCopy())).onSuccess(aid -> {
                                logger.info("Deployed SerialAgent");
                                broadcast(sharedData, vertx.eventBus());
                            }).onFailure(msg -> {
                                logger.error("Cannot deploy SerialAgent :/ -" + msg.getMessage());
                            });
                    
                    broadcast(sharedData, vertx.eventBus());
                })
                .onFailure((ex) -> {
                    logger.error("Could not start the CUS application!");
                    ex.printStackTrace();
                });
    }
}
