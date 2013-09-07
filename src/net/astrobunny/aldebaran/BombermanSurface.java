package net.astrobunny.aldebaran;

import org.libsdl.app.SDLSurface;

import tv.ouya.console.api.OuyaController;
import android.content.Context;
import android.view.KeyEvent;
import android.view.View;

class BombermanSurface extends SDLSurface {
	
	public static native void onOuyaControllerKey(int player, int keyCode, int action);
	
	public BombermanSurface(Context context) {
		super(context);
	}
	
	// Key events
    @Override
    public boolean onKey(View  v, int keyCode, KeyEvent event) {
    	int player = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
    	onOuyaControllerKey(player, keyCode, event.getAction());
        return true;
    }
}
