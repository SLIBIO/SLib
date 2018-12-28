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

package slib.platform.android.ui;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.Surface;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import java.lang.reflect.Field;
import java.util.Vector;

import me.leolin.shortcutbadger.ShortcutBadger;
import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;

public class Util {

	public static Display getDefaultDisplay(Activity activity) {
		return activity.getWindowManager().getDefaultDisplay();
	}

	public static Point getDisplaySize(Display display) {
		Point pt = new Point();
		DisplayMetrics metrics;
		if (display != null) {
			metrics = new DisplayMetrics();
			display.getMetrics(metrics);
		} else {
			metrics = Resources.getSystem().getDisplayMetrics();
		}
		pt.x = metrics.widthPixels;
		pt.y = metrics.heightPixels;
		return pt;
	}

	public static Point getDisplaySize(Activity activity) {
		return getDisplaySize(getDefaultDisplay(activity));
	}

	public static float getDisplayDensity(Activity activity) {
		return activity.getResources().getDisplayMetrics().density;
	}

	public static int getScreenOrientation(Activity activity) {
		try {
			DisplayMetrics metrics;
			Display display = activity.getWindowManager().getDefaultDisplay();
			if (display == null) {
				return 0;
			}
			metrics = new DisplayMetrics();
			display.getMetrics(metrics);
			int rotation = display.getRotation();
			int width = metrics.widthPixels;
			int height = metrics.heightPixels;
			if (((rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) && height > width) || ((rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) && width > height)) {
				switch(rotation) {
					case Surface.ROTATION_0:
						return 0;
					case Surface.ROTATION_90:
						return 90;
					case Surface.ROTATION_180:
						return 180;
					case Surface.ROTATION_270:
						return 270;
					default:
						break;
				}
			} else {
				switch(rotation) {
					case Surface.ROTATION_0:
						return 90;
					case Surface.ROTATION_90:
						return 0;
					case Surface.ROTATION_180:
						return 270;
					case Surface.ROTATION_270:
						return 180;
					default:
						break;
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return 0;
	}

	public static void setScreenOrientations(Activity activity, boolean flag0, boolean flag90, boolean flag180, boolean flag270) {

	}

	public static int getStatusBarHeight(Activity activity) {
		try {
			Class<?> c = Class.forName("com.android.internal.R$dimen");
			Object obj = c.newInstance();
			Field field = c.getField("status_bar_height");
			int x = Integer.parseInt(field.get(obj).toString());
			return activity.getResources().getDimensionPixelSize(x);
		} catch (Exception e) {
			e.printStackTrace();
			return 0;
		}
	}

	public static void setStatusBarStyle(Activity activity, int style) {
		try {
			Window window = activity.getWindow();
			if (style > 0) {
				window.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
			} else {
				window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static int getAndroidAlignment(int align) {
		int ret = 0;
		int horz = align & 3;
		if (horz == 1) {
			ret |= Gravity.LEFT;
		} else if (horz == 2) {
			ret |= Gravity.RIGHT;
		} else {
			ret |= Gravity.CENTER_HORIZONTAL;
		}
		int vert = align & 12;
		if (vert == 4) {
			ret |= Gravity.TOP;
		} else if (vert == 8) {
			ret |= Gravity.BOTTOM;
		} else {
			ret |= Gravity.CENTER_VERTICAL;
		}
		return ret;
	}

	public static int getSlibAlignment(int gravity) {
		int ret = 0;
		if ((gravity & Gravity.LEFT) != 0) {
			ret |= 1;
		} else if ((gravity & Gravity.RIGHT) != 0) {
			ret |= 2;
		}
		if ((gravity & Gravity.TOP) != 0) {
			ret |= 4;
		} else if ((gravity & Gravity.BOTTOM) != 0) {
			ret |= 8;
		}
		return ret;
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
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void openURL(final Activity activity, final String url) {
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
					activity.startActivity(intent);
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		});
	}

	public static void setBadgeNumber(final Activity activity, int count) {
		try {
			ShortcutBadger.applyCount(activity, count);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void grantPermissions(final Activity activity, final int permissions) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					grantPermissions(activity, permissions);
				}
			});
			return;
		}
		Vector<String> list = new Vector<String>();
		if ((permissions & 0x00000001) != 0) {
			list.add(Manifest.permission.CAMERA);
		}
		if ((permissions & 0x00000002) != 0) {
			list.add(Manifest.permission.RECORD_AUDIO);
		}
		if ((permissions & 0x00000004) != 0) {
			list.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
		}
		if (list.size() == 0) {
			return;
		}
		Permissions.grantPermissions(activity, list.toArray(new String[] {}), SlibActivity.REQUEST_PERMISSIONS);
	}


}