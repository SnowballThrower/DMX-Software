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
    Red, Green, Blue, White, Amber, UV, 
    Dimmer, Strobe, Speed, Macro, Program, Pan, Tilt, Curve, Other;

    static Function toFunction(String string) {
        try {
            return Function.valueOf(string);
        } catch (IllegalArgumentException iaex) {
            return Other;
        }
    }
}
