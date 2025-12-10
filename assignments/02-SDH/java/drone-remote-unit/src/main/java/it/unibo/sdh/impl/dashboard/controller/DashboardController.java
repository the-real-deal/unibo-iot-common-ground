package it.unibo.sdh.impl.dashboard.controller;

import it.unibo.sdh.api.serial_communication.CommunicationChannel;
import it.unibo.sdh.impl.dashboard.view.DashboardView;
import it.unibo.sdh.impl.serial_communication.SerialCommunicationChannel;

public class DashboardController {

    private CommunicationChannel channel;

    public DashboardController(String arduinoSerialPort, DashboardView view) {
        try {
            channel = new SerialCommunicationChannel(arduinoSerialPort, 9600);
        } catch (final Exception ex) {
            
        }
    }
}
