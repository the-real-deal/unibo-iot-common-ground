package it.unibo.iot.cus.model;

public record WaterLevelSampleData(double level, long timestampMs) {
    public WaterLevelSampleData(final double level) {
        this(level, System.currentTimeMillis());
    }

    public WaterLevelSampleData getCopy() {
        return new WaterLevelSampleData(level, timestampMs);
    }
}
