/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Function;
import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;

/**
 *
 * @author Sven
 */
public class Fader {

    int value;
    double height;
    double width;
    double frameY;
    double frameX;
    double posX;
    double posY;
    Function function;
    Group background;
    Group head;
    Group fader;
    int max = 255;

    public Fader(double posX, double posY, double width, double height, Channel channel) {
        this.function = channel.getFunction();
        this.width = width;
        this.height = height;
        this.posX = posX;
        this.posY = posY;
        frameY = height / 20;
        frameX = width / 10;
        background = getBackGround();
        head = getHead();
        fader = new Group(background, head);
        fader.setLayoutX(posX);
        fader.setLayoutY(posY);
    }

    public void setValue(int value) {
        this.value = value;
        head.setTranslateY(-((height - 2 * frameY) / max) * value);
        System.out.println(value);
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
        double vert = height * 0.05;
        Rectangle rect = new Rectangle(frameX, height - frameY - 0.5 * vert, width - 2 * frameX, vert);
        rect.setFill(Color.GREY);
        return new Group(rect);
    }

    private Group getBackGround() {
        Rectangle rect = new Rectangle(0, 0, width, height);
        rect.setFill(Color.LIGHTGRAY);
        rect.setStroke(Color.BLACK);
        switch (function) {
            case Red:
                rect.setFill(Color.RED);
                break;
            case Green:
                rect.setFill(Color.LIMEGREEN);
                break;
            case Blue:
                rect.setFill(Color.BLUE);
                break;
            case White:
                rect.setFill(Color.WHITESMOKE);
                break;
            case Amber:
                rect.setFill(Color.GOLD);
                break;
            case UV:
                rect.setFill(Color.BLUEVIOLET);
                break;
        }
        Line line = new Line(width * 0.5, frameY, width * 0.5, height - frameY);
        return new Group(rect, line);
    }

    double getWidth() {
        return width;
    }

    double getHeight() {
        return height;
    }
}
