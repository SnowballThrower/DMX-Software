/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Communicate;

/**
 *
 * @author Sven
 */
public class SerialConnection {

    final static int BYTE_MAX = 255;
    final static int MAX_CH = 1023;

    void sendValue(int channel, int value) {
        if (channel >= 0 && channel < MAX_CH && value >= 0 && value <= BYTE_MAX) {

        }
    }

    void sendConfig() {

    }
}
