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

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.Surface;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import java.lang.reflect.Field;

import me.leolin.shortcutbadger.ShortcutBadger;
import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

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

	public static void setScreenOrientations(final Activity activity, final boolean flag0, final boolean flag90, final boolean flag180, final boolean flag270) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					setScreenOrientations(activity, flag0, flag90, flag180, flag270);
				}
			});
			return;
		}
		try {
			int orientation;
			if (flag90 || flag270) {
				if (flag0 || flag180) {
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
						orientation = ActivityInfo.SCREEN_ORIENTATION_FULL_USER;
					} else {
						orientation = ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR;
					}
				} else if (flag90 && flag270) {
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
						orientation = ActivityInfo.SCREEN_ORIENTATION_USER_LANDSCAPE;
					} else {
						orientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
					}
				} else if (flag270) {
					orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
				} else {
					orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
				}
			} else {
				if (flag0 && flag180) {
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
						orientation = ActivityInfo.SCREEN_ORIENTATION_USER_PORTRAIT;
					} else {
						orientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;
					}
				} else if (flag180) {
					orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
				} else {
					orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
				}
			}
			activity.setRequestedOrientation(orientation);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static Rect getSafeAreaInsets(Activity activity) {
		if (activity instanceof SlibActivity) {
			Rect rect = ((SlibActivity)activity).getLatestInsets();
			if (rect != null) {
				return rect;
			}
		}
		Rect rect = new Rect(0, 0, 0, 0);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			try {
				Window window = activity.getWindow();
				if ((window.getAttributes().flags & WindowManager.LayoutParams.FLAG_FULLSCREEN) == 0) {
					if ((window.getAttributes().flags & WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS) != 0 || (window.getDecorView().getSystemUiVisibility() & View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN) != 0) {
						rect.top = getStatusBarHeight(activity);
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return rect;
	}

	public static int getNavigationBarHeight(Activity activity) {
		try {
			Class<?> c = Class.forName("com.android.internal.R$dimen");
			Object obj = c.newInstance();
			Field field = c.getField("navigation_bar_height");
			int x = Integer.parseInt(field.get(obj).toString());
			return activity.getResources().getDimensionPixelSize(x);
		} catch (Exception e) {
			e.printStackTrace();
			return 0;
		}
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

	public static void setStatusBarStyle(final Activity activity, final int style) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					setStatusBarStyle(activity, style);
				}
			});
			return;
		}
		try {
			Window window = activity.getWindow();
			if (style > 0) {
				window.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
					window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
					window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
					int flagSystemUI = View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
					if (style == 1) {
						flagSystemUI |= View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
					}
					window.getDecorView().setSystemUiVisibility(flagSystemUI);
					window.setStatusBarColor(Color.TRANSPARENT);
				} else {
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
						window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
					}
				}
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

}