package it.unibo.sdh.api.model;

import java.util.Optional;

/**
 * Interface for decoding messages received from a {@link CommunicationChannel}.
 * 
 * <p>This interface provides methods to parse and extract information from serialized messages.
 * Messages are typically structured with delimiters to separate different components such as source,
 * content, and value.</p>
 * 
 * <p>Example message format: {@code SOURCE:CONTENT:VALUE}</p>
 */
public interface MessageDecoder {
    
    /**
     * Sets the message to be decoded.
     * 
     * @param message the raw message string to decode; must not be null
     */
    void setMessage(String message);
    
    /**
     * Retrieves the currently set message.
     * 
     * @return an {@code Optional} containing the message, or {@code Optional.empty()} if no message has been set
     */
    Optional<String> getMessage();
    
    /**
     * Extracts the content portion of the message (the middle part between first and last delimiters).
     * 
     * @return an {@code Optional} containing the content, or {@code Optional.empty()} if the message or delimiter is not set
     */
    Optional<String> getContent();
    
    /**
     * Extracts the source portion of the message (the part before the first delimiter).
     * 
     * @return an {@code Optional} containing the source, or {@code Optional.empty()} if the message or delimiter is not set
     */
    Optional<String> getSource();
    
    /**
     * Extracts the value portion of the message (the part after the last delimiter).
     * 
     * @return an {@code Optional} containing the value, or {@code Optional.empty()} if the message or delimiter is not set
     */
    Optional<String> getValue();
}
