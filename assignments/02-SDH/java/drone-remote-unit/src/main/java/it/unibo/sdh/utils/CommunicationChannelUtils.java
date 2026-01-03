package it.unibo.sdh.utils;

import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import jssc.SerialPortException;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.impl.model.SerialCommunicationChannel;

public class CommunicationChannelUtils {

    public static final String DRONE_PREFIX = "DU"; // Drone Unit
    public static final String HANGAR_PREFIX = "SH"; // Smart Hangar
    public static final String DEFAULT_SEPARATOR = ":";

    private static final Logger logger = LoggerFactory.getLogger(CommunicationChannelUtils.class); 
    
    private static final int MAX_RECONNECTION_ATTEMPTS = 5;
    private static final int RETRY_DELAY_MS = 5000;

    private CommunicationChannelUtils() { }

    public static Optional<CommunicationChannel> tryToConnect(final String port) {
        logger.atInfo().log("Trying to connect to port: ".concat(port));
        logger.atInfo().log("Maximum attempts: ".concat(String.valueOf(MAX_RECONNECTION_ATTEMPTS)));
        for (int attempt = 1; attempt <= MAX_RECONNECTION_ATTEMPTS; attempt++) { 
            try {
                logger.atInfo().log("Connection attempt #".concat(String.valueOf(attempt)));
                final var channel = new SerialCommunicationChannel(port);
                return Optional.of(channel);
            } catch (final SerialPortException ex) { 
                logger.atError().log("NOO! Cannot open port: ".concat(port));
            } catch (final Exception ex) {
                logger.atError().log("Generic exception caught: ".concat(ex.getMessage()));
            }
            
            logger.info("Trying again in " + (RETRY_DELAY_MS / 1000) + "s.");
            ThreadUtils.tryToSleep(RETRY_DELAY_MS);
        }

        return Optional.empty();
    }
}
