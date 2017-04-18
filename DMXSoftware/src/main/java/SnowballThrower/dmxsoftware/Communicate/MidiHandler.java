/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Communicate;

import SnowballThrower.dmxsoftware.Processing.Manage;
import javax.sound.midi.MidiMessage;
import javax.sound.midi.Receiver;
import javax.sound.midi.ShortMessage;
import static javax.sound.midi.ShortMessage.CONTROL_CHANGE;
import static javax.sound.midi.ShortMessage.NOTE_OFF;
import static javax.sound.midi.ShortMessage.NOTE_ON;

/**
 *
 * @author Sven
 */
public class MidiHandler implements Receiver {

    Manage manager;

    MidiHandler(Manage manager) {
        this.manager = manager;
    }

    @Override
    public void send(MidiMessage message, long timeStamp) {
        System.out.println("got message");
        handle((ShortMessage) message);
    }

    @Override
    public void close() {
        System.out.println("closed MidiHandler");
    }

    private void handle(ShortMessage message) {
        System.out.println(message.getCommand());
        System.out.println(message.getChannel());
        System.out.println(message.getData1());
        System.out.println(message.getData2());
        if (message.getCommand() == CONTROL_CHANGE) {
            int fader = message.getChannel();
            int value = message.getData2() + message.getData1() * 32;
            System.out.println("CC " + fader + " " + value);
            manager.handleMidiFader(fader, value);
        }
        if (message.getCommand() == NOTE_ON) {
            manager.handleMidiAction(message.getData1(), true);
        }
        if (message.getCommand() == NOTE_OFF) {
            manager.handleMidiAction(message.getData1(), false);

        }
    }

}
