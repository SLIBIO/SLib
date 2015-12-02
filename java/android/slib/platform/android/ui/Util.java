package slib.platform.android.ui;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Point;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import slib.platform.android.Logger;
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
	
	public static boolean isUiThread() {
		return Looper.myLooper() == Looper.getMainLooper();
	}
	
	private static native void nativeDispatchCallback();
	private static Runnable _dispatchRunnable = new Runnable() {
		public void run() {
			nativeDispatchCallback();
		}
	};
	public static void dispatch(SlibActivity activity) {
		activity.runOnUiThread(_dispatchRunnable);
	}
	
	private static native void nativeShowAlertResult(long ptr, int result);
	public static void showAlert(final SlibActivity activity, final Alert alert)
	{
		if (!isUiThread()) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					showAlert(activity, alert);
				}
			});
		} else {
			try {
				boolean flagRight = true;
				AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
				alertDialogBuilder.setTitle(alert.caption);
				alertDialogBuilder.setMessage(alert.text);
				if (alert.titleOk == null) {
					alert.titleOk = "OK";
				}
				if (alert.titleCancel == null) {
					alert.titleCancel = "Cancel";
				}
				if (alert.titleYes == null) {
					alert.titleYes = "Yes";
				}
				if (alert.titleNo == null) {
					alert.titleNo = "No";
				}
				switch (alert.type) {
				case Alert.TYPE_OK:
					alertDialogBuilder.setPositiveButton(alert.titleOk, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_OK);
						}
					});
					alertDialogBuilder.setCancelable(false);
					break;
				case Alert.TYPE_OKCANCEL:
					alertDialogBuilder.setPositiveButton(alert.titleOk, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_OK);
						}
					});
					alertDialogBuilder.setNegativeButton(alert.titleCancel, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				case Alert.TYPE_YESNO:
					alertDialogBuilder.setPositiveButton(alert.titleYes, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_YES);
						}
					});
					alertDialogBuilder.setNegativeButton(alert.titleNo, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_NO);
						}
					});
					break;
				case Alert.TYPE_YESNOCANCEL:
					alertDialogBuilder.setPositiveButton(alert.titleYes, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_YES);
						}
					});
					alertDialogBuilder.setNeutralButton(alert.titleNo, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_NO);
						}
					});
					alertDialogBuilder.setNegativeButton(alert.titleCancel, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowAlertResult(alert.nativeObject, Alert.RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				default:
					flagRight = false;
				}
				if (flagRight) {
					AlertDialog alertDialog = alertDialogBuilder.create();
					alertDialog.show();
					return;
				}
			} catch (Throwable e) {
				Logger.exception(e);
			}
			nativeShowAlertResult(alert.nativeObject, Alert.RESULT_FAIL);
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
}
