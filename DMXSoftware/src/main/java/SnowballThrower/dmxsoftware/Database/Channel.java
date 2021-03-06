/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import SnowballThrower.dmxsoftware.Surface.Fader;
import java.util.LinkedList;
import java.util.List;
import javafx.application.Platform;

/**
 *
 * @author Sven
 */
public abstract class Channel {

    int number;
    int value;
    TypeChannel typeCh;
    private final Function function;
    List<Fader> faders;
    List<Channel> channels;

    public void setValue(int value) {
        if (value >= 0 && value < 256) {
            if(this.value == value)
                return;
            this.value = value;
            for (Fader fader : faders) {
                fader.act();
            }
        }
    }

    public void act() {
        //Platform.runLater(new Runnable() {

            //@Override
            //public void run() {

                for (Fader fader : faders) {
                    try {
                        fader.act();
                        //System.out.println("act Fader");
                    } catch (Exception e) {
                        System.out.println("Fehler bei fader aktualisieren.");
                    }
                }
            //}
        //});
    }

    public int getValue() {
        return this.value;
    }

    public TypeChannel getchType() {
        return typeCh;
    }

    public Function getFunction() {
        return function;
    }

    public Channel(TypeChannel ch) {
        faders = new LinkedList<Fader>();
        value = 0;
        typeCh = ch;
        function = ch.getFunction();
        number = 0;
    }

    public int getNumber() {
        return this.number;
    }

    public void register(Fader aThis) {
        faders.add(aThis);
    }

    public void setHighlighted(boolean high) {
        for (Fader fader : faders) {
            fader.highlight(high);
        }
    }
}
