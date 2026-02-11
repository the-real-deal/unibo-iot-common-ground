package it.unibo.iot.cus.agents;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import it.unibo.iot.cus.model.Context;
import it.unibo.iot.cus.model.InputMode;
import it.unibo.iot.cus.model.WaterLevelSampleData;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

public class HttpAgent extends AbstractVerticle {
    private int port;
    private Context sharedData;
    private String senderID;
    final static Logger logger = LoggerFactory.getLogger(HttpAgent.class);

    public HttpAgent(final int port, final Context sharedData) {
        this.port = port;
        this.sharedData = sharedData;
        this.senderID = HttpAgent.class.getName();
		logger.atInfo().log("new agent created.");
    }

    @Override
    public void start() throws Exception {
        final var msgChannel = vertx.eventBus();
        // Event bus registrations
        msgChannel.consumer("tank.waterlevel", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1];
            this.sharedData.setLastWaterLevelSample(new WaterLevelSampleData(Double.valueOf(value)));
            logger.atInfo().log("tank.waterlevel updated internal state with: ".concat(content));
        });
        msgChannel.consumer("system.inputmode", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1];
            this.sharedData.setInputMode(InputMode.valueOf(value));
            logger.atInfo().log("system.inputmode updated internal state with: ".concat(value));
        });
        msgChannel.consumer("tank.valveopening", msg -> {
            final var content = String.valueOf(msg.body());
            final var sender = content.split(":")[0];
            // avoid echo
            if (sender.equals(this.senderID)) {
                return;
            }
            final var value = content.split(":")[1];
            this.sharedData.setValveOpeningPercentage(Double.valueOf(value));
            logger.atInfo().log("tank.valveopening updated internal state with: ".concat(value));
        });

        // HTTP Endpoints setup
        final var server = vertx.createHttpServer();
        final var router = Router.router(vertx);
        router.route().handler(BodyHandler.create());
        // Enable CORS with specific settings
        router.route().handler(CorsHandler.create("*")
            .allowedMethod(HttpMethod.GET)
            .allowedMethod(HttpMethod.POST)
            .allowedHeader("Content-Type")
            // .allowedHeader("Authorization") // TODO: uncomment if needed
        );
        router.get("/api/status").handler(ctx -> {
            HttpServerResponse response = ctx.response();
            response.putHeader("content-type", "application/json");
            final var res = new JsonObject();
            res.addProperty(
                    "status",
                    this.sharedData.getInputMode().name());
            final var output = res.toString();
            response.end(output);
            logger.atInfo().log("GET /api/status -> ".concat(output));
        });

        router.get("/api/water-level").handler(ctx -> {
            HttpServerResponse response = ctx.response();
            response.putHeader("content-type", "application/json");
            final var res = new JsonObject();
            res.addProperty(
                    "level",
                    this.sharedData.getLastWaterLevelSample().level());
            res.addProperty(
                    "timestampMS",
                    this.sharedData.getLastWaterLevelSample().timestampMs());
            final var output = res.toString();
            response.end(output);
            logger.atInfo().log("GET /api/water-level -> ".concat(output));
        });

        router.get("/api/valve-opening").handler(ctx -> {
            HttpServerResponse response = ctx.response();
            response.putHeader("content-type", "application/json");
            final var res = new JsonObject();
            res.addProperty("opening", this.sharedData.getValveOpeningPercentage());
            final var output = res.toString();
            response.end(output);
            logger.atInfo().log("GET /api/valve-opening -> ".concat(output));
        });

        router.post("/api/valve-opening").handler(ctx -> {
            HttpServerResponse response = ctx.response();
            // Assuming the JSON sent is: {"opening": 0.75}
            JsonObject body = (JsonObject)JsonParser.parseString(ctx.getBody().toString());
            if (body.has("opening")) {
                double newOpening = body.get("opening").getAsDouble();
                this.sharedData.setValveOpeningPercentage(newOpening);
                final var res = new JsonObject();
                res.addProperty("status", "updated");
                msgChannel.publish("tank.valveopening", this.senderID.concat(":" + String.valueOf(newOpening)));
                response.setStatusCode(200)
                        .putHeader("content-type", "application/json")
                        .end(res.toString());
                logger.atInfo().log("POST /api/valve-opening -> Updated to " + newOpening);
            } else {
                response.setStatusCode(400).end("Missing 'opening' property");
            }
        });

        router.post("/api/status").handler(ctx -> {
            HttpServerResponse response = ctx.response();
            // Assuming the JSON sent is: {"status": AUTOMATIC|MANUAL|DISCONNECTED}
            JsonObject body = (JsonObject)JsonParser.parseString(ctx.getBody().toString());
            if (body.has("status")) {
                InputMode newMode = InputMode.valueOf(body.get("status").getAsString());
                this.sharedData.setInputMode(newMode);
                final var res = new JsonObject();
                res.addProperty("status", "updated");
                msgChannel.publish("system.inputmode", this.senderID.concat(":" + newMode.name()));
                response.setStatusCode(200)
                        .putHeader("content-type", "application/json")
                        .end(res.toString());
                logger.atInfo().log("POST /api/valve-opening -> Updated to " + newMode);
            } else {
                response.setStatusCode(400).end("Missing 'status' property");
            }
        });

        server.requestHandler(router).listen(this.port);
    }
}
