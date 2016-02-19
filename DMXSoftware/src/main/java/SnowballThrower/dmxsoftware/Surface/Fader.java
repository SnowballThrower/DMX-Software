/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.TypeChannel;
import SnowballThrower.dmxsoftware.Database.Device;
import SnowballThrower.dmxsoftware.Database.Function;
import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Paint;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;

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
    Text name;
    Text meaning;
    Channel channel;
    int max = 255;
    private String Id;
    Device device;

    public Fader(String id, double posX, double posY, double width, double height, Channel channel, Device device) {
        channel.register(this);
        this.function = channel.getFunction();
        this.width = width;
        this.height = height;
        this.posX = posX;
        this.posY = posY;
        this.channel = channel;
        this.device = device;
        frameY = height / 10;
        frameX = width / 10;
        background = getBackGround();
        head = getHead();
        name = new Text(device.getChannelName(channel.getNumber() - 1));
        meaning = new Text(device.getMeaning(channel.getNumber() - 1));
        meaning.setLayoutY(height + 10);
        fader = new Group(background, head, name, meaning);
        fader.setLayoutX(posX);
        fader.setLayoutY(posY);
        this.Id = id;
        act();
    }

    public void setValue(int value) {
        if (value >= 0 && value < 256) {
            this.value = value;
            channel.setValue(value);
            head.setTranslateY(-((height - 2 * frameY) / max) * value);
            meaning.setText(device.getMeaning(channel.getNumber() - 1));
            name.setText(device.getChannelName(channel.getNumber() - 1));
            //System.out.println(value);
        }
    }

    public void act() {
        if (channel.getValue() >= 0 && channel.getValue() < 256) {
            this.value = channel.getValue();
        }
        try {
            head.setTranslateY(-((height - 2 * frameY) / max) * value);
        } catch (Exception ex) {

        }
        meaning.setText(device.getMeaning(channel.getNumber() - 1));
        name.setText(device.getChannelName(channel.getNumber() - 1));
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
        switch (function) {
            case Red:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.RED)};
                break;
            case Green:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.LIMEGREEN)};
                break;
            case Blue:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.BLUE)};
                break;
            case White:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.WHITESMOKE)};
                break;
            case Amber:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.GOLD)};
                break;
            case UV:
                stops = new Stop[]{new Stop(0, colorOut), new Stop(1, Color.BLUEVIOLET)};
                break;
            case Dimmer:
                stops = new Stop[]{new Stop(0, Color.BLACK), new Stop(1, Color.WHITE)};
                break;
            default:
                gradient = false;
        }
        if (gradient) {
            LinearGradient lg1 = new LinearGradient(0, 1, 0, 0, true, CycleMethod.NO_CYCLE, stops);
            rect.setFill(lg1);
        }
        switch (function) {
            case Macro:
                for (int i = 0; i < max; i++) {
                    Rectangle mac = new Rectangle(
                            0, height - frameY - ((height - 2 * frameY) / max) * i,
                            width, (height - 2 * frameY) / max + 1);

                    String string = channel.getchType().getStandardMeaning(i);
                    //System.out.println(string);
                    try {
                        string = string.toUpperCase();
                        mac.setFill(Paint.valueOf(string));
                    } catch (Exception ex) {
                        mac.setFill(Color.LIGHTGRAY);
                    }
                    backGround.getChildren().add(mac);
                }
                Rectangle bg = new Rectangle(0, 0, width, height);
                bg.setFill(null);
                bg.setStroke(Color.BLACK);
                backGround.getChildren().add(bg);
                break;
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

    String getID() {
        return Id;
    }
}
