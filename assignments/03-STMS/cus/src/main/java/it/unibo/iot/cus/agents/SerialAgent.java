package it.unibo.iot.cus.agents;

import java.util.Objects;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.vertx.core.AbstractVerticle;
import it.unibo.iot.cus.commchannel.CommunicationChannel;
import it.unibo.iot.cus.model.Context;
import it.unibo.iot.cus.model.InputMode;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialAgent extends AbstractVerticle implements SerialPortEventListener, CommunicationChannel {
    private final static Logger logger = LoggerFactory.getLogger(SerialAgent.class);
    private StringBuffer currentMessage;
    private SerialPort port;
    private Context sharedData;
    private String senderID;

    public SerialAgent(
            final String portName,
            final int baudRate,
            final Context sharedData) {
        try {
            this.port = new SerialPort(portName);
            this.port.openPort();
            this.port.setParams(
                    baudRate,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            this.port.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
            this.port.addEventListener(this);
        } catch (SerialPortException e) {
            logger.error("Could not open serial port " + portName + "!");
        }
        this.currentMessage = new StringBuffer("");
        this.senderID = SerialAgent.class.getName();
        this.sharedData = sharedData;
    }

    public SerialAgent(final String portName, final Context sharedData) {
        this(portName, SerialPort.BAUDRATE_9600, sharedData);
    }

    @Override
    public void serialEvent(SerialPortEvent serialPortEvent) {
        // Avoiding events before the vertx is deployed
        if (Objects.isNull(vertx)) {
            return;
        }

        if (serialPortEvent.isRXCHAR()) {
            final var msgChannel = vertx.eventBus();
            try {
                final var rawMessage = port.readString(serialPortEvent.getEventValue());
                currentMessage.append(rawMessage);
            } catch (final SerialPortException ex) { }
            boolean goAhead = true;
            while (goAhead) {
                final var finalMessage = currentMessage.toString();
                final var indexOfNewLine = finalMessage.indexOf("\n");
                if (indexOfNewLine >= 0) {
                    final var msg = finalMessage.substring(0, indexOfNewLine);
                    if (msg.contains("VALVE")) {
                        msgChannel.publish(
                                "tank.valveopening",
                                senderID + ":" + msg.trim().split(":")[1]);
                    } else if (msg.contains("MODE")) {
                        msgChannel.publish(
                                "system.inputmode",
                                senderID + ":" + msg.trim().split(":")[1]);
                    }
                    currentMessage = new StringBuffer("");
                    if (indexOfNewLine + 1 < finalMessage.length()) {
                        currentMessage.append(finalMessage.substring(indexOfNewLine + 1));
                    }
                } else {
                    goAhead = false;
                }
            }
        }
    }

    @Override
    public void sendMessage(final String message) {
        final char[] array = (message.concat("\n")).toCharArray();
        final byte[] bytes = new byte[array.length];
        for (int i = 0; i < array.length; i++) {
            bytes[i] = (byte) array[i];
        }
        try {
            synchronized (port) {
                port.writeBytes(bytes);
            }
            logger.atInfo().log("SERIAL-SEND: " + message);
        } catch (final SerialPortException ex) {
            logger.atError().log("Something wrong :/ — " + ex.getMessage());
        }
    }

    @Override
    public void close() {
        try {
            if (!Objects.isNull(port)) {
                port.removeEventListener();
                port.closePort();
            }
        } catch (final Exception ex) { }
    }

    @Override
    public void start() throws Exception {
        final var msgChannel = vertx.eventBus();
        msgChannel.consumer("system.inputmode", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0].trim();
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1].trim().toUpperCase();
            this.sharedData.setInputMode(InputMode.valueOf(value));
            sendMessage("MODE:".concat(value));
            logger.atInfo().log("system.inputmode updated internal state with: ".concat(value));
        });
        msgChannel.consumer("tank.valveopening", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0].trim();
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1].trim();
            this.sharedData.setValveOpeningPercentage(Double.valueOf(value));
            sendMessage("VALVE:".concat(value));
            logger.atInfo().log("tank.valveopening updated internal state with: ".concat(value));
        });
    }
}
