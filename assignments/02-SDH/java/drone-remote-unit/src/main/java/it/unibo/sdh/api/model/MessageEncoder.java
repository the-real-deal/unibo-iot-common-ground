package it.unibo.sdh.api.model;

/**
 * Interface for building and encoding messages to be sent through a {@link CommunicationChannel}.
 * 
 * <p>This interface provides a builder pattern for constructing structured messages with topic
 * and content components. The final message is constructed using a delimiter-separated format.</p>
 * 
 * <p>Example usage:
 * <pre>
 * MessageEncoder encoder = new MessageEncoderImpl();
 * String message = encoder.setTopic("DU")
 *                          .setContent("TAKING_OFF")
 *                          .build();
 * </pre>
 * </p>
 */
public interface MessageEncoder {
    
    /**
     * Sets the source component of the message.
     * 
     * <p>The topic typically identifies the subject the message refers to; could be Drone Hangar or Drone Unit.</p>
     * 
     * @param topic the topic identifier; must not be null
     * @return this encoder instance for method chaining
     */
    MessageEncoder setTopic(String topic);
    
    /**
     * Sets the content component of the message.
     * 
     * <p>The content represents the main data or payload of the message.</p>
     * 
     * @param content the message content; must not be null
     * @return this encoder instance for method chaining
     */
    MessageEncoder setContent(String content);
    
    /**
     * Constructs and returns the complete encoded message.
     * 
     * <p>The returned message is formatted as a delimiter-separated string combining the
     * previously set source and content components.</p>
     * 
     * @return the complete encoded message string
     */
    String build();
    
    /**
     * Clears all previously set message components, resetting the encoder to its initial state.
     * 
     * <p>After calling this method, the encoder can be reused to build a new message.</p>
     */
    void clear();
}
