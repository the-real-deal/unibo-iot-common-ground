package it.unibo.sdh.api.model;

import java.util.Optional;

public interface CommunicationChannel {

    /**
     * Sends a message through the communication channel.
     *
     * @param message the message to send
     * @throws IllegalStateException if the channel is not available or closed
     * @throws NullPointerException if the message is null
     */
    void sendMessage(String message);

    /**
     * Receives a message from the channel.
     * Returns {@code Optional.empty()} if no message is available or the channel is closed.
     *
     * @return an {@code Optional} containing the received message, or {@code Optional.empty()} if none is available or the channel is closed
     */
    Optional<String> receiveMessage();

    /**
     * Checks if the communication channel is available.
     *
     * @return true if the channel is available, false otherwise
     */
    boolean isMessageAvailable();

    /**
     * Closes the communication channel, releasing any associated resources.
     * If called multiple times, subsequent calls have no effect.
     */
    void close();
}