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

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Build;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import slib.platform.android.ui.UiThread;

public class Android {
	
	public static void finishActivity(Activity activity)
	{
		try {
			activity.finish();
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public static InputStream openAsset(Activity activity, String path)
	{
		try {
			AssetManager assets = activity.getAssets();			
			return assets.open(path);
		} catch (Throwable e) {
			Logger.exception(e);
			return null;
		}
	}

	static String getCurrentLocale() {
		try {
			java.util.Locale locale =  java.util.Locale.getDefault();
			if (locale != null) {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
					String script = locale.getScript();
					if (script != null && script.length() > 0) {
						return locale.getLanguage() + "_" + script + "_" + locale.getCountry();
					} else {
						return locale.getLanguage() + "_" + locale.getCountry();
					}
				} else {
					return locale.getLanguage() + "_" + locale.getCountry();
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void showKeyboard(final Activity activity) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					showKeyboard(activity);
				}
			});
			return;
		}
		try {
			InputMethodManager imm = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
			if (imm != null) {
				View view = activity.getCurrentFocus();
				if (view != null) {
					imm.showSoftInput(view, 0);
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void dismissKeyboard(final Activity activity) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					dismissKeyboard(activity);
				}
			});
			return;
		}
		try {
			InputMethodManager imm = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
			if (imm != null) {
				View view = activity.getCurrentFocus();
				if (view != null) {
					imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
				} else {
					imm.hideSoftInputFromWindow(activity.getWindow().getDecorView().getWindowToken(), 0);
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static final int KEYBOARD_ADJUST_PAN = 1;
	public static final int KEYBOARD_ADJUST_RESIZE = 2;

	public static void setKeyboardAdjustMode(final Activity activity, int mode) {
		try {
			switch (mode) {
				case KEYBOARD_ADJUST_PAN:
					activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
					break;
				case KEYBOARD_ADJUST_RESIZE:
					activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
					break;
				default:
					activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_NOTHING);
					break;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void onCreateActivity(Activity activity)
	{
		nativeOnCreateActivity(activity);
	}
	
	public static void onResumeActivity(Activity activity)
	{
		nativeOnResumeActivity(activity);
	}
	
	public static void onPauseActivity(Activity activity)
	{
		nativeOnPauseActivity(activity);
	}
	
	public static void onDestroyActivity(Activity activity)
	{
		nativeOnDestroyActivity(activity);
	}
	
	public static boolean onBack(Activity activity)
	{
		return nativeOnBack(activity);
	}

	public static void onConfigurationChanged(Activity activity) {
		nativeOnConfigurationChanged(activity);
	}

	public static void onChangeWindowInsets(Activity activity) {
		nativeOnChangeWindowInsets(activity);
	}

	private static native void nativeOnCreateActivity(Activity activity);
	private static native void nativeOnResumeActivity(Activity activity);
	private static native void nativeOnPauseActivity(Activity activity);
	private static native void nativeOnDestroyActivity(Activity activity);
	private static native boolean nativeOnBack(Activity activity);
	private static native void nativeOnConfigurationChanged(Activity activity);
	private static native void nativeOnChangeWindowInsets(Activity activity);

}