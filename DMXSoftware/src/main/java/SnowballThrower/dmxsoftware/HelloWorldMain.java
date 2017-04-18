/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware;

import SnowballThrower.dmxsoftware.Processing.Devices;
import SnowballThrower.dmxsoftware.Processing.Manage;
import SnowballThrower.dmxsoftware.Surface.ControlSurface;
import SnowballThrower.dmxsoftware.Surface.MixerSurface;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

/**
 * @author Sven
 */
public class HelloWorldMain extends Application {

    @Override
    public void start(Stage primaryStage) {
        Manage mng = new Manage();
        Button btn = new Button();
        Devices devices = new Devices(mng);
        btn.setText("Say 'Hello World'");
        btn.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                System.out.println("Hello World!");
                mng.startMidi();
                //MixerSurface ms = new MixerSurface(new Stage());
                ControlSurface cs = new ControlSurface(mng, devices.getDevices(), devices.getChannels());
            }
        });

        StackPane root = new StackPane();
        root.getChildren().add(btn);
        Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Hello World!");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }

}
