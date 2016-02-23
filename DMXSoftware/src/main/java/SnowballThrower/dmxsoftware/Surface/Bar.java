/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Surface;

import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

/**
 *
 * @author Sven
 */
public class Bar {

    int segments = 8;
    Group group;
    Rectangle[] rects;
    Group bg;
    double width;
    double height;

    public Bar(double width, double height) {
        this.width = width;
        this.height = height;
        double xSize = width / (segments + 1);
        double frameY = height / 10;
        rects = new Rectangle[segments];
        bg = new Group(new Rectangle(0, 0, width, height));
        group = new Group(bg);
        for (int i = 0; i < segments; i++) {
            rects[i] = new Rectangle(xSize * (i + 0.5), frameY, xSize, height - 2 * frameY);
            group.getChildren().add(rects[i]);
        }

    }

    void setColor(int seg, int red, int green, int blue) {
        try {
            rects[seg].setFill(Color.rgb(red, green, blue));
        } catch (Exception e) {

        }
    }

    Group getGroup() {
        return group;
    }
}
