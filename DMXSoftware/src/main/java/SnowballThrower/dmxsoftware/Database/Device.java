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
public class Device {

    int startChannel;
    String name;
    String addition;
    String id;
    String typeId;
    DeviceType type;
    DMXChannel[] channels;

    int getValue(int channel) {
        return channels[channel].getValue();
    }

    public String getMeaning(int channel) {
        if (channel < 0) {
            return "";
        }
        int[] values = new int[type.channelNumber];
        for (DMXChannel ch : channels) {
            if (ch != null) {
                values[ch.getNumber() - 1] = ch.getValue();
            }
        }
        return type.getMeaning(channel, values);
    }

    public String getChannelName(int channel) {
        if (channel < 0) {
            return "";
        }
        if (type == null) {
            return "no type";
        }
        if (type.channels == null) {
            return "no channels";
        }
        if (type.channels[channel] != null) {
            int[] values = new int[type.channelNumber];
            for (DMXChannel ch : channels) {
                if (ch != null) {
                    values[ch.getNumber() - 1] = ch.getValue();
                }
            }
            return channels[channel].typeCh.getName(values);
        }
        return null;
    }

    public String getTypeId() {
        return typeId;
    }

    public void setType(DeviceType type) {
        this.type = type;
        channels = new DMXChannel[type.getChannelNumber()];
        for (TypeChannel tc : type.getChannels()) {
            if (tc != null) {
                channels[tc.getNumber() - 1] = new DMXChannel(tc, this);
            }
        }
    }

    public String getName() {
        return this.name;
    }

    public DeviceType getType() {
        return this.type;
    }

    public String getImagePath() {
        return type.picturePath;
    }

    public int getStartCh() {
        return this.startChannel;
    }

    public String getAdditon() {
        return this.addition;
    }

    public DMXChannel[] getChannels() {
        return channels;
    }

    void act() {
        for(DMXChannel channel : channels){
            if(channel!= null){
                channel.act();
            }
        }
    }
}
