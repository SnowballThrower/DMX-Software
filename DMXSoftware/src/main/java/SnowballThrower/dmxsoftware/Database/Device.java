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
    int[] values;

    int getValue(int channel) {
        return values[channel];
    }

    String getMeaning(int channel) {
        return type.getMeaning(channel, values);
    }

    String getChannelName(int channel) {
        return type.channels[channel].name;
    }

    public String getTypeId() {
        return typeId;
    }

    public void setType(DeviceType type) {
        this.type = type;
    }

    public String getName() {
        return this.name;
    }

    public DeviceType getType() {
        return this.type;
    }
}
