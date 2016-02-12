/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import java.util.LinkedList;
import java.util.List;
import javafx.event.EventHandler;
import javafx.scene.input.MouseEvent;

/**
 *
 * @author Sven
 */
public class FaderListener implements EventHandler<MouseEvent> {

    List<Fader> faders;

    public FaderListener() {
        faders = new LinkedList<Fader>();
    }

    public void addFader(Fader fader) {
        faders.add(fader);
    }

    public void removeFader(Fader fader) {
        faders.remove(fader);
    }

    public void mouseClicked(MouseEvent e) {
        for (Fader fader : faders) {
            if (e.getX() >= fader.getPosX() + fader.getFrameX()
                    && e.getX() <= fader.getPosX() + fader.getWidth() - fader.getFrameX()
                    && e.getY() >= fader.getPosY() + fader.getFrameY()
                    && e.getY() <= fader.getPosY() + fader.getHeight() - fader.getFrameY()) {
                fader.setValue((int) ((fader.getHeight() - fader.getFrameY()
                        - e.getY() + fader.getPosY())
                        * 255 / (fader.getHeight() - 2 * fader.getFrameY())));
            }
        }
    }

    @Override
    public void handle(MouseEvent event) {
        mouseClicked(event);
    }

}
