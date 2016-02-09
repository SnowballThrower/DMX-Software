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
    Channel[] channels;

    public String getMeaning(int channel, int[] values) {
        if (channels[channel] == null) {
            return null;
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
            for (Channel channel : channels) {
                if (channel != null) {
                    string = string + "  " + channel.name + " " + channel.funcion;
                }
            }
        } else {
            return "no Channels";
        }
        return string;
    }

    public String getChannelName(int x) {
        if (channels != null) {
            if (x < channelNumber) {
                if (channels[x] != null) {
                    return channels[x].name;
                }
            }
        }
        return "no Channel";
    }

}
