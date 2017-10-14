package SnowballThrower.dmxsoftware.Processing;

import SnowballThrower.dmxsoftware.Communicate.MidiConnection;
import SnowballThrower.dmxsoftware.Database.Channel;
import SnowballThrower.dmxsoftware.Database.Channels;
import SnowballThrower.dmxsoftware.Database.Scene;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Created by Sven on 06.10.2017.
 */
public class Blender {
    Manage mng;
    Channels channels;
    BlendSequence b;

    public Blender(Manage mng) {
        this.mng = mng;
        channels = mng.devs.getChannels();
    }


    public void blend(Scene oldScene, Scene newScene, double time) {
        System.out.println("blending " + oldScene.getName() + " to " + newScene.getName() + " in " + time + "s");
        if(b!=null){
            b.kill();
//            b.destroy();
        }
        BlendSequence bs = new BlendSequence(oldScene, newScene, time, channels);
        bs.start();
        b = bs;
    }

    public class BlendSequence extends Thread {
        Channels channels;
        double time;
        Scene oldScene, newScene;
        boolean stop;

        public BlendSequence(Scene oldScene, Scene newScene, double time, Channels channels) {
            this.channels = channels;
            this.newScene = newScene;
            this.oldScene = oldScene;
            this.time = time;
            stop = false;
        }

        public void kill(){
            stop = true;
        }

        @Override
        public void run() {
            System.out.println("start blend");
            double alpha = 0;
            double readyTime = 0;
            int sleepTime = 40;
            while (alpha < 1.0) {
                alpha = readyTime / 1000 / time;
                blendStep(alpha);
                try {
                    Thread.sleep(sleepTime);

                } catch (InterruptedException ex) {
                    Logger.getLogger(MidiConnection.class
                            .getName()).log(Level.SEVERE, null, ex);
                }
                if(stop) {
                    return;
                }
                readyTime += sleepTime;
            }
            blendStep(1);
            System.out.println("stop blend");
        }

        private void blendStep(double alpha) {
            if (newScene == null || oldScene == null)
                return;
            //System.out.println("blending "+ alpha);
            int newVals[] = newScene.getValues();
            int oldVals[] = oldScene.getValues();
            for (int i = 0; i < 512; i++) {
                if(stop) {
                    return;
                }
                int val = (int) (newVals[i] * alpha + oldVals[i] * (1.0 - alpha));
                Channel ch = channels.get(i);
                if (ch != null) {
                    ch.setValue(val);
                    //if(i < 30)
                        //System.out.println("blend "+i+ " to " + val);
                } else {
                    //System.out.println("blend "+i+ " failed");
                }
            }
        }
    }
}
