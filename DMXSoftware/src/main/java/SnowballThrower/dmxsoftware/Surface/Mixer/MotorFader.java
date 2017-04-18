package SnowballThrower.dmxsoftware.Surface.Mixer;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Database.Function;
import javafx.scene.Group;
import javafx.scene.paint.*;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.StrokeType;
import javafx.scene.text.Text;

/**
 * Created by Sven on 16.04.2017.
 */
public class MotorFader {
    byte value;
    double height;
    double width;
    double frameY;
    double frameX;
    double posX;
    double posY;
    Group background;
    Group head;
    Group fader;
    int max = 255;
    Rectangle high;

    public MotorFader(double posX, double posY, double width, double height) {
        this.width = width;
        this.height = height;
        this.posX = posX;
        this.posY = posY;
        frameY = height / 10;
        frameX = width / 10;
        background = getBackGround();
        head = getHead();
        fader = new Group(background, head);
        fader.setLayoutX(posX);
        fader.setLayoutY(posY);
        makeFrame();
    }

    public void setValue(byte value) {
        if (value >= 0 && value < 256) {
            this.value = value;
            head.setTranslateY(-((height - 2 * frameY) / max) * value);
            //System.out.println(value);
        }
    }

    double getFrameX() {
        return frameX;
    }

    double getFrameY() {
        return frameY;
    }

    double getPosX() {
        return posX;
    }

    double getPosY() {
        return posY;
    }

    int getValue() {
        return value;
    }

    public Group getFader() {
        return this.fader;
    }

    void setStyle(int style) {

    }

    private Group getHead() {
        double vert = height * 0.1;
        Rectangle rect = new Rectangle(frameX, height - frameY - 0.5 * vert, width - 2 * frameX, vert);
        rect.setFill(Color.GREY);
        rect.setStroke(Color.BLACK);
        return new Group(rect);
    }

    private Group getBackGround() {
        Group backGround = new Group();
        Rectangle rect = new Rectangle(0, 0, width, height);
        rect.setFill(Color.LIGHTGRAY);
        rect.setStroke(Color.BLACK);
        backGround.getChildren().add(rect);

        Stop[] stops = new Stop[]{new Stop(0, Color.LIGHTGREY), new Stop(1, Color.LIGHTGREY)};
        Color colorOut = Color.LIGHTGREY;
        boolean gradient = true;
        gradient = false;

        if (gradient) {
            LinearGradient lg1 = new LinearGradient(0, 1, 0, 0, true, CycleMethod.NO_CYCLE, stops);
            rect.setFill(lg1);
        }


        Line line = new Line(width * 0.5, frameY, width * 0.5, height - frameY);
        return new Group(backGround, line);
    }

    double getWidth() {
        return width;
    }

    double getHeight() {
        return height;
    }


    public void highlight(boolean sel) {
        if (sel) {
            fader.getChildren().add(high);
        } else {
            fader.getChildren().remove(high);
        }
    }

    private void makeFrame() {
        high = new Rectangle(0, 0, width, height);
        high.setFill(Color.TRANSPARENT);
        high.setStroke(Color.BLACK);
        high.setStrokeType(StrokeType.INSIDE);
        high.setStrokeWidth(frameX);
    }
}
