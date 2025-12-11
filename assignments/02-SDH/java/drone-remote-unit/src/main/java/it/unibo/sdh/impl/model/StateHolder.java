package it.unibo.sdh.impl.model;

import java.util.Optional;

public class StateHolder<S> {
    private Optional<S> state;
    
    public StateHolder() {}

    public StateHolder(S initialState) {
        setState(initialState);
    }

    public Optional<S> getCurrentState() {
        return state;
    }

    public void setState(final S newState) {
        this.state = Optional.of(newState);
    }
}
