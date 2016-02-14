/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import java.util.List;
import javafx.scene.text.Text;

/**
 *
 * @author Sven
 */
public class Channel {

    String name;
    int number;
    Function function;
    List<Meaning> meanings;

    String getMeaning(int value, int[] values) {
        int i;
        for (i = 0; i < meanings.size(); i++) {
            if (meanings.get(i).met(values)) {
                return meanings.get(i).getMeaning(value);
            }
        }
        return value + "";
    }

    public String getStandardMeaning(int value) {
        if (meanings != null) {
            if (meanings.get(0) != null) {
                return meanings.get(0).getMeaning(value);
            }
        }
        return null;
    }

    public Function getFunction() {
        return function;
    }

}
