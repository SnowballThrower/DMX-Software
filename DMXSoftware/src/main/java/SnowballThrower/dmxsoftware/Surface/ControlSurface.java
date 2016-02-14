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
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.InputEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;

/**
 *
 * @author Sven
 */
public class ControlSurface extends Application {

    ScrollPane bars;
    Scene faders;
    Scene pultRouting;
    Scene groups;
    Node presets;
    ScrollPane deviceScene;
    Scene device;
    Stage stage;
    StackPane stack;
    List<Device> devices;
    double windowHeight;
    double windowWidth;
    Manage manager;

    public ControlSurface(Manage mng, List<Device> devices) {
        manager = mng;
        this.devices = devices;
        stack = new StackPane();
        stage = new Stage();
        deviceScene = allDevs();
        bars = bars();
        Button btn1 = new Button("Geräte");
        Button btn2 = new Button("Bars");
        Button btn3 = new Button("Szenen");
        Button btn4 = new Button("kompakt");
        GridPane buttons = new GridPane();
        buttons.add(btn1, 0, 0);
        buttons.add(btn2, 1, 0);
        buttons.add(btn3, 2, 0);
        buttons.add(btn4, 3, 0);
        GridPane scenePane = new GridPane();
        scenePane.add(buttons, 0, 0);
        scenePane.add(stack, 0, 1);

        btn1.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                stack.getChildren().clear();
                stack.getChildren().add(deviceScene);
            }
        });
        btn2.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                stack.getChildren().clear();
                stack.getChildren().add(bars);
            }
        });

        faders = new Scene(scenePane, 1000, 800);
        stage.setScene(faders);
        stage.setTitle("Control Surface");
        //pane.setMaxHeight(stage.getHeight());
        stage.show();
    }

    ScrollPane allDevs() {

        int dev = 0;
        double sizeX = 70;
        double sizeY = 200;
        double distY = 50;
        ScrollPane pane = new ScrollPane();
        pane.setVbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);

        Group deviceGroup = new Group();
        for (Device device : devices) {
            Group fG = faderGroup(sizeX, sizeY, device);
            fG.setLayoutY(0.5 * distY + dev * (sizeY + distY));

            dev++;
            deviceGroup.getChildren().add(fG);
        }
        /*deviceGroup.getChildren().add(new Line(
         0, dev * (sizeY + distY),
         2000, dev * (sizeY + distY)));
         deviceGroup.getChildren().add(new Line(
         0, 0,
         2000, 0)); */
        Group group = new Group(deviceGroup);
        pane.setContent(group);
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

    Group deviceImage(Device device, double sizeX, double sizeY) {
        Group picture;
        System.out.println(device.getImagePath());
        Rectangle rect = new Rectangle(0, 0, sizeX * 2, sizeY);
        rect.setFill(Color.WHITE);
        picture = new Group(rect);
        try {
            Image image = new Image(device.getImagePath());
            ImageView imV = new ImageView(image);
            imV.setPreserveRatio(true);
            imV.setFitHeight(2 * sizeX);

            picture.getChildren().add(imV);
        } catch (Exception ex) {
            System.out.println(ex);
        }
        Font font = new Font(sizeY/12);
        Text type = new Text(device.getType().getName());
        type.setLayoutY(sizeY / 8 * 6);
        type.setFont(font);
        Text name = new Text(device.getName());
        name.setLayoutY(sizeY / 8 * 7);
        name.setFont(font);
        Text place = new Text(device.getAdditon());
        place.setLayoutY(sizeY);
        place.setFont(font);
        Group texts = new Group(picture, name, place, type);
        return new Group(picture, texts);
    }

    ScrollPane bars() {
        ScrollPane pane = new ScrollPane();
        pane.setContent(new Rectangle(100, 100));
        return pane;
    }

    Group faderGroup(double sizeX, double sizeY, Device device) {

        FaderListener fl = new FaderListener(manager);
        Group faderGroup = new Group();
        faderGroup.getChildren().add(deviceImage(device, sizeX, sizeY));
        int ch = 2;
        System.out.println(device.getName());
        for (Channel channel : device.getType().getChannels()) {
            if (channel != null) {
                Fader fader = new Fader("" + (device.getStartCh() + ch - 2),
                        ch * sizeX, 0, sizeX, sizeY, channel);
                fl.addFader(fader);
                Group fade = fader.getFader();
                fade.addEventFilter(InputEvent.ANY, (event) -> {
                    if (event.getEventType().toString().contains("SCROLL")) {
                        //System.out.println(event.getEventType());

                        event.consume();
                    }
                });
                faderGroup.getChildren().add(fade);
            }
            ch++;
        }
        faderGroup.setOnMouseClicked(fl);
        faderGroup.setOnMousePressed(fl);
        faderGroup.setOnMouseDragged(fl);
        return faderGroup;
    }
}
