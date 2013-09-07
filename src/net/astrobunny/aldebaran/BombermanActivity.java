package net.astrobunny.aldebaran;

import org.libsdl.app.SDLActivity;
import tv.ouya.console.api.OuyaController;
import android.os.Bundle;

/* 
 * A sample wrapper class that just calls SDLActivity 
 */ 

public class BombermanActivity extends SDLActivity
{
    public static native void onOuyaControllerKey(int player, int keycode, int action);
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	OuyaController.init(this);
		mLayout.removeAllViews();
		mSurface = new BombermanSurface(getApplication());
		mLayout.addView(mSurface);
    }
}
