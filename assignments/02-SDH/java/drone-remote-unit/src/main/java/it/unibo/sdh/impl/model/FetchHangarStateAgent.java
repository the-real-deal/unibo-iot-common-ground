package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.EventPublisher;
import it.unibo.sdh.api.model.StateHolder;

/**
 * A background thread agent that monitors and fetches the hangar state from a serial communication channel.
 * 
 * <p>This class continuously reads messages from a communication channel and updates the internal
 * hangar state when a valid state change is detected. It implements the observer pattern by notifying
 * all registered listeners whenever the hangar state changes.</p>
 * 
 * <p>The agent runs in a continuous loop, parsing received messages as {@link HangarStates} enum values.
 * Invalid messages are silently ignored. State changes are detected by comparing the newly received state
 * with the current state, and only valid changes trigger event notifications to subscribers.</p>
 * 
 * @author SDH Team
 * @version 1.0
 */
public class FetchHangarStateAgent extends Thread implements EventPublisher<StateHolder<HangarStates>> {

    private CommunicationChannel channel;
    private StateHolder<HangarStates> state;
    private List<EventListener<StateHolder<HangarStates>>> stateChangeListeners;
    
    /**
     * Constructs a FetchHangarStateAgent with the specified communication channel and state holder.
     * 
     * <p>Initializes the agent and immediately publishes the current state to any listeners that
     * have already been registered (though typically registration happens after instantiation).</p>
     * 
     * @param channel the communication channel to read hangar state messages from; must not be null
     * @param state the state holder that maintains the current hangar state; must not be null
     * @throws NullPointerException if either channel or state is null
     */
    public FetchHangarStateAgent(CommunicationChannel channel, StateHolder<HangarStates> state)
    {
        this.channel = channel;
        this.stateChangeListeners = new ArrayList<>();
        this.state = state;
        publishEvent(state);
    }

    /**
     * Main execution loop that continuously monitors and updates the hangar state.
     * 
     * <p>This method runs indefinitely in a separate thread, performing the following steps:</p>
     * <ol>
     *   <li>Reads messages from the communication channel</li>
     *   <li>Parses the message as a {@link HangarStates} enum value</li>
     *   <li>Compares the parsed state with the current state</li>
     *   <li>Updates the state and notifies listeners if a change is detected</li>
     * </ol>
     * 
     * <p>Thread safety is ensured through synchronization on both the communication channel
     * and the state holder to prevent race conditions during state transitions.</p>
     * 
     * <p>Invalid messages (those that cannot be parsed as valid HangarStates values) are
     * silently ignored, and the loop continues to the next iteration.</p>
     */
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
                        publishEvent(state);
                    }
                }
            }
        }
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
    public void subscribe(EventListener<StateHolder<HangarStates>> listener) {
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
    public boolean unsubscribe(EventListener<StateHolder<HangarStates>> listener) {
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
    public void publishEvent(StateHolder<HangarStates> data) {
        stateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }
}
