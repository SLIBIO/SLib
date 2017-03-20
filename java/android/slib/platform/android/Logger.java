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
