package it.unibo.sdh.impl.model;

public interface HangarStateSetter {
    
    void subscribe(HangarStateListener listener);

    boolean unsubscribe(HangarStateListener listener);

    void notifyAll(HangarState state);
}
