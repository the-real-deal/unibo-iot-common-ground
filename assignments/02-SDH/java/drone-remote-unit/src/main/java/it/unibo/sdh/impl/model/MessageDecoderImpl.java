package it.unibo.sdh.impl.model;

import java.util.Optional;

import it.unibo.sdh.api.model.MessageDecoder;
import it.unibo.sdh.utils.CommunicationChannelUtils;

public class MessageDecoderImpl implements MessageDecoder {

    private Optional<String> message = Optional.empty();
    private String separator = CommunicationChannelUtils.DEFAULT_SEPARATOR;

    public MessageDecoderImpl(final String message, final String separator) {
        setMessage(message);
        this.separator = separator;
    }

    public MessageDecoderImpl(final String message) {
        this(message, CommunicationChannelUtils.DEFAULT_SEPARATOR);
    }

    public MessageDecoderImpl() { }

    @Override
    public void setMessage(String message) {
        this.message = Optional.of(message);
    }

    @Override
    public Optional<String> getMessage() {
        return message;
    }

    @Override
    public Optional<String> getContent() {
        if (message.isEmpty()) {
            return Optional.empty();
        }

        final var msg = message.get();

        return Optional.of(
            msg.substring(
                msg.indexOf(separator) + 1,
                msg.lastIndexOf(separator)
            )
        );
    }

    @Override
    public Optional<String> getSource() {
        if (message.isEmpty()) {
            return Optional.empty();
        }

        final var msg = message.get();
        return Optional.of(
            msg.substring(
                0,
                msg.indexOf(separator)
            )
        );
    }

    @Override
    public Optional<String> getValue() {
        if (message.isEmpty()) {
            return Optional.empty();
        }

        final var msg = message.get();
        final int startIndex = msg.lastIndexOf(separator);
        if (startIndex == -1) {
            return Optional.empty();
        }

        return Optional.of(
            msg.substring(
                startIndex + 1
            )
        );
    }
}
