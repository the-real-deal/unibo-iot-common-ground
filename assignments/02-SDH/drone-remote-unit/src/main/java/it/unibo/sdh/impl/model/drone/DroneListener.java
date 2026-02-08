package it.unibo.sdh.impl.model.drone;

import java.util.Objects;
import java.util.Optional;

import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.controller.DashboardController;
import it.unibo.sdh.utils.Pair;
import it.unibo.sdh.utils.StringUtils;

public class DroneListener implements EventListener<StateHolder<Pair<DroneStates, Optional<String>>>> {

    private DashboardController controller;

    public DroneListener(final DashboardController controller) {
        this.controller = Objects.requireNonNull(controller);
    }

    @Override
    public void update(final StateHolder<Pair<DroneStates, Optional<String>>> data) {
        if (data.getState().isEmpty()) {
            return;
        }
        final var newState = data.getState().get();
        controller.displayDroneState(newState.first().name());
        newState.second().ifPresent(value -> {
            if (StringUtils.isNumeric(value)) {
                controller.displayDroneDistance(Double.valueOf(value));
            }
        });
    }

}
