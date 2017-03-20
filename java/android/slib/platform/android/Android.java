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
