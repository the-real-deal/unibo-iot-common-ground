package it.unibo.iot.cus.agents;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttServer;
import io.vertx.mqtt.MqttServerOptions;

public class MqttBrokerAgent extends AbstractVerticle {
	private static final Logger logger = LoggerFactory.getLogger(MqttBrokerAgent.class);
    private MqttServer mqttServer;
	private int port;

	public MqttBrokerAgent(final int port) {
		this.port = port;
		logger.atInfo().log("new agent created.");
	}
	
	@Override
	public void start() throws Exception {
		this.mqttServer = MqttServer.create(vertx, new MqttServerOptions().setPort(this.port));
		this.mqttServer.endpointHandler(endpoint -> {
			// shows main connect info
			logger.atInfo().log("MQTT client [" + endpoint.clientIdentifier() + "] request to connect, clean session = " + endpoint.isCleanSession());
			// accept connection from the remote client
			endpoint.accept(false);
		}).listen(ar -> {
			if (ar.succeeded()) {
				logger.atInfo().log("MQTT server is listening on port " + ar.result().actualPort());
			} else {
				logger.atError().log("Error on starting the MQTT server");
				ar.cause().printStackTrace();
			}
		});
	}
}