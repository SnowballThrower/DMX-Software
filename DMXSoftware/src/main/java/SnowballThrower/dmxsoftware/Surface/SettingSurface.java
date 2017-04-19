/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Database.DeviceType;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;
import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.ScrollPane;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.util.Callback;

/**
 *
 * @author Sven
 */
public class SettingSurface extends Application {

    Stage settings;
    Scene general;
    Scene devices;
    Group devs;
    Group deviceScene;
    List<Device> deviceList;
    double sizeX = 40;
    double sizeY = 40;
    List<DeviceType> typeList;

    public SettingSurface(Stage settings, List<Device> deviceList, List<DeviceType> typeList) {
        devs = new Group();
        this.settings = settings;
        this.deviceList = deviceList;
        this.typeList = typeList;
        deviceScene = new Group(devices());
        ScrollPane pane = new ScrollPane(deviceScene);
        settings.setScene(new Scene(pane));
        settings.setTitle("Setting Surface");
        settings.show();
    }

    Group devices() {
        devs.getChildren().clear();
        int counter = 0;
        for (Device device : deviceList) {

            Button rmB = new Button("-");
            rmB.setOnAction(new EventHandler<ActionEvent>() {

                @Override
                public void handle(ActionEvent t) {
                    deviceList.remove(device);
                    deviceScene = devices();
                }
            });

            ComboBox<DeviceType> names = new ComboBox<DeviceType>();
            names.setTranslateX(sizeX * 2);
            names.setEditable(false);
            names.setOnMousePressed(new EventHandler<MouseEvent>() {
                @Override
                public void handle(MouseEvent event) {
                    names.requestFocus();
                }
            });
            names.getItems().addAll(typeList);
            names.setValue(device.getType());
            Callback<ListView<DeviceType>, ListCell<DeviceType>> cellFac = new Callback<ListView<DeviceType>, ListCell<DeviceType>>() {
                @Override
                public ListCell<DeviceType> call(ListView<DeviceType> p) {
                    return new ListCell<DeviceType>() {
                        private final Group group;

                        {
                            setContentDisplay(ContentDisplay.GRAPHIC_ONLY);
                            group = new Group();
                        }

                        @Override
                        protected void updateItem(DeviceType item, boolean empty) {
                            super.updateItem(item, empty);

                            if (item == null || empty) {
                                setGraphic(null);
                            } else {
                                Text text = new Text(item.getName());
                                setGraphic(text);
                            }
                        }
                    };
                }
            };
            names.setButtonCell(cellFac.call(new ListView<DeviceType>()));
            names.setCellFactory(cellFac);
            Group pic = new Group(getPicture(names.getValue()));
            Group dev = new Group(rmB, pic, names);
            names.setOnAction(new EventHandler<ActionEvent>() {

                @Override
                public void handle(ActionEvent t) {
                    Group newPic = getPicture((names.getValue()));
                    pic.getChildren().clear();
                    pic.getChildren().add(newPic);
                    device.setType(names.getValue());
                }

            });
            dev.setTranslateY(sizeY * counter);
            devs.getChildren().add(dev);
            counter++;
        }

        Button adB = new Button("+");
        adB.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent t) {
                deviceList.add(new Device());
                deviceScene = devices();
            }
        });
        Group addButton = new Group(adB);

        addButton.setTranslateY(sizeY
                * counter);
        devs.getChildren()
                .add(addButton);
        return devs;
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        new SettingSurface(primaryStage, new LinkedList<Device>(), new LinkedList<DeviceType>());
    }

    Group getPicture(DeviceType selected) {
        Rectangle rect = new Rectangle(0, 0, sizeX, sizeY);
        rect.setFill(Color.WHITE);
        Group picture = new Group(rect);
        try {
            Image image = new Image(selected.getPicturePath());
            ImageView imV = new ImageView(image);
            imV.setPreserveRatio(true);
            imV.setFitHeight(sizeX);

            picture.getChildren().add(imV);
        } catch (Exception ex) {
            System.out.println(ex);
        }
        picture.setTranslateX(sizeX);
        return picture;
    }
}
