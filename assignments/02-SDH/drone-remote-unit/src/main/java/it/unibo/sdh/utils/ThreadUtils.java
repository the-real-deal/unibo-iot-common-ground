package it.unibo.sdh.utils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ThreadUtils {
    private ThreadUtils() {}

    private final static Logger logger = LoggerFactory.getLogger(ThreadUtils.class);
    
    public static void tryToSleep(final long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            logger.atError().log("Something went wrong when trying to wait " + ms + " millis. Continuing...");
            e.printStackTrace();
        }
    }
}
