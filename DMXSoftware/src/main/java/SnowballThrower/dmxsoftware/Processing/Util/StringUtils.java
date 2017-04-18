package SnowballThrower.dmxsoftware.Processing.Util;

/**
 * Created by Sven on 16.04.2017.
 */
public class StringUtils {
    public static String fillRight(String original, int length, char letter) {
        String filled = "";
        for (int i = 0; i < length; i++) {
            if (i < original.length()) {
                filled += original.charAt(i);
            } else {
                filled += letter;
            }
        }
        return filled;
    }

    public static String fillRight(String original, int length) {
        return fillRight(original, length, ' ');
    }
}
