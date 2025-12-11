package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.EventPublisher;

public class FetchHangarStateAgent extends Thread implements EventPublisher<StateHolder<HangarStates>> {

    private CommunicationChannel channel;
    private StateHolder<HangarStates> state;
    private List<EventListener<StateHolder<HangarStates>>> stateChangeListeners;
    
    public FetchHangarStateAgent(final CommunicationChannel channel, final StateHolder<HangarStates> state) {
        this.channel = channel;
        this.stateChangeListeners = new ArrayList<>();
        this.state = state;
        notifyAll(state);
    }

    @Override
    public void run() {
        while (true) {
            synchronized (channel) {
                final var serialMessage = channel.receiveMessage();
                if (serialMessage.isEmpty()) {
                    // no message read, so move on to the next iteration
                    continue;
                }
                HangarStates fetchedState;
                try {
                    fetchedState = HangarStates.valueOf(serialMessage.get());
                } catch (IllegalArgumentException ex) {
                    // read message not relative to hangar state, so ignore it and keep going
                    continue;
                }
                synchronized (state) {
                    if (state.getCurrentState().isEmpty() || fetchedState != state.getCurrentState().get()) {
                        state.setState(fetchedState);
                        notifyAll(state);
                    }
                }
            }
        }
    }

    @Override
    public void subscribe(EventListener<StateHolder<HangarStates>> listener) {
        stateChangeListeners.add(listener);
    }

    @Override
    public boolean unsubscribe(EventListener<StateHolder<HangarStates>> listener) {
        return stateChangeListeners.remove(listener);
    }

    @Override
    public void notifyAll(StateHolder<HangarStates> data) {
        stateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }
}
