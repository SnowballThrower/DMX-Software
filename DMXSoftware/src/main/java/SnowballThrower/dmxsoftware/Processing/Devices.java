/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Processing;

import SnowballThrower.dmxsoftware.Database.Channels;
import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Database.DeviceType;
import SnowballThrower.dmxsoftware.Database.XMLReader;
import java.util.List;

/**
 *
 * @author Sven
 */
public class Devices {

    List<Device> devices;
    List<DeviceType> types;
    Channels channels;

    public Devices(Manage mng) {
        mng.setDevs(this);
        XMLReader xml = new XMLReader();
        types = xml.getTypes();
        devices = xml.getDevices();
        System.out.println("have read.");
        for (Device device : devices) {
            for (DeviceType type : types) {
                if (device.getTypeId().equals(type.getId())) {
                    device.setType(type);
                    System.out.println(type.getId());
                }
            }
        }
        System.out.println("have merged.");
        channels = new Channels(mng, devices);
        mng.setDispText("Ferngesteuerter", "Modus mit Java");
        System.out.println("Send Text");
        //showDevs();
    }

    private void showDevs() {
        for (Device device : devices) {
            System.out.println("Device: ");
            System.out.println(device.getName() + ": " + device.getType().getName() + ", " + device.getType().getType());
            System.out.println(device.getType().getChannelNames());
            int[] values = new int[device.getType().getChannelNumber()];
            for (int v = 0; v < 256; v++) {
                for (int x = 0; x < device.getType().getChannelNumber(); x++) {
                    values[x] = v;
                    System.out.println(device.getType().getMeaning(x, values) + device.getType().getChannelName(x, values));
                }

            }
        }
    }

    public List<Device> getDevices() {
        return this.devices;
    }

    public Channels getChannels() {
        return channels;
    }
}
