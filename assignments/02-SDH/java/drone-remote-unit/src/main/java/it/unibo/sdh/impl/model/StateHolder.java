package it.unibo.sdh.impl.model;

import java.util.Optional;

public class StateHolder<S> {
    private Optional<S> state;
    
    public StateHolder() {
        setState(Optional.empty());
    }

    public StateHolder(S initialState) {
        setState(Optional.of(initialState));
    }

    public Optional<S> getCurrentState() {
        return state;
    }

    public void setState(final Optional<S> newState) {
        this.state = newState;
    }

    public void setState(final S newState) {
        setState(Optional.of(newState));
    }
}
