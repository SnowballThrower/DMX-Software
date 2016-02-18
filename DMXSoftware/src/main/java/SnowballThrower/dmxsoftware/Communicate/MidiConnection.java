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
import static javax.sound.midi.ShortMessage.PITCH_BEND;
import static javax.sound.midi.ShortMessage.PROGRAM_CHANGE;
import javax.sound.midi.Transmitter;

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

    /**
     *
     * @param line (line (0-1).
     * @param col Column(0-15).
     * @param ascii 8bit.
     */
    public void sendDisp(int line, int col, char ascii) {
        if (line >= 0 && line < 2 && col >= 0 && col < 16) {
            int ch = line * 32 + col * 2 + ascii % 2;
            int asc = ((int)ascii) / 2;
            try {
                ShortMessage message = new ShortMessage(PITCH_BEND, 1, ch, asc);
                try {
                    dmxController.getReceiver().send(message, -1);
                    System.out.println(ascii+""+(int)ascii);
                    System.out.println(message.getCommand() + "," + message.getChannel() + ": " + message.getData1() + "-" + message.getData2());
                    System.out.println(PROGRAM_CHANGE + " " + ch + " " + (int) asc);
                } catch (MidiUnavailableException ex) {
                    //stop = true;
                    //stop();
                } catch (NullPointerException np) {
                    System.out.println("midi=null");
                }
            } catch (InvalidMidiDataException ex) {
                Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     *
     * @param sel selection LED or other.
     * @param led which (0-7).
     * @param value (0-255).
     */
    public void sendLED(int sel, int led, int value) {
        if (sel >= 0 && sel < 2 && led >= 0 && led < 8 && value >= 0 && value < 256) {
            int ch = 64 + sel * 32 + led * 2 + value % 2;
            int val = value / 2;
            try {
                ShortMessage message = new ShortMessage(CONTROL_CHANGE, ch, val);
                try {
                    dmxController.getReceiver().send(message, -1);
                } catch (MidiUnavailableException ex) {
                    //stop = true;
                    //stop();
                } catch (NullPointerException np) {
                    System.out.println("midi=null");
                }
            } catch (InvalidMidiDataException ex) {
                Logger.getLogger(MidiConnection.class.getName()).log(Level.SEVERE, null, ex);
            }

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
            Logger.getLogger(MidiConnection.class
                    .getName()).log(Level.SEVERE, null, ex);
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
            receive();
        }
        try {
            Thread.sleep(SLEEP);

        } catch (InterruptedException ex) {
            Logger.getLogger(MidiConnection.class
                    .getName()).log(Level.SEVERE, null, ex);
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

    void receive() {

    }
}
