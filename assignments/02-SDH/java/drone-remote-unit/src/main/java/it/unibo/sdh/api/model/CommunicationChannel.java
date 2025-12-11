package it.unibo.sdh.api.model;

import java.util.Optional;

public interface CommunicationChannel {

    void sendMessage(String message);

    Optional<String> receiveMessage();

    boolean isAvailable();

    void close();
}