package SnowballThrower.dmxsoftware.Database;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Sven on 06.10.2017.
 */
public class Scenes {
    List<Scene> scenes;

    public List<Scene> getScenes() {
        return scenes;
    }

    public Scenes(){
        scenes = new ArrayList<>();
        for (String name : getNames()){
            scenes.add(readScene(name));
        }
    }

    public List<String> getNames(){
        List<String> names = new ArrayList<>();
        File folder = new File("Data/Scenes/");
        File[] fileList = folder.listFiles();
        if(fileList == null)
            return names;
        for(File file : fileList){
            if(file.isFile()){
                names.add(file.getName().replace(".scn",""));
            }
        }
        return names;
    }

    public void saveScene(Scene scene){
        if(scene == null){
            return;
        }
        String name = scene.name;
        int values[] = scene.values;
        try {
            FileWriter fileWriter = new FileWriter("Data/Scenes/" + name + ".scn");
            for (int i = 0; i < values.length; i++){
                fileWriter.write("" + values[i]+'\n');
            }
            fileWriter.close();
        } catch (Exception e){

        }
    }
    public Scene readScene(String name){
        int values[] = new int[512];
        try {
            File file = new File("Data/Scenes/" + name + ".scn");
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            StringBuffer stringBuffer = new StringBuffer();
            String line;
            int i = 0;
            while ((line = bufferedReader.readLine()) != null && i <512) {
                values[i] = Integer.parseInt(line);
                //System.out.println(i+": "+values[i]);
                i++;
            }
            fileReader.close();
            return new Scene(name, values);
        } catch (Exception e){
            return null;
        }
    }
}
