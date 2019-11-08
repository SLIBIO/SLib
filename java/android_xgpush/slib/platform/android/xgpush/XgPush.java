/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

package slib.platform.android.xgpush;

import android.app.Activity;

import com.tencent.android.tpush.XGIOperateCallback;
import com.tencent.android.tpush.XGPushConfig;
import com.tencent.android.tpush.XGPushManager;

public class XgPush {

	public static void start(Activity activity) {
		XGPushConfig.getToken(activity);
		XGPushManager.registerPush(activity, new XGIOperateCallback() {
			@Override
			public void onSuccess(Object token, int i) {
				onDeviceToken(token.toString());
			}

			@Override
			public void onFail(Object o, int i, String s) {
				onDeviceToken(null);
			}
		});
	}

	public static void stop(Activity activity) {
		XGPushManager.unregisterPush(activity);
	}

	public static void setEnableDebug(Activity activity, boolean flag) {
		XGPushConfig.enableDebug(activity, flag);
	}

	private static native void nativeOnDeviceToken(String token);

	public static void onDeviceToken(String token) {
		nativeOnDeviceToken(token);
	}

	private static native void nativeOnReceiveMessage(boolean flagClicked, String title, String content, String data);

	public static void onReceiveMessage(boolean flagClicked, String title, String content, String data) {
		nativeOnReceiveMessage(flagClicked, title, content, data);
	}

}
