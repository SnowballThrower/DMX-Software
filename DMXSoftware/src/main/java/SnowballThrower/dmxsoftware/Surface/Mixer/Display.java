package SnowballThrower.dmxsoftware.Surface.Mixer;


import com.sun.xml.internal.ws.util.StringUtils;
import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.scene.text.TextBoundsType;

import static SnowballThrower.dmxsoftware.Processing.Util.StringUtils.*;

/**
 * Created by Sven on 16.04.2017.
 */
public class Display {
    String content[];
    Text lines[];
    int lineNumber = 4;
    int colNumber = 20;
    int colLength = colNumber / 4;

    Color background = Color.BEIGE;
    Color foreground = Color.BLACK;

    double sizeX, sizeY;
    double posX, posY;

    Group texts;
    Group disp;


    public Display(double offXDisplay, double offYDisplay, double displaySizeX, double displaySizeY) {
        posX = offXDisplay;
        posY = offYDisplay;
        sizeX = displaySizeX;
        sizeY = displaySizeY;
        content = new String[lineNumber];
        lines = new Text[lineNumber];
        texts = new Group();
        for (int i = 0; i < lineNumber; i++) {
            content[i] = fillRight("", colNumber, '.');
            Text text = new Text(content[i]);
            text.setTextAlignment(TextAlignment.CENTER);
            text.resize(sizeX, sizeY / lineNumber);
            text.setFill(foreground);
            text.setTranslateX(sizeX / 2);
            //text.setFont(Font.font(1));
            //text.setScaleX(sizeX / colNumber * 2);
            text.setTranslateY(i * sizeY / lineNumber);
            //text.setScaleY(sizeY / lineNumber);
            lines[i] = text;
            texts.getChildren().add(text);
        }
        Rectangle frame = new Rectangle(sizeX, sizeY, background);
        frame.setStroke(Color.BLACK);
        disp = new Group(frame, texts);
        disp.setTranslateX(posX);
        disp.setTranslateY(posY);
    }

    public void setText(int line, String text) {
        if (text == null) {
            content[line] = fillRight("", colNumber);
        }
        if (line >= 0 && line < lineNumber) {
            content[line] = fillRight(text, colNumber);
            lines[line].setText(text);
        }
    }

    public void setText(int line, int col, String text) {
        if (line >= 0 && line < lineNumber) {
            if (col >= 0 && line < colNumber) {
                if (text == null) text = "";
                text = fillRight(text, colLength);
                content[line] = content[line].substring(0, col * colLength) + text +
                        content[line].substring((col + 1) * colLength, colNumber);
                lines[line].setText(content[line]);
            }
        }
    }

    public Group getDisp() {
        return disp;
    }
}
