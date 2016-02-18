/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;

/**
 *
 * @author Sven
 */
public class toggleHandler implements EventHandler<ActionEvent> {

    int i;
    int func;
    ControlSurface cs;
    toggleHandler(int i, int i0) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    toggleHandler(int i, int i0, ControlSurface aThis) {
        this.cs = aThis;
        this.i = i;
        this.func = i0;
    }

    @Override
    public void handle(ActionEvent t) {
        cs.toggle(func, i);
    }
    
}
