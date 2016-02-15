/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

/**
 *
 * @author Sven
 */
public class DeviceType {

    String id;
    String name;
    Type type;
    int power;
    String picturePath;
    int channelNumber;
    TypeChannel[] channels;

    public String getMeaning(int channel, int[] values) {
        if (channels == null) {
            return null;
        }
        if (channels[channel] == null) {
            return null;
        }
        if (values == null) {
            return "values null";
        }
        return channels[channel].getMeaning(values[channel], values);
    }

    public int getChannelNumber() {
        return channelNumber;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public Type getType() {
        return type;
    }

    public String getChannelNames() {
        String string = "";
        if (channels != null) {
            for (TypeChannel channel : channels) {
                if (channel != null) {
                    string = string + "  " + channel.name + " " + channel.function;
                }
            }
        } else {
            return "no Channels";
        }
        return string;
    }

    public String getChannelName(int x, int[] values) {
        if (channels != null) {
            if (x < channelNumber) {
                if (channels[x] != null) {
                    return channels[x].getName(values);
                }
            }
        }
        return "no Channel";
    }

    public TypeChannel[] getChannels() {
        return channels;
    }

}
