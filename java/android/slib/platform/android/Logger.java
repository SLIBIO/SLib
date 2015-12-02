package slib.platform.android;

import android.util.Log;

public class Logger {

	public static void exception(Throwable e) {
		Log.e("Exception", e.getMessage(), e);
	}
	
	public static void warning(String warning) {
		Log.w("Warning", warning);
	}
	
	public static void info(String info) {
		Log.d("INFO", info);
	}
	
	public static void error(String error) {
		Log.e("Error", error);
	}
	
}
