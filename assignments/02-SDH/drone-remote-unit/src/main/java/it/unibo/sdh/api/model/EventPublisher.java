package it.unibo.sdh.api.model;

public interface EventPublisher<D> {
    
    void subscribe(EventListener<D> listener);

    boolean unsubscribe(EventListener<D> listener);

    /**
     * Notifies all subscribed listeners with the provided event data.
     *
     * @param data the event data to be sent to all listeners
     */
    void publishEvent(D data);
}
