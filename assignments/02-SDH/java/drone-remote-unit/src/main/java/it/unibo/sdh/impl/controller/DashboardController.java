package it.unibo.sdh.impl.controller;

import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.model.DroneStates;
import it.unibo.sdh.impl.model.FetchHangarStateAgent;
import it.unibo.sdh.impl.model.HangarStates;
import it.unibo.sdh.impl.model.StateHolderImpl;
import it.unibo.sdh.impl.view.DashboardView;
import it.unibo.sdh.utils.CommunicationChannelUtils;

public class DashboardController {

    private final static Logger logger = LoggerFactory.getLogger(DashboardController.class);
    private static final String DRONE_PREFIX = "DU:"; // Drone Unit
    private static final String HANGAR_PREFIX = "SH:"; // Smart Hangar

    private CommunicationChannel channel;

    private StateHolder<HangarStates> hangar;
    private HangarListener hangarListener;
    private FetchHangarStateAgent hangarAgent;
    
    private StateHolder<DroneStates> drone;
    private DroneListener droneListener;
    
    private DashboardView view;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        
        CommunicationChannelUtils.tryToConnect(arduinoSerialPort)
            .ifPresentOrElse(channel -> {
                this.channel = channel;
            }, () -> {
                logger.atError().log("Couldn't find Arduino. Exiting...");
                System.exit(-1);
            });

        this.view = view;

        this.hangar = new StateHolderImpl<HangarStates>();
        this.hangarListener = new HangarListener();
        this.hangarAgent = new FetchHangarStateAgent(channel, hangar);
        this.hangarAgent.subscribe(hangarListener);
        this.hangarAgent.start();
        
        this.drone = new StateHolderImpl<DroneStates>(DroneStates.REST);
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
            final var newState = data.getCurrentState().get();
            view.displayHangarState(newState.name());
            switch (newState) {
                case NORMAL:
                    view.enableActionButtons();
                    view.disableResetButton();
                    break;
                case PRE_ALARM:
                    view.disableActionButtons();
                    break;
                case ALARM:
                    view.enableResetButton();
                    view.disableActionButtons();
                    drone.getCurrentState().ifPresent(state -> {
                        if (state == DroneStates.OPERATING) {
                            // if drone is flying, it gets notified.
                            channel.sendMessage(HANGAR_PREFIX.concat(newState.name()));
                        } 
                    });
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
            final var newState = data.getCurrentState().get();
            channel.sendMessage(DRONE_PREFIX.concat(newState.name()));
            view.displayDroneState(newState.name());
        }

    }
}
