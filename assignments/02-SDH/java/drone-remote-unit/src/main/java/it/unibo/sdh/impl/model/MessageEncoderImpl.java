package it.unibo.sdh.impl.model;

import it.unibo.sdh.api.model.MessageEncoder;
import it.unibo.sdh.utils.CommunicationChannelUtils;

public class MessageEncoderImpl implements MessageEncoder {

    private StringBuilder messageBuilder;
    private String delimiter;

    public MessageEncoderImpl(final String delimiter) {
        this.messageBuilder = new StringBuilder();
        this.delimiter = delimiter;
    }

    public MessageEncoderImpl() {
        this(CommunicationChannelUtils.DEFAULT_SEPARATOR);
    }

    @Override
    public MessageEncoder setSource(String source) {
        this.messageBuilder.append(source);
        this.messageBuilder.append(this.delimiter);
        return this;
    }

    @Override
    public MessageEncoder setContent(String content) {
        this.messageBuilder.append(content);
        this.messageBuilder.append(this.delimiter);
        return this;
    }

    @Override
    public String build() {
        final var result = this.messageBuilder.toString(); 
        if (result.endsWith(this.delimiter)) {
            return result.substring(0, result.length() - 1);
        }
        return result;
    }

    @Override
    public void clear() {
        this.messageBuilder = new StringBuilder();
    }
    
}
