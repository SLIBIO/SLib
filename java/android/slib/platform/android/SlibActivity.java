/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

package slib.platform.android;

import java.util.Vector;

import slib.platform.android.camera.SCamera;
import slib.platform.android.camera.TakePhoto;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.view.UiGLView;
import slib.platform.android.ui.view.UiWebView;
import slib.platform.android.ui.window.UiWindow;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

public class SlibActivity extends Activity {

	boolean flagVisible;

	public static final int REQUEST_IMAGE_CAPTURE = 0x000100;
	public static final int REQUEST_WEBVIEW_CHOOSE_FILE = 0x000101;

	public static final int REQUEST_PERMISSIONS = 0x000201;
	public static final int REQUEST_SCAMERA_PERMISSION = 0x000201;

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);

		Android.onCreateActivity(this);

		Window window = getWindow();
		window.setFlags(WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED, WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED);
		window.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
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
	public boolean dispatchKeyEvent(final KeyEvent ev) {
		return super.dispatchKeyEvent(ev);
	}

	@Override
	public boolean dispatchTouchEvent(final MotionEvent ev) {
		// avoid crash on using UI::runLoop
		final MotionEvent evNew = MotionEvent.obtain(ev);
		UiThread.post(new Runnable() {
			public void run() {
				dispatchTouchEventToSuper(evNew);
				evNew.recycle();
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
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQUEST_IMAGE_CAPTURE) {
			TakePhoto.onResult(this, resultCode, data);
		} else if (requestCode == REQUEST_WEBVIEW_CHOOSE_FILE) {
			UiWebView.onResult(this, resultCode, data);
		}
	}

	public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
		if (requestCode == REQUEST_SCAMERA_PERMISSION) {
			SCamera.onRequestPermissionsResult();
		}
	}

}
