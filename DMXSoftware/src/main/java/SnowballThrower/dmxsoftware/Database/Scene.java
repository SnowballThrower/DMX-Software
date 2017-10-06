package SnowballThrower.dmxsoftware.Database;

/**
 * Created by Sven on 06.10.2017.
 */
public class Scene {
    String name;
    int values[];
    public Scene(String name, int values[]){
        this.name = name;
        this.values = values;
    }

    public int[] getValues() {
        return values;
    }

    public String getName() {
        return name;
    }

    public Scene(String name){
        this.name = name;

        this.values = new int[600];
    }

    public static Scene toScene(String name, Channel channels[]) {
        int l = channels.length;
        int values[] = new int[l];
        for (int i = 0; i < l; i++) {
            if(channels[i] != null) {
                values[i] = channels[i].getValue();
            }else {
                values[i] = 0;
                //System.out.println("null pointer at toScene channel "+ i);
            }
        }
        return new Scene(name, values);
    }
}
