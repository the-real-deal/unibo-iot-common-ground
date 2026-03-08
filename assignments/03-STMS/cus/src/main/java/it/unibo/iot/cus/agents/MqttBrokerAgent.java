package it.unibo.iot.cus.agents;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttEndpoint;
import io.vertx.mqtt.MqttServer;
import io.vertx.mqtt.MqttServerOptions;

public class MqttBrokerAgent extends AbstractVerticle {
	private static final Logger logger = LoggerFactory.getLogger(MqttBrokerAgent.class);
    private MqttServer mqttServer;
    private List<MqttEndpoint> subscribers;
	private int port;

	public MqttBrokerAgent(final int port) {
		this.port = port;
		this.subscribers = new ArrayList<>();
		logger.atInfo().log("new agent created.");
	}
	
	@Override
	public void start() throws Exception {
		this.mqttServer = MqttServer.create(vertx, new MqttServerOptions().setPort(this.port));
		this.mqttServer.endpointHandler(endpoint -> {
			// shows main connect info
			logger.atInfo().log("MQTT client [" + endpoint.clientIdentifier() + "] request to connect, clean session = " + endpoint.isCleanSession());
			
			// add the connected client to the subscribers list,
			// specifying some QoS params (Quality of Service)
			// endpoint.subscribeHandler(newSub -> {
			// 	this.subscribers.add(endpoint);

			// 	final List<MqttQoS> grantedQosLevels = new ArrayList<>();
            //     newSub.topicSubscriptions().forEach(s -> {
            //         logger.info("[BR] Client " + endpoint.clientIdentifier() + " subscribed to " + s.topicName());
            //         grantedQosLevels.add(s.qualityOfService());
            //     });
            //     endpoint.subscribeAcknowledge(newSub.messageId(), grantedQosLevels);
			// });

			// notify all the subscribers of the new message
			// endpoint.publishHandler(msg -> {
			// 	subscribers.forEach(sub -> {
			// 		sub.publish(
			// 			msg.topicName(),
			// 			msg.payload(),
			// 			msg.qosLevel(),
			// 			msg.isDup(),
			// 			msg.isRetain());
			// 		});
					
			// 		if (msg.qosLevel() == MqttQoS.AT_LEAST_ONCE) {
            //         	endpoint.publishAcknowledge(msg.messageId());
            //     	}
			// });

			// removal from subscribers list
			// endpoint.disconnectHandler(v -> {
            //     logger.info("MQTT client disconnected: " + endpoint.clientIdentifier());
            //     subscribers.remove(endpoint);
            // });

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