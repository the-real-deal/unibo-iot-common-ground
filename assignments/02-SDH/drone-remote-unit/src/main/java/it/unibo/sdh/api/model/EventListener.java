package it.unibo.sdh.api.model;

public interface EventListener<D> {
    void update(D data);
}
