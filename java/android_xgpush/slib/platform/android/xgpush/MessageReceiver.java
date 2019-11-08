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

import android.content.Context;

import com.tencent.android.tpush.XGPushBaseReceiver;
import com.tencent.android.tpush.XGPushClickedResult;
import com.tencent.android.tpush.XGPushRegisterResult;
import com.tencent.android.tpush.XGPushShowedResult;
import com.tencent.android.tpush.XGPushTextMessage;

public class MessageReceiver extends XGPushBaseReceiver {

	@Override
	public void onRegisterResult(Context context, int i, XGPushRegisterResult xgPushRegisterResult) {
	}

	@Override
	public void onUnregisterResult(Context context, int i) {
	}

	@Override
	public void onSetTagResult(Context context, int i, String s) {
	}

	@Override
	public void onDeleteTagResult(Context context, int i, String s) {
	}

	@Override
	public void onTextMessage(Context context, XGPushTextMessage message) {
	}

	@Override
	public void onNotifactionClickedResult(Context context, XGPushClickedResult result) {
		XgPush.onReceiveMessage(true, result.getTitle(), result.getContent(), result.getCustomContent());
	}

	@Override
	public void onNotifactionShowedResult(Context context, XGPushShowedResult result) {
		XgPush.onReceiveMessage(false, result.getTitle(), result.getContent(), result.getCustomContent());
	}

}
