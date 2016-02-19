/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Processing.Manage;
import javafx.event.EventHandler;
import javafx.scene.input.MouseEvent;

/**
 *
 * @author Sven
 */
class DevClickListener implements EventHandler<MouseEvent> {

    Manage manager;
    int startCh;

    public DevClickListener(Manage manager, int startCh) {
        this.manager = manager;
        this.startCh = startCh;
    }

    @Override
    public void handle(MouseEvent t) {
        manager.setTargetChannel(startCh);
    }

}
