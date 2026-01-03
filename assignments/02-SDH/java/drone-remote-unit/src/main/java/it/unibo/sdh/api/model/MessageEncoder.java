package it.unibo.sdh.api.model;

/**
 * Interface for building and encoding messages to be sent through a {@link CommunicationChannel}.
 * 
 * <p>This interface provides a builder pattern for constructing structured messages with source
 * and content components. The final message is constructed using a delimiter-separated format.</p>
 * 
 * <p>Example usage:
 * <pre>
 * MessageEncoder encoder = new MessageEncoderImpl();
 * String message = encoder.setSource("DU")
 *                          .setContent("TAKING_OFF")
 *                          .build();
 * </pre>
 * </p>
 */
public interface MessageEncoder {
    
    /**
     * Sets the source component of the message.
     * 
     * <p>The source typically identifies the origin or identifier of the message sender.</p>
     * 
     * @param source the source identifier; must not be null
     * @return this encoder instance for method chaining
     */
    MessageEncoder setSource(String source);
    
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
