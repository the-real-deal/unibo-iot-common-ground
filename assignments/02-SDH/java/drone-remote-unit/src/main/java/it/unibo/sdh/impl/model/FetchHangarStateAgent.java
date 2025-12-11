package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.EventPublisher;

public class FetchHangarStateAgent extends Thread implements EventPublisher<HangarState> {

    private CommunicationChannel channel;
    private HangarState currentState;
    private List<EventListener<HangarState>> stateChangeListeners;
    
    public FetchHangarStateAgent(final CommunicationChannel channel) {
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
            final var fetchedState = HangarState.valueOf(serialMessage.get());
            if (fetchedState != currentState) {
                notifyAll(fetchedState);
                currentState = fetchedState;
            }
        }
    }

    @Override
    public void subscribe(EventListener<HangarState> listener) {
        stateChangeListeners.add(listener);
    }

    @Override
    public boolean unsubscribe(EventListener<HangarState> listener) {
        return stateChangeListeners.remove(listener);
    }

    @Override
    public void notifyAll(HangarState data) {
        stateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }
}
