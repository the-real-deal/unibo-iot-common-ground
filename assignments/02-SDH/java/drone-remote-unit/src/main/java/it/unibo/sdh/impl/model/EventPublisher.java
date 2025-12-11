package it.unibo.sdh.impl.model;

public interface EventPublisher<D> {
    
    void subscribe(EventListener<D> listener);

    boolean unsubscribe(EventListener<D> listener);

    void notifyAll(D data);
}
