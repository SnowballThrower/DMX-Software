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
import SnowballThrower.dmxsoftware.Surface.SettingSurface;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 * @author Sven
 */
public class HelloWorldMain extends Application {
    
    @Override
    public void start(Stage primaryStage) {
        Manage mng = new Manage();
        Button btn1 = new Button();
        Button btn2 = new Button();
        btn2.setTranslateY(30);
        Devices devices = new Devices(mng);
        btn1.setText("ControlSurface");
        btn1.setOnAction(new EventHandler<ActionEvent>() {
            
            @Override
            public void handle(ActionEvent event) {
                mng.startMidi();
                ControlSurface cs = new ControlSurface(mng, devices.getDevices(), devices.getChannels());
            }
        });
        
        btn2.setText("Settings");
        btn2.setOnAction(new EventHandler<ActionEvent>() {
            
            @Override
            public void handle(ActionEvent event) {
                SettingSurface ss = new SettingSurface(new Stage(), devices.getDevices(), devices.getTypes());
            }
        });
        
        StackPane root = new StackPane(btn1,btn2);
        Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Hello World!");
        Platform.setImplicitExit(false);
        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            
            @Override
            public void handle(WindowEvent event) {
                Stage dialog = new Stage();
                Text text = new Text("Are You sure, You want to exit the Application?");
                Button ok = new Button("OK");
                ok.setOnAction(new EventHandler<ActionEvent>() {

                    @Override
                    public void handle(ActionEvent t) {
                        dialog.close();
                        System.exit(1);
                    }
                });
                Button cancel = new Button("Cancel");
                cancel.setOnAction(new EventHandler<ActionEvent>() {

                    @Override
                    public void handle(ActionEvent t) {
                        dialog.close();
                    }
                });
                text.setTranslateX(20);
                text.setTranslateY(20);
                ok.setTranslateX(150);
                ok.setTranslateY(50);
                cancel.setTranslateX(80);
                cancel.setTranslateY(50);
                Group menu = new Group(text, ok, cancel);
                Scene closing = new Scene(menu);
                dialog.setScene(closing);
                dialog.setTitle("Shutdown?");
                dialog.show();
                event.consume();
            }
        });
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
