package it.unibo.sdh.impl.model;

import it.unibo.sdh.api.model.MessageEncoder;
import it.unibo.sdh.utils.CommunicationChannelUtils;

public class MessageEncoderImpl implements MessageEncoder {

    private StringBuilder messageBuilder;
    private String separator;

    public MessageEncoderImpl(final String separator) {
        this.messageBuilder = new StringBuilder();
        this.separator = separator;
    }

    public MessageEncoderImpl() {
        this(CommunicationChannelUtils.DEFAULT_SEPARATOR);
    }

    @Override
    public MessageEncoder setTopic(String topic) {
        this.messageBuilder.append(topic);
        this.messageBuilder.append(this.separator);
        return this;
    }

    @Override
    public MessageEncoder setContent(String content) {
        this.messageBuilder.append(content);
        this.messageBuilder.append(this.separator);
        return this;
    }

    @Override
    public String build() {
        final var result = this.messageBuilder.toString(); 
        if (result.endsWith(this.separator)) {
            return result.substring(0, result.length() - 1);
        }
        return result;
    }

    @Override
    public void clear() {
        this.messageBuilder = new StringBuilder();
    }
    
}
