package it.unibo.iot.cus;

import static java.lang.System.out;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

public final class AppLauncher {
    private AppLauncher() {
    }

    /**
     * Launches the application.
     */
    public static void main(final String[] args) {
        out.println("Hello, world!");
        try {
            InputStream is = AppLauncher.class.getClassLoader().getResourceAsStream("config.json");
            String content = new String(is.readAllBytes(), StandardCharsets.UTF_8);
            JsonObject config = new Gson().fromJson(content, JsonObject.class);
        } catch (Exception e) {
            System.err.println("Could not load config: " + e.getMessage());
        }
    }
}
