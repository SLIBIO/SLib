package slib.platform.android;

import java.util.Vector;

import slib.platform.android.camera.SCamera;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.view.UiGLView;
import slib.platform.android.ui.window.UiWindow;
import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;

public class SlibActivity extends Activity {

	boolean flagVisible;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Android.onCreateActivity(this);
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		Android.onPauseActivity(this);
		SCamera.onPauseActivity(this);
		UiGLView.onPauseViews();
		flagVisible = false;
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		Android.onResumeActivity(this);
		SCamera.onResumeActivity(this);
		UiGLView.onResumeViews();
		flagVisible = true;
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Android.onDestroyActivity(this);
		windows.removeAllElements();
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
	
	public boolean onCloseWindow(UiWindow window) {
		if (windows.contains(window)) {
			windows.remove(window);
			if (windows.isEmpty()) {
				finish();
				return true;
			}
		}
		return false;
	}
	
	@Override
	public boolean dispatchTouchEvent(final MotionEvent ev) {
		// avoid crash on using UI::runLoop
		final MotionEvent evNew = MotionEvent.obtain(ev);
		UiThread.post(new Runnable() {
			public void run() {
				dispatchTouchEventToSuper(evNew);
			}
		});
		return true;
	}
	
	void dispatchTouchEventToSuper(final MotionEvent ev) {
		try {
			super.dispatchTouchEvent(ev);
		} catch (Exception e) {
			Logger.exception(e);
		}
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
		try {
			super.dispatchKeyEvent(ev);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
}
