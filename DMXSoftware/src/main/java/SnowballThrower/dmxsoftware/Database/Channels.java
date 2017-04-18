/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import SnowballThrower.dmxsoftware.Processing.Manage;

import java.util.List;

/**
 *
 * @author Sven
 */
public class Channels {

    DMXChannel[] channels;
    Manage manager;

    public Channels(Manage manager) {
        channels = new DMXChannel[600];
    }

    public Channels(Manage manager, List<Device> devices) {
        channels = new DMXChannel[600];
        for (Device device : devices) {
            for (DMXChannel channel : device.getChannels()) {
                if (channel != null) {
                    channel.addManager(manager);
                    channels[channel.getAddress()] = channel;
                }
            }
        }
    }

    public DMXChannel[] getAll() {
        return channels;
    }

    public DMXChannel get(int index) {
        try {
            return channels[index];
        } catch (Exception ex) {
            return null;
        }
    }
}
