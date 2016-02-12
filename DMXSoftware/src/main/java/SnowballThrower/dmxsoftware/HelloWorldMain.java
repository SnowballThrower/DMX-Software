/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware;

import SnowballThrower.dmxsoftware.Database.Function;
import SnowballThrower.dmxsoftware.Processing.Devices;
import SnowballThrower.dmxsoftware.Database.XMLReader;
import SnowballThrower.dmxsoftware.Surface.Fader;
import SnowballThrower.dmxsoftware.Surface.FaderListener;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

/**
 *
 * @author Sven
 */
public class HelloWorldMain extends Application {

    @Override
    public void start(Stage primaryStage) {
        Button btn = new Button();
        btn.setText("Say 'Hello World'");
        btn.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                System.out.println("Hello World!");
                Devices devices = new Devices();
            }
        });

        StackPane root = new StackPane();
        Fader fader1 = new Fader(Function.Red, 50, 50, 100, 500);
        Fader fader2 = new Fader(Function.Green, 150, 50, 100, 500);
        Fader fader3 = new Fader(Function.Blue, 250, 50, 100, 500);
        Fader fader4 = new Fader(Function.White, 350, 50, 100, 500);
        Fader fader5 = new Fader(Function.Amber, 450, 50, 100, 500);
        Fader fader6 = new Fader(Function.UV, 550, 50, 100, 500);
        Fader fader7 = new Fader(Function.Dimmer, 650, 50, 100, 500);
        FaderListener fl = new FaderListener();
        fl.addFader(fader1);
        fl.addFader(fader2);
        fl.addFader(fader3);
        fl.addFader(fader4);
        fl.addFader(fader5);
        fl.addFader(fader6);
        fl.addFader(fader7);
        root.getChildren().add(btn);
        Group group = new Group(root, fader1.getFader(), fader2.getFader(),
                fader3.getFader(), fader4.getFader(), fader5.getFader(),
                fader6.getFader(), fader7.getFader());
        Scene scene = new Scene(group, 300, 250);
        scene.setOnMouseClicked(fl);
        scene.setOnMousePressed(fl);
        scene.setOnMouseDragged(fl);
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
