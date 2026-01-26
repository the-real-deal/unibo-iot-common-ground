package it.unibo.sdh.impl.model;

import java.util.Optional;

import it.unibo.sdh.api.model.StateHolder;

public class StateHolderImpl<S> implements StateHolder<S> {
    
    private Optional<S> state;
    
    public StateHolderImpl() {
        setState(Optional.empty());
    }

    public StateHolderImpl(S initialState) {
        setState(Optional.of(initialState));
    }

    @Override
    public Optional<S> getState() {
        return state;
    }
 
    @Override
    public void setState(final S newState) {
        setState(Optional.of(newState));
    }

    private void setState(final Optional<S> newState) {
        this.state = newState;
    }
}
