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
        TextField nameField = new TextField("Scene-name");
        TextField timeField = new TextField("blending-time");
        create.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                scenes.saveScene(Scene.toScene(nameField.getText(),channels.getAll()));
            }
        });
        buttons.add(nameField,0,0);
        buttons.add(create, 0, 1);
        buttons.add(timeField, 0, 2);
        int i = 2;
        for (String name : scenes.getNames()){
            i++;
            Button scnButton = new Button(name);
            scnButton.setOnAction(new EventHandler<ActionEvent>() {
                @Override
                public void handle(ActionEvent event) {
                    double time = 5.0;
                    try {
                        time = Double.parseDouble(timeField.getText());
                    } catch (Exception e){
                        time = 5.0;
                    }

                    blender.blend(Scene.toScene("tmp",channels.getAll()),scenes.readScene(name),time);
                }
            });
            buttons.add(scnButton,0,i);
        }
        return buttons;
    }
}
