/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android;

import java.util.Vector;

import slib.platform.android.camera.SCamera;
import slib.platform.android.camera.TakePhoto;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.view.UiGLView;
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

	public static final int REQUEST_IMAGE_CAPTURE = 0x0000ff;
	
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
		}
	}
}
