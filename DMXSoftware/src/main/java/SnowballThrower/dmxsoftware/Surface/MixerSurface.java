package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Surface.Mixer.Display;
import SnowballThrower.dmxsoftware.Surface.Mixer.MotorFader;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 * Created by Sven on 16.04.2017.
 */
public class MixerSurface {

    Stage stage;

    MotorFader faders[];
    Display displays[];
    int noFaders = 12;
    int noDisplays = 3;
    double offXFader = 50;
    double offYFader = 400;
    double faderSizeX = 50;
    double faderSizeY = 200;
    double offXDisplay = 50;
    double offYDisplay = 300;
    double displaySizeX = 200;
    double displaySizeY = 100;

    Group surface;
    Group faderGroup;
    Group displayGroup;

    public MixerSurface(Stage stage) {
        this.stage = stage;
        faderGroup = new Group();
        displayGroup = new Group();
        faders = new MotorFader[noFaders];
        displays = new Display[noDisplays];
        for (int i = 0; i < noFaders; i++) {
            MotorFader fader = new MotorFader(offXFader + i * faderSizeX, offYFader, faderSizeX, faderSizeY);
            faders[i] = fader;
            faderGroup.getChildren().add(fader.getFader());
        }
        for (int i = 0; i < noDisplays; i++) {
            Display display = new Display(offXDisplay + i * displaySizeX, offYDisplay, displaySizeX, displaySizeY);
            displays[i] = display;
            Group disp = display.getDisp();
            displayGroup.getChildren().add(disp);
        }
        surface = new Group(displayGroup, faderGroup);
        stage.setScene(new Scene(surface));

        stage.setTitle("Control Surface");
        //pane.setMaxHeight(stage.getHeight());
        stage.show();

        texting();
    }

    void texting() {
        displays[1].setText(0, 0, "erstes");
        displays[1].setText(1, 1, "zwei");
        displays[1].setText(3, 3, "letztes");
    }
}
