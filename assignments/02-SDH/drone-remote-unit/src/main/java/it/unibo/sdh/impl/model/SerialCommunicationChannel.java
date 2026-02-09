package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.EventPublisher;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialCommunicationChannel implements CommunicationChannel, SerialPortEventListener, EventPublisher<String> {

    private final static Logger logger = LoggerFactory.getLogger(SerialCommunicationChannel.class);
    private final static int MAX_MESSAGES_COUNT = 100;
    private StringBuffer currentMessage;
    private SerialPort port;
    private BlockingQueue<String> messageQueue;
    private List<EventListener<String>> msgEventListeners;

    public SerialCommunicationChannel(final String portName, final int baudRate) throws SerialPortException {
        this.currentMessage = new StringBuffer("");
        this.messageQueue = new ArrayBlockingQueue<>(MAX_MESSAGES_COUNT);
        this.msgEventListeners = new ArrayList<>();
        this.port = new SerialPort(portName);
        this.port.openPort();
        this.port.setParams(
                baudRate,
                SerialPort.DATABITS_8,
                SerialPort.STOPBITS_1,
                SerialPort.PARITY_NONE);
        this.port.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
        this.port.addEventListener(this);
    }

    public SerialCommunicationChannel(final String portName) throws SerialPortException {
        this(portName, SerialPort.BAUDRATE_9600);
    }

    @Override
    public void serialEvent(SerialPortEvent serialPortEvent) {
        if (serialPortEvent.isRXCHAR()) {
            try {
                final var rawMessage = port.readString(serialPortEvent.getEventValue());
                currentMessage.append(rawMessage);
            } catch (final SerialPortException ex) { }
            boolean goAhead = true;
            while (goAhead) {
                final var finalMessage = currentMessage.toString();
                final var indexOfNewLine = finalMessage.indexOf("\n");
                if (indexOfNewLine >= 0) {
                    try {
                        messageQueue.put(finalMessage.substring(0, indexOfNewLine));
                    } catch (final InterruptedException ex) { }
                    publishEvent(finalMessage.substring(0, indexOfNewLine));
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
            logger.atInfo().log("|DRU -> SDH| " + message);
        } catch (final SerialPortException ex) { }
    }

    @Override
    public boolean receiveMessage() {
        try {
            messageQueue.take();
            return true;
        } catch (final Exception ex) {
            return false;
        }
    }

    public Optional<String> readMessage() {
        try {
            return Optional.of(messageQueue.element());
        } catch (final Exception ex) {
            return Optional.empty();
        }
    }

    @Override
    public boolean isMessageAvailable() {
        return !messageQueue.isEmpty();
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
    public void subscribe(EventListener<String> listener) {
        msgEventListeners.add(listener);
    }

    @Override
    public boolean unsubscribe(EventListener<String> listener) {
        return msgEventListeners.remove(listener);
    }

    @Override
    public void publishEvent(String data) {
        logger.atDebug().log("Event published! Listeners: " + msgEventListeners.size() + "; string: " + data);
        msgEventListeners.forEach(listener -> {
            listener.update(data);
        });
    }
}
