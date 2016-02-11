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
public enum Type {

    Bar, Par, Wash, Beam, Gobo, Scanner, Other;

    static Type getType(String string) {
        try {
            return Type.valueOf(string);
        } catch (IllegalArgumentException iaex) {
            return Other;
        }
    }
}
