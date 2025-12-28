package it.unibo.sdh.impl.model;

import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import org.slf4j.Logger;

import it.unibo.sdh.api.model.CommunicationChannel;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialCommunicationChannel implements CommunicationChannel, SerialPortEventListener {

    private final static Logger logger = org.slf4j.LoggerFactory.getLogger(SerialCommunicationChannel.class);
    private final static int MAX_MESSAGES_COUNT = 100;
    private StringBuffer currentMessage;
    private SerialPort port;
    private BlockingQueue<String> messageQueue;

    public SerialCommunicationChannel(final String portName, final int baudRate) throws SerialPortException {
        this.currentMessage = new StringBuffer("");
        this.messageQueue = new ArrayBlockingQueue<>(MAX_MESSAGES_COUNT);

        this.port = new SerialPort(portName);
        this.port.openPort();
        this.port.setParams(
                baudRate,
                SerialPort.DATABITS_8,
                SerialPort.STOPBITS_1,
                SerialPort.PARITY_NONE);
        this.port.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
    }

    public SerialCommunicationChannel(final String portName) throws SerialPortException {
        this(portName, SerialPort.BAUDRATE_9600);
    }

    @Override
    public void serialEvent(SerialPortEvent serialPortEvent) {
        if (serialPortEvent.isRXCHAR()) {
            try {
                final var rawMessage = port.readString(serialPortEvent.getEventValue());
                final var sanitizedMessage = rawMessage.replaceAll("\r", "");
                logger.atInfo().log("|SDH -> DRU| " + sanitizedMessage);
                currentMessage.append(sanitizedMessage);
            } catch (final SerialPortException ex) { }
            boolean goAhead = true;
            while (goAhead) {
                final var finalMessage = currentMessage.toString();
                final var indexOfNewLine = finalMessage.indexOf("\n");
                if (indexOfNewLine >= 0) {
                    try {
                        messageQueue.put(finalMessage.substring(0, indexOfNewLine));
                    } catch (final InterruptedException ex) { }
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
        logger.atInfo().log("|DRU -> SDH| " + message);
        final char[] array = (message.concat("\n")).toCharArray();
        final byte[] bytes = new byte[array.length];
        for (int i = 0; i < array.length; i++) {
            bytes[i] = (byte) array[i];
        }
        try {
            synchronized (port) {
                port.writeBytes(bytes);
            }
        } catch (final SerialPortException ex) { }
    }

    @Override
    public Optional<String> receiveMessage() {
        try {
            return Optional.of(messageQueue.take());
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
}
