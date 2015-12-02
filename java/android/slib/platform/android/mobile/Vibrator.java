package slib.platform.android.mobile;

import slib.platform.android.Logger;
import android.app.Activity;
import android.content.Context;

public class Vibrator {
	/*
		this class need permission "android.permission.VIBRATE"
	 */
	public static boolean vibrate(Activity context, int millisec) {
		try {
			android.os.Vibrator v = (android.os.Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
			if (v != null) {
				v.vibrate(millisec);
				return true;
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return false;
	}
	
	public static boolean cancel(Activity context)
	{
		try {
			android.os.Vibrator v = (android.os.Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
			if (v != null) {
				v.cancel();
				return true;
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return false;
	}
}
