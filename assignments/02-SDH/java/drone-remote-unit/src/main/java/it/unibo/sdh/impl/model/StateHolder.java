package it.unibo.sdh.impl.model;

public class StateHolder<S> {
    private S state;
    
    public StateHolder(S initialState) {
        this.state = initialState;
    }

    public S getCurrentState() {
        return state;
    }

    public void setState(final S newState) {
        this.state = newState;
    }
}
