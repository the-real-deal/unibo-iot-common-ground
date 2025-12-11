package it.unibo.sdh.impl.controller;

import java.util.Objects;
import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.impl.model.DroneStates;
import it.unibo.sdh.impl.model.FetchHangarStateAgent;
import it.unibo.sdh.impl.model.HangarStates;
import it.unibo.sdh.impl.model.SerialCommunicationChannel;
import it.unibo.sdh.impl.model.StateHolder;
import it.unibo.sdh.impl.view.DashboardView;
import jssc.SerialPortException;

public class DashboardController {

    private final static Logger logger = LoggerFactory.getLogger(DashboardController.class);
    private static final int MAX_RECONNECTION_ATTEMPTS = 5;

    private CommunicationChannel channel;

    private StateHolder<HangarStates> hangar;
    private HangarListener hangarListener;
    private FetchHangarStateAgent hangarAgent;
    
    private StateHolder<DroneStates> drone;
    private DroneListener droneListener;
    
    private DashboardView view;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        boolean arduinoConnected = false;
        logger.atInfo().log("Trying to reach Arduino on port: ".concat(arduinoSerialPort));
        for (int attempt = 0; attempt < MAX_RECONNECTION_ATTEMPTS && !arduinoConnected; attempt++) {
            try {
                channel = new SerialCommunicationChannel(arduinoSerialPort);
            } catch (final SerialPortException ex) { 
                logger.atError().log("NOO! Cannot open port: ".concat(arduinoSerialPort));
            } catch (final Exception ex) {
                logger.atError().log("Generic exception caught: ".concat(ex.getMessage()));
            } finally {
                arduinoConnected = !Objects.isNull(channel);
                if (!arduinoConnected) {
                    logger.info("Trying again in 5s, attempt #"
                        .concat(String.valueOf(attempt + 1) + "/" + String.valueOf(MAX_RECONNECTION_ATTEMPTS)));
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        logger.atError().log("Something went wrong when waiting...");
                        System.exit(-1);
                    }
                }
            }
        }

        if (!arduinoConnected) {
            logger.atError().log("Arduino not detected. Exiting...");
            System.exit(-1);
        }

        this.view = view;

        this.hangar = new StateHolder<HangarStates>();
        this.hangarListener = new HangarListener();
        this.hangarAgent = new FetchHangarStateAgent(channel, hangar);
        this.hangarAgent.subscribe(hangarListener);
        this.hangarAgent.start();
        
        this.drone = new StateHolder<DroneStates>(DroneStates.REST);
        this.droneListener = new DroneListener();
    }

    public void takeOff() {
        this.drone.setState(DroneStates.TAKING_OFF);
        droneListener.update(this.drone);
    }

    public void land() {
        this.drone.setState(DroneStates.LANDING);
        droneListener.update(this.drone);
    }

    public void resetHangar() {
        this.hangar.setState(HangarStates.NORMAL);
        hangarListener.update(this.hangar);
    }

    public Optional<HangarStates> getHangarState() {
        return hangar.getCurrentState();
    }

    public Optional<DroneStates> getDroneState() {
        return drone.getCurrentState();
    }

    private class HangarListener implements EventListener<StateHolder<HangarStates>> {

        @Override
        public void update(final StateHolder<HangarStates> data) {
            if (data.getCurrentState().isEmpty()) {
                return;
            }
            view.displayHangarState(data.getCurrentState().get().name());
            switch (data.getCurrentState().get()) {
                case NORMAL:
                    view.enableActionButtons();
                    break;
                case PRE_ALARM:
                    view.disableActionButtons();
                    break;
                case ALARM:
                    // do something very serious
                    break;
                default:
                    break;
            }
        }

    }

    private class DroneListener implements EventListener<StateHolder<DroneStates>> {

        @Override
        public void update(final StateHolder<DroneStates> data) {
            if (data.getCurrentState().isEmpty()) {
                return;
            }
            channel.sendMessage(data.getCurrentState().get().name());
            view.displayDroneState(data.getCurrentState().get().name());
        }

    }
}
