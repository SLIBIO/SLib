package slib.platform.android;

import java.io.InputStream;

import android.app.Activity;
import android.content.res.AssetManager;

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
	
	private static native void nativeOnCreateActivity(Activity activity);
	private static native void nativeOnResumeActivity(Activity activity);
	private static native void nativeOnPauseActivity(Activity activity);
	private static native void nativeOnDestroyActivity(Activity activity);
	private static native boolean nativeOnBack(Activity activity);
}
