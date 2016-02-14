/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Communicate;

import java.util.logging.Level;
import java.util.logging.Logger;
import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.MidiDevice;
import javax.sound.midi.MidiDevice.Info;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.ShortMessage;
import static javax.sound.midi.ShortMessage.CONTROL_CHANGE;

/**
 *
 * @author Sven
 */
public class MidiConnection extends Thread {

    static final int MAX_CH = 1000;
    MidiDevice dmxController;
    private int[] valOld;
    int[] valNew;
    boolean stop;
    private long SLEEP = 2;

    public MidiConnection() {
        stop = false;
        valOld = new int[MAX_CH];
        valNew = new int[MAX_CH];
        Info[] infos = MidiSystem.getMidiDeviceInfo();
        for (int i = 0; i < infos.length; i++) {
            System.out.println(infos[i].getName());
            if (infos[i].getName().contains("Cable")) {
                try {
                    dmxController = MidiSystem.getMidiDevice(infos[i]);
                } catch (MidiUnavailableException ex) {
                    Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
        if (dmxController != null) {
            if (!dmxController.isOpen()) {
                try {
                    dmxController.open();
                } catch (MidiUnavailableException ex) {
                    Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    public void change(int channel, int value) {
        if (value < 256 && value >= 0 && channel >= 0 && channel < MAX_CH) {
            valNew[channel] = value;
        }
    }

    private void send(int channel, int value) {
        //System.out.println("Midi send: " + channel + " ," + value / 2);
        //System.out.println("Midi send: " + (CONTROL_CHANGE + channel / 128) + " ," + channel % 128 + " ," + value / 2);
        try {
            ShortMessage message = new ShortMessage(CONTROL_CHANGE, channel / 128, channel % 128, value / 2);
            try {
                dmxController.getReceiver().send(message, -1);
            } catch (MidiUnavailableException ex) {
                System.out.println("No Midi " + channel + " ," + value);
                //stop = true;
                //stop();
            } catch (NullPointerException np) {
                System.out.println("midi=null");
            }
        } catch (InvalidMidiDataException ex) {
            Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private void loop() {
        int i;
        for (i = 0; i < MAX_CH; i++) {
            int valBuff = valNew[i];
            if (valOld[i] != valBuff) {
                valOld[i] = valBuff;
                send(i, valOld[i]);
            }
        }
        try {
            Thread.sleep(SLEEP);
        } catch (InterruptedException ex) {
            Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void run() {
        while (!stop) {
            loop();
        }
        if (dmxController != null) {
            dmxController.close();
        }
    }
}
