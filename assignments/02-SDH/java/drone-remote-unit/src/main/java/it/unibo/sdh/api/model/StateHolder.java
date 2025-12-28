package it.unibo.sdh.api.model;

import java.util.Optional;

public interface StateHolder<S> {

    Optional<S> getCurrentState();

    void setState(S newState);

}