package it.unibo.sdh.impl.model;

public class DroneContext {

    public enum States {
        REST,
        TAKING_OFF,
        OPERATING,
        LANDING
    }

    private States state;
    
    public DroneContext() {
        this.state = States.REST;
    }

    public States getCurrentState() {
        return state;
    }

    public void setState(final States newState) {
        this.state = newState;
    }
    
}
