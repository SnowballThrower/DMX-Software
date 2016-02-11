/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Processing;

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

    public Devices() {
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
        System.out.println("have connected.");
        showDevs();
    }

    private void showDevs() {
        for (Device device : devices) {
            System.out.println("Device: ");
            System.out.println(device.getName() + ": " + device.getType().getName() + ", " + device.getType().getType());
            System.out.println(device.getType().getChannelNames());
            int[] values = new int[device.getType().getChannelNumber()];
            for (int value : values) {
                value = 0;
            }
            for (int v = 0; v < 256; v++) {
                for (int x = 0; x<device.getType().getChannelNumber();x++) {
                    values[x]=v;
                    System.out.println(device.getType().getMeaning(x, values)+device.getType().getChannelName(x));
                }
                
            }
        }
    }
}
