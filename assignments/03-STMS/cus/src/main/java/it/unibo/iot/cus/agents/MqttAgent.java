package it.unibo.iot.cus.agents;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;
import it.unibo.iot.cus.model.Context;
import it.unibo.iot.cus.model.WaterLevelSampleData;

public class MqttAgent extends AbstractVerticle {

    private static final Logger logger = LoggerFactory.getLogger(MqttAgent.class);
    private MqttClient client;
    private String topic;
    private String clientID;
    private String broker;
    private int port;
    private Context sharedData;
    private String senderID;

    public MqttAgent(
            final String mqttClientID,
            final String mqttTopic,
            final String mqttBroker,
            final int mqttPort,
            final Context sharedData) {
        this.clientID = mqttClientID;
        this.topic = mqttTopic;
        this.port = mqttPort;
        this.sharedData = sharedData;
        this.broker = mqttBroker;
        this.senderID = MqttAgent.class.getName();
    }

    @Override
    public void start() throws Exception {
        this.client = MqttClient.create(vertx, new MqttClientOptions().setClientId(this.clientID));
        client.connect(port, broker, conn -> {
            if (conn.succeeded()) {
                logger.atInfo().log("Connected to broker!");
                client.subscribe(this.topic, MqttQoS.AT_LEAST_ONCE.value(), subscription -> {
                    if (subscription.succeeded()) {
                        logger.info("Subscribed to " + this.topic);
                    } else {
                        logger.error("Subscription failed: " + subscription.cause().getMessage());
                    }
                });

                client.publishHandler(message -> {
                    String topic = message.topicName();
                    String payload = message.payload().toString();
                    logger.info("Received on " + topic + " with payload: " + payload);
                    try {
                        double level = Double.valueOf(payload);
                        this.sharedData.setLastWaterLevelSample(new WaterLevelSampleData(level));
                        vertx.eventBus().publish(
                                "tank.waterlevel",
                                this.senderID.concat(":" + this.sharedData.getLastWaterLevelSample().toString()));
                    } catch (Exception e) {
                        logger.atInfo().log("Error parsing data: " + e.getMessage());
                    }
                });
            } else {
                logger.atError().log("Failed to connect: " + conn.cause().getMessage());
            }
        });
    }

}
