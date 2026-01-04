package it.unibo.sdh.impl.model.drone;

import java.util.Optional;

import it.unibo.sdh.api.model.CommunicationChannel;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.model.MessageDecoderImpl;
import it.unibo.sdh.impl.model.MonitoringStateAgent;
import it.unibo.sdh.impl.model.StateHolderImpl;
import it.unibo.sdh.utils.CommunicationChannelUtils;
import it.unibo.sdh.utils.Pair;
import it.unibo.sdh.utils.StringUtils;

public class FetchDroneStateAgent extends MonitoringStateAgent<Pair<DroneStates, Optional<String>>> {

    public FetchDroneStateAgent(final CommunicationChannel channel, final StateHolder<Pair<DroneStates, Optional<String>>> state) {
        super(channel, state);
    }

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
                final var keepMessage = decoder.getTopic().get().startsWith(CommunicationChannelUtils.DRONE_PREFIX);
                if (!keepMessage) {
                    // if the message is not about the drone, then move on to the next iteration
                    continue;
                }
                DroneStates decoded;
                try {
                    decoded = DroneStates.valueOf(decoder.getContent().get());
                } catch (final Exception e) {
                    continue;
                }
                final var value = decoder.getValue().get();
                final var  stateHolder = super.getCurrentState(); 
                synchronized (stateHolder) {
                    Pair<DroneStates, Optional<String>> state = stateHolder.getState().get();
                    if (decoded.equals(DroneStates.LANDING) && value == "OK") {
                        state = new Pair<>(DroneStates.REST, Optional.empty());
                    } else if (decoded.equals(DroneStates.LANDING) && StringUtils.isNumeric(value)) {
                        state = new Pair<>(DroneStates.OPERATING, Optional.of(value));
                    } else if (decoded.equals(DroneStates.TAKING_OFF) && value == "OK") {                    
                        state = new Pair<>(DroneStates.OPERATING, Optional.empty());
                    }
                    var newStateHolder = new StateHolderImpl<>(state);
                    this.publishEvent(newStateHolder);
                }
                return;
            }
        }
    }
}
