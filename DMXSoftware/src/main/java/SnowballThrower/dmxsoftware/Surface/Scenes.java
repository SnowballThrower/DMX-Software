package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channels;
import SnowballThrower.dmxsoftware.Database.Scene;
import SnowballThrower.dmxsoftware.Processing.Blender;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;

/**
 * Created by Sven on 06.10.2017.
 */
public class Scenes {

    public GridPane getSceneButtons(Blender blender, Channels channels){
        Group btns = new Group();
        SnowballThrower.dmxsoftware.Database.Scenes scenes = new SnowballThrower.dmxsoftware.Database.Scenes();
        GridPane buttons = new GridPane();
        Button create = new Button("save as Scene");
        TextField nameField = new TextField();
        create.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                scenes.saveScene(Scene.toScene(nameField.getText(),channels.getAll()));
            }
        });
        int i = 1;
        buttons.add(nameField,0,0);
        buttons.add(create, 0, i);
        for (String name : scenes.getNames()){
            i++;
            Button scnButton = new Button(name);
            scnButton.setOnAction(new EventHandler<ActionEvent>() {
                @Override
                public void handle(ActionEvent event) {
                    blender.blend(Scene.toScene("tmp",channels.getAll()),scenes.readScene(name),5.0);
                }
            });
            buttons.add(scnButton,0,i);
        }
        return buttons;
    }
}
