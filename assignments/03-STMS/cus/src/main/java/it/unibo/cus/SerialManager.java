package it.unibo.cus;

import com.fazecast.jSerialComm.SerialPort;

public class SerialManager {
    private SerialPort comPort;

    public SerialManager(String portName) {
        comPort = SerialPort.getCommPort(portName);
        comPort.setBaudRate(9600);
        comPort.openPort();
    }

    public void sendValveValue(int value) {
        if (comPort.isOpen()) {
            String msg = value + "\n";
            comPort.writeBytes(msg.getBytes(), msg.length());
            System.out.println("[SERIAL] Sent to Arduino: " + msg.trim());
        }
    }
}