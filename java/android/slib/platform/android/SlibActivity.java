package slib.platform.android;

import java.util.Vector;

import slib.platform.android.camera.SCamera;
import slib.platform.android.ui.window.UiWindow;
import android.app.Activity;
import android.os.Bundle;

public class SlibActivity extends Activity {

	boolean flagVisible;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Android.onCreateActivity(this);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		Android.onPauseActivity(this);
		SCamera.onPauseActivity(this);
		flagVisible = false;
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		Android.onResumeActivity(this);
		SCamera.onResumeActivity(this);
		flagVisible = true;
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Android.onDestroyActivity(this);
	}
	
	@Override
	public void onBackPressed() {
		if (!Android.onBack(this)) {
			return;
		}
		
		if (windows.isEmpty()) {
			super.onBackPressed();			
			return;
		}
		try {
			UiWindow window = windows.get(windows.size() - 1);
			window.onClose();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public boolean isVisible() {
		return flagVisible;
	}
	
	Vector<UiWindow> windows = new Vector<UiWindow>();
	public void onCreateWindow(UiWindow window) {
		windows.add(window);
	}
	
	public void onCloseWindow(UiWindow window) {
		windows.remove(window);
		if (windows.isEmpty()) {
			finish();
		}
	}
}
