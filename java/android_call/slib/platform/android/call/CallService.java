package slib.platform.android.call;

import android.annotation.TargetApi;
import android.app.Activity;
import android.os.Build;
import android.telecom.Call;
import android.telecom.InCallService;

import slib.platform.android.Logger;
import slib.platform.android.device.PhoneCall;

@TargetApi(Build.VERSION_CODES.M)
public class CallService extends InCallService {

	private Class<?> mActivityClassForeground = null;

	public void setForegroundActivityClass(Class<?> activity) {
		mActivityClassForeground = activity;
	}

	private Call.Callback callback = new Call.Callback() {
		@Override
		public void onStateChanged(Call call, int state) {
			super.onStateChanged(call, state);
			try {
				if (state == Call.STATE_DISCONNECTED) {
					removeCall(call);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	};

	@Override
	public void onCallAdded(Call call) {
		super.onCallAdded(call);
		try {
			int state = call.getState();
			if (state == Call.STATE_RINGING) {
				call.registerCallback(callback);
				String callId = PhoneCall.registerCall(call);
				if (mActivityClassForeground != null) {
					PhoneCall.launchActivity(this, mActivityClassForeground, "in", callId, getPhoneNumber(call));
				} else {
					PhoneCall.onIncomingCall(callId, getPhoneNumber(call));
				}
			} else if (state == Call.STATE_CONNECTING) {
				call.registerCallback(callback);
				String callId = PhoneCall.registerCall(call);
				if (mActivityClassForeground != null) {
					PhoneCall.launchActivity(this, mActivityClassForeground, "out", callId, getPhoneNumber(call));
				} else {
					PhoneCall.onOutgoingCall(callId, getPhoneNumber(call));
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@Override
	public void onCallRemoved(Call call) {
		super.onCallRemoved(call);
		try {
			removeCall(call);
			call.unregisterCallback(callback);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private void removeCall(Call call) {
		String callId = PhoneCall.unregisterCall(call);
		if (callId != null) {
			if (mActivityClassForeground != null) {
				PhoneCall.launchActivity(this, mActivityClassForeground, "end", callId, getPhoneNumber(call));
			} else {
				PhoneCall.onEndCall(callId, getPhoneNumber(call));
			}
		}
	}

	private static String getPhoneNumber(Call call) {
		Call.Details details = call.getDetails();
		return details.getHandle().getSchemeSpecificPart();
	}

}