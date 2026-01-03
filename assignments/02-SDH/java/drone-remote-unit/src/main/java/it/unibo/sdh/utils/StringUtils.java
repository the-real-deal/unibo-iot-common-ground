package it.unibo.sdh.utils;

public class StringUtils {
    private StringUtils() { }

    public static boolean isNumeric(final String string) {
        try {
            Double.parseDouble(string);
            return true;
        } catch (final Exception e) {
            return false;
        }
    }
}
