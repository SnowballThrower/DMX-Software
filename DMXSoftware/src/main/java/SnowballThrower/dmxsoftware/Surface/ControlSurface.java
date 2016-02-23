/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Channels;
import SnowballThrower.dmxsoftware.Database.DMXChannel;
import SnowballThrower.dmxsoftware.Database.TypeChannel;
import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Database.Function;
import SnowballThrower.dmxsoftware.Database.Type;
import SnowballThrower.dmxsoftware.Processing.Manage;
import java.util.List;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.ToggleGroup;
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
    ScrollPane dmx;
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
    Channels dmxChannels;
    double sizeX = 70;
    double sizeY = 200;
    double distY = 50;

    public ControlSurface(Manage mng, List<Device> devices, Channels channels) {
        manager = mng;
        dmxChannels = channels;
        this.devices = devices;
        stack = new StackPane();
        stage = new Stage();
        dmx = dmxChannels();
        deviceScene = allDevs();
        bars = bars();
        mng.register(this);//gets also  selected channel
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
                for (Device device : devices) {
                    if (device.getType().getType() == Type.Bar) {
                        Bar bar = new Bar(200, 10);
                        //stack.getChildren().add(bar.getGroup());
                    }
                }
                stack.setAlignment(Pos.TOP_CENTER);
            }
        });

        btn3.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                stack.getChildren().clear();
                stack.getChildren().add(dmx);
                stack.setAlignment(Pos.BOTTOM_CENTER);
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
        ScrollPane pane = new ScrollPane();
        pane.setVbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);

        Group deviceGroup = new Group();
        for (Device device : devices) {
            Group fG = faderGroup(device);
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

    Group deviceImage(Device device) {
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
        Font font = new Font(sizeY / 12);
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
        picture.setOnMouseClicked(new DevClickListener(manager, device.getStartCh()));
        return new Group(picture, texts);
    }

    ScrollPane bars() {
        ScrollPane pane = new ScrollPane();
        Group bars = new Group();
        Group red = getBars(Function.Red);
        red.setLayoutY((sizeY + distY) * 1);
        bars.getChildren().add(red);
        Group green = getBars(Function.Green);
        green.setLayoutY((sizeY + distY) * 2);
        bars.getChildren().add(green);
        Group blue = getBars(Function.Blue);
        blue.setLayoutY((sizeY + distY) * 3);
        bars.getChildren().add(blue);
        Group checkBoxes = getCheckBoxes();
        checkBoxes.setLayoutY(sizeY - 60);
        bars.getChildren().add(checkBoxes);
        bars.getChildren().add(new Rectangle(0, sizeY / 2, 0, 0));
        pane.setContent(bars);
        return pane;
    }

    ScrollPane dmxChannels() {
        Group dmx = new Group();
        ScrollPane pane = new ScrollPane();
        FaderListener fl = new FaderListener(manager);
        for (DMXChannel channel : dmxChannels.getAll()) {
            if (channel != null) {
                Fader fader = new Fader("" + (channel.getAdress()),
                        channel.getAdress() * sizeX, 0, sizeX, sizeY, channel, channel.getDevice());
                fl.addFader(fader);
                Group fade = fader.getFader();
                fade.addEventFilter(InputEvent.ANY, (event) -> {
                    if (event.getEventType().toString().contains("SCROLL")) {
                        //System.out.println(event.getEventType());

                        event.consume();
                    }
                });
                Text ch = new Text("Ch " + channel.getAdress());
                //ch.setLayoutX(channel.getAdress() * sizeX);
                ch.setLayoutY(-10);
                ch.setFont(new Font(10));
                fade.getChildren().add(ch);
                dmx.getChildren().add(fade);
            } else {
            }
        }
        for (Device device : devices) {
            Text name = new Text(device.getName());
            name.setLayoutX(device.getStartCh() * sizeX);
            name.setLayoutY(-20);
            name.setFont(new Font(15));
            dmx.getChildren().add(name);
        }
        dmx.setOnMouseClicked(fl);
        dmx.setOnMousePressed(fl);
        dmx.setOnMouseDragged(fl);
        pane.setContent(dmx);
        pane.setVbarPolicy(ScrollPane.ScrollBarPolicy.NEVER);
        pane.setHbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);
        return pane;
    }

    Group faderGroup(Device device) {

        FaderListener fl = new FaderListener(manager);
        Group faderGroup = new Group();
        faderGroup.getChildren().add(deviceImage(device));
        int ch = 2;
        System.out.println(device.getName());
        for (Channel channel : device.getChannels()) {
            if (channel != null) {
                Fader fader = new Fader("" + (device.getStartCh() + ch - 2),
                        ch * sizeX, 0, sizeX, sizeY, channel, device);
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

    private Group getBars(Function function) {
        FaderListener fl = new FaderListener(manager);
        Group faders = new Group();
        int i = 0;
        for (Device device : devices) {
            if (device.getType().getType() == Type.Bar) {
                for (DMXChannel channel : device.getChannels()) {
                    if (channel != null) {
                        if (channel.getchType().getFunction() == function) {
                            Fader fader = new Fader("" + (device.getStartCh() + i - 2),
                                    i * sizeX, 0, sizeX, sizeY, channel, device);
                            Group fade = fader.getFader();
                            fl.addFader(fader);
                            faders.getChildren().add(fade);
                            i++;
                        }
                    }
                }

            }
        }
        faders.setOnMouseClicked(fl);
        faders.setOnMousePressed(fl);
        faders.setOnMouseDragged(fl);

        return faders;
    }

    private Group getCheckBoxes() {
        Group boxes = new Group();
        for (int i = 0; i < 5; i++) {
            ToggleGroup group = new ToggleGroup();
            RadioButton clone = new RadioButton("Kopie");
            clone.setLayoutY(0);
            clone.setToggleGroup(group);
            RadioButton mirror = new RadioButton("Spiegel");
            mirror.setLayoutY(30);
            mirror.setToggleGroup(group);
            RadioButton extend = new RadioButton("Erweitern");
            extend.setLayoutY(60);
            extend.setToggleGroup(group);
            extend.setSelected(true);
            if (i >= 3) {
                mirror.setSelected(true);
            }
            Group triple = new Group(clone, mirror, extend);
            triple.setLayoutX(sizeX * Math.pow(2, i));
            boxes.getChildren().add(triple);
        }
        return boxes;
    }

    public void setRemFader(int ch) {
        try {
            //selection.setTranslateX(ch * sizeX);
            dmx.setHvalue(((double) ch) / 512 * (dmx.getHmax() - dmx.getHmin()));
        } catch (Exception ex) {

        }
    }
}
