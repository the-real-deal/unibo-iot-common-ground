package it.unibo.sdh.impl.model.hangar;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.model.MessageDecoderImpl;
import it.unibo.sdh.impl.model.MonitoringStateAgent;
import it.unibo.sdh.utils.CommunicationChannelUtils;

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
 */
public class FetchHangarStateAgent extends MonitoringStateAgent<HangarStates> {
    
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
    public FetchHangarStateAgent(CommunicationChannel channel, StateHolder<HangarStates> state) {
        super(channel, state);
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
            final var channel = super.getChannel();
            synchronized (channel) {
                final var serialMessage = channel.receiveMessage();
                if (serialMessage.isEmpty()) {
                    // no message read, so move on to the next iteration
                    continue;
                }
                final var msg = serialMessage.get();
                final var decoder = new MessageDecoderImpl(msg);
                if (decoder.getTopic().get() != CommunicationChannelUtils.HANGAR_PREFIX) {
                    continue;
                }
                HangarStates fetchedState;
                try {
                    fetchedState = HangarStates.valueOf(decoder.getContent().get());
                } catch (IllegalArgumentException ex) {
                    // read message not relative to hangar state, so ignore it and keep going
                    continue;
                }
                final var state = super.getCurrentState();
                synchronized (state) {
                    if (state.getState().isEmpty() || fetchedState != state.getState().get()) {
                        state.setState(fetchedState);
                        publishEvent(state);
                    }
                }
            }
        }
    }
}
