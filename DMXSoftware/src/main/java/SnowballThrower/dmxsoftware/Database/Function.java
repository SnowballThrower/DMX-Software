/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

/**
 *
 * @author Sven
 */
public enum Function {
    //Colors
    Red, Green, Blue, White, Amber, UV, Hue, Sat, Vol, 
    //General Functions
    Dimmer, Strobe, Speed, Macro, Program, 
    //Moving Heads
    Gobo, Rotation, Zoom, Prism, Shutter, ColorWheel, 
    Pan, Tilt, PanSpeed, TiltSpeed, PanLow, PanHigh, TiltLow, TiltHigh,
    //Specific
    Laser, DimCurve, TurnOff, Other;

    static Function toFunction(String string) {
        try {
            return Function.valueOf(string);
        } catch (IllegalArgumentException iaex) {
            return Other;
        }
    }
}
