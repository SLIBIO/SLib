package slib.platform.android.ui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.net.Uri;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

import slib.platform.android.SlibActivity;

public class Util {

	public static Display getDefaultDisplay(SlibActivity activity) {
		return activity.getWindowManager().getDefaultDisplay();
	}
	
	public static Display getDefaultDisplay(Activity activity) {
		return activity.getWindowManager().getDefaultDisplay();
	}
	
	public static Point getDisplaySize(Display display) {
		Point pt = new Point();
		DisplayMetrics metrics = new DisplayMetrics();
		display.getMetrics(metrics);
		pt.x = metrics.widthPixels;
		pt.y = metrics.heightPixels;
		/*
		try {
		    // used when 17 > SDK_INT >= 14; includes window decorations (statusbar bar/menu bar)
		    pt.x = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
		    pt.y = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
		} catch (Exception ignored) {
		}
		
		try {
		    // used when SDK_INT >= 17; includes window decorations (statusbar bar/menu bar)
		    Point realSize = new Point();
		    Display.class.getMethod("getRealSize", Point.class).invoke(display, realSize);
		    pt.x = realSize.x;
		    pt.y = realSize.y;
		} catch (Exception ignored) {
		}*/
		return pt;
	}
	
	public static float getDisplayDensity(Activity activity) {
		return activity.getResources().getDisplayMetrics().density;
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

	public static void dismissKeyboard(Activity activity) {
		InputMethodManager imm = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		if (imm != null) {
			View view = activity.getCurrentFocus();
			if (view != null) {
				imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
			}
		}
	}

	public static void openURL(final Activity activity, final String url) {
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				activity.startActivity(intent);
			}
		});
	}
}
