package it.unibo.iot.cus.model;

public class Context {
    private WaterLevelSampleData lastWaterLevelSample; 
    private InputMode inputMode; 
    private double valveOpeningPercentage;
    
    public Context(
        final WaterLevelSampleData lastWaterLevelSample, 
        final InputMode inputMode, 
        final double valveOpeningPercentage) {
        this.lastWaterLevelSample = lastWaterLevelSample;
        this.inputMode = inputMode;
        this.valveOpeningPercentage = valveOpeningPercentage;
    }

    public WaterLevelSampleData getLastWaterLevelSample() {
        return lastWaterLevelSample.getCopy();
    }

    public void setLastWaterLevelSample(final WaterLevelSampleData lastWaterLevelSample) {
        this.lastWaterLevelSample = lastWaterLevelSample.getCopy();
    }

    public InputMode getInputMode() {
        return inputMode;
    }

    public void setInputMode(final InputMode inputMode) {
        this.inputMode = inputMode;
    }

    public double getValveOpeningPercentage() {
        return valveOpeningPercentage;
    }

    public void setValveOpeningPercentage(final double valveOpeningPercentage) {
        this.valveOpeningPercentage = valveOpeningPercentage;
    }

    public boolean isSystemInAutomaticMode() {
        return this.inputMode.equals(InputMode.AUTOMATIC);
    }
    
    public Context getCopy() {
        return new Context(lastWaterLevelSample, inputMode, valveOpeningPercentage);
    }
}
