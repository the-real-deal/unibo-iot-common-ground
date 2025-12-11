package it.unibo.sdh.impl.controller;

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

    final static Logger logger = LoggerFactory.getLogger(DashboardController.class);

    private CommunicationChannel channel;
    private HangarListener hangarListener;
    private FetchHangarStateAgent hangarAgent;
    private StateHolder<DroneStates> drone;
    private StateHolder<HangarStates> hangar;
    private DroneListener droneListener;

    private DashboardView view;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        try {
            channel = new SerialCommunicationChannel(arduinoSerialPort);
        } catch (final SerialPortException ex) { 
            logger.atError().log("NOO! Cannot open port: ".concat(arduinoSerialPort));
        } catch (final Exception ex) {
            logger.atError().log("Generic exception caught: ".concat(ex.getMessage()));
        }

        this.view = view;

        this.hangar = new StateHolder<HangarStates>(HangarStates.NORMAL);
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

    public HangarStates getHangarState() {
        return hangar.getCurrentState();
    }

    public DroneStates getDroneState() {
        return drone.getCurrentState();
    }

    private class HangarListener implements EventListener<StateHolder<HangarStates>> {

        @Override
        public void update(final StateHolder<HangarStates> data) {
            view.displayHangarState(data.getCurrentState().name());
            switch (data.getCurrentState()) {
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
            drone.setState(data.getCurrentState());
            channel.sendMessage(data.getCurrentState().name());
            view.displayDroneState(data.getCurrentState().name());
        }

    }
}
