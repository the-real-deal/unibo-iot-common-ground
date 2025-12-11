package it.unibo.sdh.impl.controller;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.impl.model.Drone;
import it.unibo.sdh.impl.model.Drone.States;
import it.unibo.sdh.impl.model.EventListener;
import it.unibo.sdh.impl.model.FetchHangarStateAgent;
import it.unibo.sdh.impl.model.HangarState;
import it.unibo.sdh.impl.model.SerialCommunicationChannel;
import it.unibo.sdh.impl.view.DashboardView;

public class DashboardController {

    private CommunicationChannel channel;
    private DashboardView view;
    private FetchHangarStateAgent agent;
    private Drone drone;

    public DashboardController(final String arduinoSerialPort, final DashboardView view) {
        try {
            channel = new SerialCommunicationChannel(arduinoSerialPort, 9600);
        } catch (final Exception ex) { }

        drone = new Drone();
        agent = new FetchHangarStateAgent(channel);
        agent.subscribe(new HangarListener());
        agent.start();
    }

    public void takeOff() {
        drone.setState(Drone.States.TAKING_OFF);
    }

    public void land() {
        drone.setState(Drone.States.LANDING);
    }


    private class HangarListener implements EventListener<HangarState> {

        @Override
        public void update(HangarState data) {
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

    // TODO: Use it
    private class DroneListener implements EventListener<Drone.States> {

        @Override
        public void update(States data) {
            throw new UnsupportedOperationException("Unimplemented method 'update'");
        }

    }
}
