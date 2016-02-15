/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Processing;

import SnowballThrower.dmxsoftware.Communicate.MidiConnection;
import SnowballThrower.dmxsoftware.Communicate.SerialConnection;
import SnowballThrower.dmxsoftware.Surface.ControlSurface;

/**
 *
 * @author Sven
 */
public class Manage {

    ControlSurface cs;
    MidiConnection mc;
    SerialConnection sc;

    public Manage() {
        mc = new MidiConnection();
    }

    public void send(int channel, int value) {
        if (channel < 16 * 12 && channel >= 0 && value >= 0 && value < 256) {
            mc.change(channel, value);
        }
    }

    public void handle(String id, int value) {
        try {
            int ch = Integer.parseInt(id);
            if (ch < 1000) {
                //System.out.println("Change: " + id + " ," + value);
                send(ch - 1, value);
            }
        } catch (Exception ex) {
            System.out.println("no id: " + id);
        }
    }

    public void handle(int channel, int value) {
        send(channel - 1, value);
    }

    public void startMidi() {
        try {
            mc.start();
        } catch (Exception ex) {
            System.out.println("Error in startMidi");
        }
    }
}
