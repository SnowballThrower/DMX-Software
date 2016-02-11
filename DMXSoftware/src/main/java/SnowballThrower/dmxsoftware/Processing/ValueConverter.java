/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Processing;

import SnowballThrower.dmxsoftware.Database.Function;
import java.awt.Color;

/**
 *
 * @author Sven
 */
public class ValueConverter {

    int getColorFor(int hue, int sat, int vol, Function func) {
        float h = hue / 255;
        float s = sat / 255;
        float v = vol / 255;
        switch (func) {
            case Red:
                return Color.getHSBColor(h, s, v).getRed();
            case Green:
                return Color.getHSBColor(h, s, v).getGreen();
            case Blue:
                return Color.getHSBColor(h, s, v).getBlue();
            case Hue:
                return hue;
            case Sat:
                return sat;
            case Vol:
                return vol;
            default:
                return -1;
        }
    }
}
