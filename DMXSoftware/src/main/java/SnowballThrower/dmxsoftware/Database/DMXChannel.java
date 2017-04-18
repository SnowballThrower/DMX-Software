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
public class DMXChannel extends Channel {
    
    Device device;
    List<ControlChannel> controls;
    int address;
    private Manage manager;
    
    public DMXChannel(TypeChannel ch) {
        super(ch);
        this.number = ch.getNumber();
    }
    
    public DMXChannel(TypeChannel ch, Device dev) {
        super(ch);
        this.number = ch.getNumber();
        this.device = dev;
        this.address = device.getStartCh() + number - 1;
    }
    
    @Override
    public void setValue(int value) {
        super.setValue(value);
        if (device != null) {
            device.act();
            
        }
        manager.handle(address, value);
    }
    
    public int getAddress() {
        return address;
    }
    
    public Device getDevice() {
        return device;
    }
    
    void addManager(Manage manager) {
        this.manager = manager;
    }
    
}
