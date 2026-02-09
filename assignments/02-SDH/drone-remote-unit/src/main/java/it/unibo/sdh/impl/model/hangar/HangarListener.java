package it.unibo.sdh.impl.model.hangar;

import java.util.Objects;

import it.unibo.sdh.api.model.EventListener;
import it.unibo.sdh.api.model.StateHolder;
import it.unibo.sdh.impl.controller.DashboardController;

public class HangarListener implements EventListener<StateHolder<HangarStates>> {

    private DashboardController controller;

    public HangarListener(final DashboardController controller) {
        this.controller = Objects.requireNonNull(controller);
    }

    @Override
    public void update(final StateHolder<HangarStates> data) {
        if (data.getState().isEmpty()) {
            return;
        }
        final var newState = data.getState().get();
        controller.displayHangarState(newState.name());
        switch (newState) {
            case NORMAL:
                controller.enableActionButtons();
                controller.clearHangarInAlarmNotification();
                break;
            case ALARM:
                controller.disableActionButtons();
                controller.notifyHangarInAlarm();
                break;
            default:
                break;
        }
    }
}
