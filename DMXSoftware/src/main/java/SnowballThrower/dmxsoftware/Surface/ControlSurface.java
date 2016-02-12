/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Device;
import java.util.List;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.ScrollPane;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;

/**
 *
 * @author Sven
 */
public class ControlSurface extends Application {

    Scene bars;
    Scene faders;
    Scene pultRouting;
    Scene groups;
    Scene presets;
    Scene deviceScene;
    Scene device;
    Stage stage;
    List<Device> devices;
    double windowHeight;
    double windowWidth;

    public ControlSurface(List<Device> devices) {
        double sizeX = 70;
        double sizeY = 200;
        double distY = 20;
        this.devices = devices;
        int dev = 0;
        ScrollPane pane = new ScrollPane();
        pane.setVbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);
        FaderListener fl = new FaderListener();
        Group faderGroup = new Group();
        for (Device device : devices) {
            Group picture;
            System.out.println(device.getImagePath());
            try {
                Image image = new Image(device.getImagePath());
                ImageView imV = new ImageView(image);
                imV.setFitHeight(2*sizeX);
                imV.setFitWidth(2*sizeX);
                imV.setLayoutY(dev * (sizeY + distY));
                picture = new Group(imV);
            } catch (Exception ex){
                System.out.println(ex);
                Rectangle rect = new Rectangle(0,dev * (sizeY + distY),sizeX*2,sizeX*2);
                rect.setFill(Color.WHITE);
                picture = new Group(rect);
            }
            faderGroup.getChildren().add(picture);
            int ch = 2;
            System.out.println(device.getName());
            for (Channel channel : device.getType().getChannels()) {
                if (channel != null) {
                    Fader fader = new Fader(ch * sizeX, dev * (sizeY + distY), sizeX, sizeY, channel);
                    fl.addFader(fader);
                    faderGroup.getChildren().add(fader.getFader());
                }
                ch++;
            }
            dev++;
        }
        Group group = new Group(faderGroup);
        pane.setContent(group);
        Scene faders = new Scene(pane, 1000, 800);
        group.setOnMouseClicked(fl);
        group.setOnMousePressed(fl);
        group.setOnMouseDragged(fl);
        stage = new Stage();
        stage.setScene(faders);
        stage.show();
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        System.out.println("Start");
    }
}
