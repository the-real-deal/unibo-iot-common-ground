package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;

import it.unibo.sdh.api.model.CommunicationChannel;

public class FetchHangarStateAgent extends Thread implements HangarStateSetter {

    private CommunicationChannel channel;
    private HangarState currentState;
    private List<HangarStateListener> stateChangeListeners;
    
    public FetchHangarStateAgent( 
        final CommunicationChannel channel) {
        
        this.channel = channel;
        this.currentState = HangarState.NORMAL;
        this.stateChangeListeners = new ArrayList<>();
    }

    @Override
    public void run() {
        while (true) {
            final var serialMessage = channel.receiveMessage();
            if (serialMessage.isEmpty()) {
                continue;
            }
            final var retrievedState = HangarState.valueOf(serialMessage.get());
            if (retrievedState != currentState) {
                notifyAll(retrievedState);
                currentState = retrievedState;
            }
        }
    }

    @Override
    public void subscribe(HangarStateListener listener) {
        stateChangeListeners.add(listener);
    }

    @Override
    public boolean unsubscribe(HangarStateListener listener) {
        return stateChangeListeners.remove(listener);
    }

    @Override
    public void notifyAll(HangarState state) {
        stateChangeListeners.forEach(listener -> {
            listener.update(state);
        });
    }
}
