package it.unibo.iot.cus.agents;

import it.unibo.iot.cus.model.Context;
import it.unibo.iot.cus.model.InputMode;
import it.unibo.iot.cus.model.WaterLevelSampleData;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import io.vertx.core.AbstractVerticle;

public class CoreAgent extends AbstractVerticle {
    private long T1, T2;
    private double L1, L2;
    private Context sharedData;
    private String senderID;
    private static final Logger logger = LoggerFactory.getLogger(CoreAgent.class);

    public CoreAgent(final long t1, final long t2, final double l1, final double l2, final Context sharedData) {
        this.T1 = t1; this.T2 = t2;
        this.L1 = l1; this.L2 = l2;
        this.sharedData = sharedData.getCopy();
        this.senderID = CoreAgent.class.getName();
		logger.atInfo().log("new agent created.");
    }

    @Override
    public void start() throws Exception {
        final var msgChannel = vertx.eventBus();
        msgChannel.consumer("tank.waterlevel", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo (here not needed)
            if (sender.equals(this.senderID)) {
                return;
            }
            restartTMSConnectionTimeoutTimer();
            final var waterLevel = Double.valueOf(content.split(":")[1].split("#")[0]);
            this.sharedData.setLastWaterLevelSample(new WaterLevelSampleData(waterLevel));
            logger.atInfo().log("tank.waterlevel updated internal state with: ".concat(content));
            
            // if not in AUTOMATIC mode, the valve is controlled by an operator -
            // so the following logic should not be applied
            if (!sharedData.isSystemInAutomaticMode()) {
                return;
            }

            // if the water level sampled is below L1, then the valve can have any opening level -
            // so the forced 50% or 100% openings should not happen
            if (waterLevel <= L1) {
                restartThresholdTimer();
            } else {
                // if the water level sampled is below L2 (but above L1), only the forced 100% opening should happen
                if (waterLevel > L2) {
                    restartThresholdTimer();
                    emptyTank();
                    return;
                }
            }

        });

        msgChannel.consumer("system.inputmode", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1];
            this.sharedData.setInputMode(InputMode.valueOf(value));
            logger.atInfo().log("system.inputmode updated internal state with: ".concat(value));
        });

        msgChannel.consumer("tank.valveopening", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1];
            this.sharedData.setValveOpeningPercentage(Double.valueOf(value));
            logger.atInfo().log("tank.valveopening updated internal state with: ".concat(value));
        });
        
        restartTMSConnectionTimeoutTimer();
        restartThresholdTimer();
        emptyTank();
    }
    
    private void restartTMSConnectionTimeoutTimer() {
        // Checks connection with TMS
        vertx.setTimer(T2, (timerID) -> {
            this.sharedData.setInputMode(InputMode.UNCONNECTED);
            vertx.eventBus().publish("system.inputmode", this.senderID.concat(":" + this.sharedData.getInputMode().name()));
        });
    }

    private void restartThresholdTimer() {
        // Sets the valve opening at 50% after T1 seconds
        vertx.setTimer(T1, (timerID) -> {
            if (!this.sharedData.isSystemInAutomaticMode()) {
                return;
            }
            this.sharedData.setValveOpeningPercentage(0.5);
            vertx.eventBus().publish("tank.valveopening", this.senderID.concat(":" + String.valueOf(this.sharedData.getValveOpeningPercentage())));
        });
    }

    private void emptyTank() {
        // Sets the valve opening at 100%
        if (!this.sharedData.isSystemInAutomaticMode()) {
            return;
        }
        this.sharedData.setValveOpeningPercentage(1.0);
        vertx.eventBus().publish("tank.valveopening", this.senderID.concat(":" + String.valueOf(this.sharedData.getValveOpeningPercentage())));
    }
}
