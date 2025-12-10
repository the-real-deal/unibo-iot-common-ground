package it.unibo.sdh.api.serial_communication;

import java.util.Optional;

public interface CommunicationChannel {

    void sendMessage(String message);

    Optional<String> receiveMessage();

    boolean isChannelAvailable();

    void close();
}