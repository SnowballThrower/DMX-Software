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
class Channel {
    String name;
    int number;
    Function funcion;
    List<Meaning> meanings;
    String getMeaning(int value, int[] values){
        int i;
        for(i=0;i<meanings.size();i++){
            if(meanings.get(i).met(values)){
                return meanings.get(i).getMeaning(value);
            }
        }
        return value+"";
    }

}
