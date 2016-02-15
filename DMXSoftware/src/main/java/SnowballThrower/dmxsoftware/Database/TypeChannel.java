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
public class TypeChannel {

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

    String getName(int[] values) {
        int i;
        for (i = 0; i < meanings.size(); i++) {
            if (meanings.get(i).met(values)) {
                //System.out.println("meaning " + i);
                if (meanings.get(i).name != null && !meanings.get(i).name.equals("")) {
                    return meanings.get(i).name;
                }
            }
        }
        return name;
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

    public int getNumber() {
        return this.number;
    }

}
