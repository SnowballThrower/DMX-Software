/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Processing.Manage;
import java.util.List;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.ScrollPane;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.InputEvent;
import javafx.scene.layout.StackPane;
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
    Manage manager;

    public ControlSurface(Manage mng, List<Device> devices) {
        manager = mng;
        this.devices = devices;
        stage = new Stage();
        ScrollPane pane = allDevs();
        pane.setLayoutY(50);
        Group buttons = new Group();
        StackPane devSceneGroup = new StackPane(pane, buttons);
        faders = new Scene(devSceneGroup, 1000, 800);
        stage.setScene(faders);
        //pane.setMaxHeight(stage.getHeight());
        stage.show();
    }
    ScrollPane allDevs(){
        
        int dev = 0;
        double sizeX = 70;
        double sizeY = 200;
        double distY = 20;
        ScrollPane pane = new ScrollPane();
        pane.setVbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);
        pane.addEventFilter(InputEvent.ANY, (event) -> {
            if (event.getEventType().toString().contains("SCROLL")) {
                //System.out.println(event.getEventType());
                event.consume();
            }
        });
        FaderListener fl = new FaderListener(manager);
        Group faderGroup = new Group();
        for (Device device : devices) {
            Group picture;
            System.out.println(device.getImagePath());
            try {
                Image image = new Image(device.getImagePath());
                ImageView imV = new ImageView(image);
                imV.setFitHeight(2 * sizeX);
                imV.setFitWidth(2 * sizeX);
                imV.setLayoutY(dev * (sizeY + distY));
                picture = new Group(imV);
            } catch (Exception ex) {
                System.out.println(ex);
                Rectangle rect = new Rectangle(0, dev * (sizeY + distY), sizeX * 2, sizeX * 2);
                rect.setFill(Color.WHITE);
                picture = new Group(rect);
            }
            faderGroup.getChildren().add(picture);
            int ch = 2;
            System.out.println(device.getName());
            for (Channel channel : device.getType().getChannels()) {
                if (channel != null) {
                    Fader fader = new Fader("" + (device.getStartCh() + ch - 2),
                            ch * sizeX, dev * (sizeY + distY), sizeX, sizeY, channel);
                    fl.addFader(fader);
                    faderGroup.getChildren().add(fader.getFader());
                }
                ch++;
            }
            dev++;
        }
        Group group = new Group(faderGroup);
        pane.setContent(group);
        group.setOnMouseClicked(fl);
        group.setOnMousePressed(fl);
        group.setOnMouseDragged(fl);
        return pane;
    }
    @Override
    public void start(Stage primaryStage) throws Exception {
        System.out.println("Start");
    }

    public void setManager(Manage mng) {
        manager = mng;
    }

    public void send(int channel, int value) {

    }
}
