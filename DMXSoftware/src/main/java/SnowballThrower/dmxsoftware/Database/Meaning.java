/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import java.util.List;

/**
 *
 * @author Sven
 */
class Meaning {
    List<Condition> conditions;
    List<Value> values;
    String name;
    
    String getMeaning(int value){
        String meaning;
        if(values.isEmpty()){
            return (""+value);
        }
        int i;
        for(i=0;i<values.size();i++){
            if(values.get(i).max>=value && values.get(i).min<=value){
                return values.get(i).value;
            }
        }
        return null;
    }

    boolean met(int[] values) { 
        boolean met = true;
        int i;
        for(i=0;i<conditions.size();i++){
            if(!conditions.get(i).met(values)){
                met = false;
            }
        }
        return met;
    }
}
