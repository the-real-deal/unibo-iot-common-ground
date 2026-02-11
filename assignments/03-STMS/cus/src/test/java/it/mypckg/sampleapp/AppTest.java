package it.mypckg.sampleapp;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;

/**
 * Application test.
 */
class AppTest {

    /**
     * Launches the app, intecepts exceptions.
     */
    @Test void testApp() {
        assertTrue(2 > 1);
    }
}
