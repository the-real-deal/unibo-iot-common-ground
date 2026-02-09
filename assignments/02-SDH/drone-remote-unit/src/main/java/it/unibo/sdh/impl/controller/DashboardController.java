package it.unibo.sdh.impl.controller;

import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.MessageEncoder;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.model.MessageEncoderImpl;
import it.unibo.sdh.impl.model.MonitoringStateAgent;
import it.unibo.sdh.impl.model.SerialCommunicationChannel;
import it.unibo.sdh.impl.model.StateHolderImpl;
import it.unibo.sdh.impl.model.drone.DroneListener;
import it.unibo.sdh.impl.model.drone.DroneStates;
import it.unibo.sdh.impl.model.hangar.HangarListener;
import it.unibo.sdh.impl.model.hangar.HangarStates;
import it.unibo.sdh.impl.view.DashboardView;
import it.unibo.sdh.utils.CommunicationChannelUtils;
import it.unibo.sdh.utils.Pair;

public class DashboardController {

    private final static Logger logger = LoggerFactory.getLogger(DashboardController.class);
    private SerialCommunicationChannel channel;
    private MessageEncoder encoder;
    private StateHolder<HangarStates> hangar;
    private StateHolder<Pair<DroneStates, Optional<String>>> drone;
    private DashboardView view;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        
        CommunicationChannelUtils.tryToConnect(arduinoSerialPort)
            .ifPresentOrElse(channel -> {
                this.channel = (SerialCommunicationChannel)channel;
                logger.atInfo().log("Arduino connected!");
            }, () -> {
                logger.atError().log("Couldn't find Arduino. Exiting...");
                System.exit(-1);
            });

        this.encoder = new MessageEncoderImpl();
        this.view = view;

        final var monitoringAgent = new MonitoringStateAgent();

        monitoringAgent.subscribeHangarListener(new HangarListener(this));
        monitoringAgent.subscribeDroneListener(new DroneListener(this));
        this.hangar = new StateHolderImpl<>(HangarStates.NORMAL);
        monitoringAgent.publishHangarEvent(hangar);
        this.drone = new StateHolderImpl<>(new Pair<>(DroneStates.REST, Optional.empty()));
        monitoringAgent.publishDroneEvent(drone);
        
        this.channel.subscribe(monitoringAgent);
    }

    public void requestTakingOff() {
        final var msg = encoder
            .setTopic(CommunicationChannelUtils.DRONE_PREFIX)
            .setContent(DroneStates.TAKING_OFF.name())
            .build();
        channel.sendMessage(msg);
        encoder.clear();
    }

    public void requestLanding() {
        final var msg = encoder
            .setTopic(CommunicationChannelUtils.DRONE_PREFIX)
            .setContent(DroneStates.LANDING.name())
            .build();
        channel.sendMessage(msg);
        encoder.clear();
    }

    public void enableActionButtons() {
        view.enableActionButtons();
    }

    public void disableActionButtons() {
        view.disableActionButtons();
    }
    
    public void displayDroneState(final String state) {
        view.displayDroneState(state);
    }

    public void displayHangarState(final String state) {
        view.displayHangarState(state);
    }

    public void displayDroneDistance(final Double distance) {
        view.displayDroneDistance(String.valueOf(distance));
    }

    public void notifyHangarInAlarm() {
        view.notifyHangarInAlarm();
    }

    public void clearHangarInAlarmNotification() {
        view.clearHangarInAlarmNotification();
    }

    public void clearDroneDistanceDisplay() {
        view.clearDroneDistanceDisplay();
    }
}
