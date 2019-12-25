package slib.platform.android.alipay;

import android.app.Activity;

import com.alipay.sdk.app.PayTask;

import java.util.Map;

import slib.platform.android.Logger;

public class Alipay {

	public static void pay(final Activity activity, final String info) {
		try {
			Runnable payRunnable = new Runnable() {
				@Override
				public void run() {
					try {
						PayTask alipay = new PayTask(activity);
						Map<String, String> result = alipay.payV2(info, true);
						if (result != null) {
							nativeOnPayResult(result.get("result"));
						} else {
							nativeOnPayResult(null);
						}
					} catch (Exception e) {
						Logger.exception(e);
						nativeOnPayResult(null);
					}
				}
			};
			Thread payThread = new Thread(payRunnable);
			payThread.start();
		} catch (Exception e) {
			Logger.exception(e);
			nativeOnPayResult(null);
		}
	}

	private static native void nativeOnPayResult(String result);

}
