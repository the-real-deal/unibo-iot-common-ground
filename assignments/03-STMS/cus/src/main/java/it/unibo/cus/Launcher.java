package it.unibo.cus;

import io.vertx.core.Vertx;

public class Launcher {

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        ControlUnitService cusService = new ControlUnitService();
        System.out.println("[LAUNCHER] Starting CUS Deployment");
        
        vertx.deployVerticle(cusService, ar -> {
            if (ar.succeeded()) {
                System.out.println("[LAUNCHER] CUS successfully deployed and running.");
            } else {
                System.err.println("[LAUNCHER] CUS Deployment FAILED.");
                ar.cause().printStackTrace();
            }
        });
    }
}
