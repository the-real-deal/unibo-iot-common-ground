package it.unibo.sdh.impl.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.impl.model.DroneContext;
import it.unibo.sdh.impl.model.FetchHangarStateAgent;
import it.unibo.sdh.impl.model.HangarState;
import it.unibo.sdh.impl.model.SerialCommunicationChannel;
import it.unibo.sdh.impl.view.DashboardView;
import jssc.SerialPortException;

public class DashboardController {

    final static Logger logger = LoggerFactory.getLogger(DashboardController.class);

    private CommunicationChannel channel;
    private DashboardView view;
    private HangarListener hangarListener;
    private FetchHangarStateAgent hangarAgent;
    private DroneContext drone;
    private DroneListener droneListener;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        try {
            channel = new SerialCommunicationChannel(arduinoSerialPort);
        } catch (final SerialPortException ex) { 
            logger.atError().log("NOO! Cannot open port: ".concat(arduinoSerialPort));
        } catch (final Exception ex) {
            logger.atError().log("Generic exception caught: ".concat(ex.getMessage()));
        }

        this.view = view;

        this.hangarListener = new HangarListener();
        this.hangarAgent = new FetchHangarStateAgent(channel);
        this.hangarAgent.subscribe(hangarListener);
        this.hangarAgent.start();
        
        this.drone = new DroneContext();
        this.droneListener = new DroneListener();
    }

    public void takeOff() {
        droneListener.update(DroneContext.States.TAKING_OFF);
    }

    public void land() {
        droneListener.update(DroneContext.States.LANDING);
    }

    public void resetHangar() {
        hangarListener.update(HangarState.NORMAL);
    }

    private class HangarListener implements EventListener<HangarState> {

        @Override
        public void update(final HangarState data) {
            view.displayHangarState(data.name());
            switch (data) {
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

    private class DroneListener implements EventListener<DroneContext.States> {

        @Override
        public void update(final DroneContext.States data) {
            drone.setState(data);
            channel.sendMessage(data.name());
            view.displayDroneState(data.name());
        }

    }
}
