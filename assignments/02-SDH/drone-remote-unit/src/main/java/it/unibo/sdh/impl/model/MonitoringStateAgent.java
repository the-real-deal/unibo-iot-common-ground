package it.unibo.sdh.impl.model;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.model.drone.DroneStates;
import it.unibo.sdh.impl.model.hangar.HangarStates;
import it.unibo.sdh.utils.CommunicationChannelUtils;
import it.unibo.sdh.utils.Pair;
import it.unibo.sdh.utils.StringUtils;

public class MonitoringStateAgent implements EventListener<String> {

    private List<EventListener<StateHolder<Pair<DroneStates, Optional<String>>>>> droneStateChangeListeners;
    private List<EventListener<StateHolder<HangarStates>>> hangarStateChangeListeners;
    private final static Logger logger = LoggerFactory.getLogger(MonitoringStateAgent.class);

    public MonitoringStateAgent() {
        this.droneStateChangeListeners = new ArrayList<>();
        this.hangarStateChangeListeners = new ArrayList<>();
    }

    public void subscribeDroneListener(EventListener<StateHolder<Pair<DroneStates, Optional<String>>>> listener) {
        droneStateChangeListeners.add(listener);
    }

    public void subscribeHangarListener(EventListener<StateHolder<HangarStates>> listener) {
        hangarStateChangeListeners.add(listener);
    }

    public void publishDroneEvent(StateHolder<Pair<DroneStates, Optional<String>>> data) {
        droneStateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }

    public void publishHangarEvent(StateHolder<HangarStates> data) {
        hangarStateChangeListeners.forEach(listener -> {
            listener.update(data);
        });
    }

    @Override
    public void update(final String data) {
        final var decoder = new MessageDecoderImpl(data);
        logger.atInfo().log("Received: ".concat(data));
        final var topic = decoder.getTopic().get();
        // if the message is about the hangar
        if (topic.equals(CommunicationChannelUtils.HANGAR_PREFIX)) {
            HangarStates fetchedState;
            try {
                fetchedState = HangarStates.valueOf(decoder.getContent().get());
            } catch (IllegalArgumentException ex) {
                // read message not relative to hangar state, so ignore it and keep going
                return;
            }
            logger.atInfo().log("Updating SDH State...");
            publishHangarEvent(new StateHolderImpl<HangarStates>(fetchedState));
        } else if (topic.equals(CommunicationChannelUtils.DRONE_PREFIX)) {
            DroneStates decoded;
            try {
                decoded = DroneStates.valueOf(decoder.getContent().get());
            } catch (final Exception e) {
                return;
            }
            final var value = decoder.getValue().get().trim();
            var newStateHolder = new StateHolderImpl<Pair<DroneStates, Optional<String>>>();
            if (decoded.equals(DroneStates.LANDING) && value.equals("OK")) {
                newStateHolder.setState(new Pair<>(DroneStates.REST, Optional.empty()));
            } else if (decoded.equals(DroneStates.LANDING) && StringUtils.isNumeric(value)) {
                newStateHolder.setState(new Pair<>(DroneStates.LANDING, Optional.of(value)));
            } else if (decoded.equals(DroneStates.TAKING_OFF) && value.equals("OK")) {                    
                newStateHolder.setState(new Pair<>(DroneStates.OPERATING, Optional.empty()));
            }
            logger.atInfo().log("Updating DRU State...");
            this.publishDroneEvent(newStateHolder);
        }
    }
}
