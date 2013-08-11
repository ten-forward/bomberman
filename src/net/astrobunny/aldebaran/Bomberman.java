package net.astrobunny.aldebaran;

import org.libsdl.app.SDLActivity; 

import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import tv.ouya.console.api.OuyaController;

/* 
 * A sample wrapper class that just calls SDLActivity 
 */ 

public class Bomberman extends SDLActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        Log.i("JAVA_BOMBERMAN", "onCreate");  

    	OuyaController.init(this);
    }


    @Override
    public boolean onKeyDown(final int keyCode, KeyEvent event){
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
        Log.i("JAVA_BOMBERMAN", "onKeyDown");
        onControllerButtonDown(keyCode, player);
        return true;
    }

    @Override
    public boolean onKeyUp(final int keyCode, KeyEvent event){
        //Get the player #
        int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
        Log.i("JAVA_BOMBERMAN", "onKeyUp");
        onControllerButtonUp(keyCode, player);
        return true;
    }

    public static native void onControllerButtonDown(int button, int which);
    public static native void onControllerButtonUp(int button, int which);
}
