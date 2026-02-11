package it.unibo.iot.cus.commchannel;

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
     * Closes the communication channel, releasing any associated resources.
     * If called multiple times, subsequent calls have no effect.
     */
    void close();
}