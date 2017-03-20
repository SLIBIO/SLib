/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android.device;

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
