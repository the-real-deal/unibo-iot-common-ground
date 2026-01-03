package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.EventPublisher;
import it.unibo.sdh.api.model.StateHolder;

public abstract class MonitoringStateAgent<S> extends Thread implements EventPublisher<StateHolder<S>> {

    private List<EventListener<StateHolder<S>>> stateChangeListeners;
    private CommunicationChannel channel;
    private StateHolder<S> state;

    public MonitoringStateAgent(final CommunicationChannel channel, final StateHolder<S> state) {
        this.channel = channel;
        this.stateChangeListeners = new ArrayList<>();
        this.state = state;
        publishEvent(state);
    }

    public CommunicationChannel getChannel() {
        return this.channel;
    }

    public StateHolder<S> getCurrentState() {
        return this.state;
    }

    /**
     * Registers a listener to be notified when the hangar state changes.
     * 
     * <p>The listener will receive an update event whenever a state change is detected from
     * the serial communication. Multiple listeners can be registered and will all receive
     * notifications.</p>
     * 
     * @param listener the event listener to register; must not be null
     */
    @Override
    public void subscribe(EventListener<StateHolder<S>> listener) {
        stateChangeListeners.add(listener);
    }

    /**
     * Unregisters a previously registered listener from receiving state change notifications.
     * 
     * <p>After unsubscription, the listener will no longer receive update events.</p>
     * 
     * @param listener the event listener to unregister
     * @return true if the listener was successfully removed, false if the listener was not registered
     */
    @Override
    public boolean unsubscribe(EventListener<StateHolder<S>> listener) {
        return stateChangeListeners.remove(listener);
    }

    /**
     * Publishes a state change event to all registered listeners.
     * 
     * <p>This method is called whenever the hangar state changes, notifying all subscribers
     * about the new state. Each listener receives an update call with the state data.</p>
     * 
     * @param data the updated state holder containing the new hangar state
     */
    @Override
    public void publishEvent(StateHolder<S> data) {
        stateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }
}
