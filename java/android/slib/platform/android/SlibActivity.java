package slib.platform.android;

import java.util.Vector;

import slib.platform.android.camera.SCamera;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.window.UiWindow;
import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

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
	
	@Override
	public void finish() {
		super.finish();
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
	
	@Override
	public boolean dispatchTouchEvent(final MotionEvent ev) {
		// avoid crash on using UI::runLoop
		UiThread.post(new Runnable() {
			public void run() {
				dispatchTouchEventToSuper(ev);
			}
		});
		return true;
	}
	
	void dispatchTouchEventToSuper(final MotionEvent ev) {
		super.dispatchTouchEvent(ev);
	}
	
	@Override
	public boolean dispatchKeyEvent(final KeyEvent ev) {
		// avoid crash on using UI::runLoop
		UiThread.post(new Runnable() {
			public void run() {
				dispatchKeyEventToSuper(ev);
			}
		});
		return true;
	}
	
	public void dispatchKeyEventToSuper(final KeyEvent ev) {
		super.dispatchKeyEvent(ev);
	}
	
}
