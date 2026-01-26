package it.unibo.cus;

import io.vertx.core.json.JsonObject;

public class DataState {
    public double waterLevel = 0;
    public int valveValue = 0;
    public String mode = "AUTOMATIC";
    public String status = "UNCONNECTED"; 
    public long lastSeenTMS = 0;
    public JsonObject toJson() {
        return new JsonObject()
            .put("level", waterLevel)
            .put("valve", valveValue)
            .put("mode", mode)
            .put("status", status);
    }
}