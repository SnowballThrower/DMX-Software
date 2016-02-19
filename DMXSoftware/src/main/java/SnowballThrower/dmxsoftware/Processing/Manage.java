/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Processing;

import SnowballThrower.dmxsoftware.Communicate.MidiConnection;
import SnowballThrower.dmxsoftware.Communicate.SerialConnection;
import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.DMXChannel;
import SnowballThrower.dmxsoftware.Surface.ControlSurface;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author Sven
 */
public class Manage {

    ControlSurface cs;
    MidiConnection mc;
    SerialConnection sc;
    Devices devs;
    int faderChannel = 0;
    private List<ControlSurface> views;

    public Manage() {
        mc = new MidiConnection(this);
        views = new LinkedList<ControlSurface>();
    }

    public void send(int channel, int value) { //channl <16 * 12
        if (channel < 1024 && channel >= 0 && value >= 0 && value < 256) {
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

    public void barExtend() {

    }

    void setDispText(String line2, String line1) {
        for (int i = 0; i < line1.length(); i++) {
            mc.sendDisp(0, i, line1.charAt(i));
        }

        for (int i = 0; i < line2.length(); i++) {
            mc.sendDisp(1, i, line2.charAt(i));
        }
    }

    public void handleMidiFader(int fader, int value) {
        try {

            devs.getChannels().get(faderChannel + fader + 1).setValue(value / 4);
        } catch (IndexOutOfBoundsException | NullPointerException ex) {
            System.out.println("nö");
        }
    }

    public void handleMidiAction(int data1, boolean b) {
        if (data1 == 17) {
            faderChannel--;
            if (faderChannel < 0) {
                faderChannel = 511;
            }
            System.out.println("turned left");
            for (ControlSurface cs : views) {
                try {
                    cs.setRemFader(faderChannel);
                } catch (Exception ex) {

                }
            }
        }
        if (data1 == 16) {
            faderChannel++;
            if (faderChannel > 512) {
                faderChannel = 0;
            }
            System.out.println("turned right");
            for (ControlSurface cs : views) {
                try {
                    cs.setRemFader(faderChannel);
                } catch (Exception ex) {

                }
            }
        }
        if (b) {
            System.out.println("Pressed Button " + data1);
        } else {
            System.out.println("Released Button " + data1);
        }
    }

    void setDevs(Devices aThis) {
        devs = aThis;
    }

    public void register(ControlSurface aThis) {
        this.views.add(aThis);
        try {
            aThis.setRemFader(faderChannel);
        } catch (Exception ex) {

        }
    }
}
