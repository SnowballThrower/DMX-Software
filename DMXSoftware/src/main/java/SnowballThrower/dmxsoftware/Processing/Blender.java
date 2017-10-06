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

    public Blender(Manage mng) {
        this.mng = mng;
        channels = mng.devs.getChannels();
    }

    public void blendStep(Scene oldScene, Scene newScene, float alpha) {
        if (newScene == null || oldScene == null)
            return;
        for (int i = 0; i < 512; i++) {
            int val = (int) (oldScene.getValues()[i] * alpha + oldScene.getValues()[i] * (1 - alpha));
            channels.get(i).setValue(val);
        }
    }

    public void blend(Scene oldScene, Scene newScene, double time) {
        System.out.println("blending " + oldScene.getName() + " to " + newScene.getName() + " in " + time + "s");
        BlendSequence bs = new BlendSequence(oldScene, newScene, time, channels);
        bs.start();
    }

    public class BlendSequence extends Thread {
        Channels channels;
        double time;
        Scene oldScene, newScene;

        public BlendSequence(Scene oldScene, Scene newScene, double time, Channels channels) {
            this.channels = channels;
            this.newScene = newScene;
            this.oldScene = oldScene;
            this.time = time;
        }

        @Override
        public void run() {
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
                readyTime += sleepTime;
            }
            blendStep(1);
        }

        private void blendStep(double alpha) {
            if (newScene == null || oldScene == null)
                return;
            //System.out.println("blending "+ alpha);
            int newVals[] = newScene.getValues();
            int oldVals[] = oldScene.getValues();
            for (int i = 0; i < 512; i++) {
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
