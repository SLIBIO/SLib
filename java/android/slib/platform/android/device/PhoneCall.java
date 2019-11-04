package slib.platform.android.device;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.telecom.Call;
import android.telecom.VideoProfile;

import java.util.HashMap;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class PhoneCall {

	public static void openDial(final Activity activity, final String phoneNumber) {
		try {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Intent intent;
					if (phoneNumber != null && phoneNumber.length() > 0) {
						intent = new Intent(Intent.ACTION_DIAL, Uri.fromParts("tel", phoneNumber, null));
					} else {
						intent = new Intent(Intent.ACTION_DIAL);
					}
					activity.startActivity(intent);
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void callPhone(final Activity activity, final String phoneNumber) {
		try {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					if( phoneNumber !=null && phoneNumber.length()>0) {
						Intent intent = new Intent(Intent.ACTION_CALL, Uri.fromParts("tel", phoneNumber, null));
						activity.startActivity(intent);
					}
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void answerCall(String callId) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			try {
				Call call = getCall(callId);
				if (call != null) {
					call.answer(VideoProfile.STATE_AUDIO_ONLY);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void endCall(String callId) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			try {
				Call call = getCall(callId);
				if (call != null) {
					call.disconnect();
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	private static native void nativeOnIncomingCall(String callId, String phoneNumber);

	public static void onIncomingCall(String callId, String phoneNumber) {
		nativeOnIncomingCall(callId, phoneNumber);
	}

	private static native void nativeOnOutgoingCall(String callId, String phoneNumber);

	public static void onOutgoingCall(String callId, String phoneNumber) {
		nativeOnOutgoingCall(callId, phoneNumber);
	}

	private static native void nativeOnEndCall(String callId, String phoneNumber);

	public static void onEndCall(String callId, String phoneNumber) {
		nativeOnEndCall(callId, phoneNumber);
	}

	public static String registerCall(Call call) {
		synchronized (mLock) {
			String callId = mCallsByCall.get(call);
			if (callId != null) {
				return callId;
			}
			callId = String.valueOf(mLastCallId);
			mLastCallId++;
			mCallsById.put(callId, call);
			mCallsByCall.put(call, callId);
			return callId;
		}
	}

	public static String unregisterCall(Call call) {
		synchronized (mLock) {
			String callId = mCallsByCall.remove(call);
			if (callId != null) {
				mCallsById.remove(callId);
			}
			return callId;
		}
	}

	public static Call unregisterCall(String callId) {
		synchronized (mLock) {
			Call call = mCallsById.remove(callId);
			if (call != null) {
				mCallsByCall.remove(call);
			}
			return call;
		}
	}

	public static String getCallId(Call call) {
		synchronized (mLock) {
			return mCallsByCall.get(call);
		}
	}

	public static Call getCall(String callId) {
		synchronized (mLock) {
			return mCallsById.get(callId);
		}
	}

	private static boolean mFlagInitIntentListener = false;
	public static void launchActivity(Context context, Class<?> activityClass, String type, String callId, String phoneNumber) {
		synchronized (mLock) {
			if (!mFlagInitIntentListener) {
				mFlagInitIntentListener = true;
				SlibActivity.addIntentListener(new SlibActivity.IntentListener() {
					@Override
					public void onIntent(Intent intent) {
						try {
							String category = intent.getStringExtra("category");
							if (category != null && category.equals("phone_call")) {
								String type = intent.getStringExtra("type");
								String id = intent.getStringExtra("id");
								String pn = intent.getStringExtra("pn");
								if (type != null && id != null && pn != null) {
									if (type.equals("in")) {
										onIncomingCall(id, pn);
									} else if (type.equals("out")) {
										onOutgoingCall(id, pn);
									} else if (type.equals("end")) {
										onEndCall(id, pn);
									}
								}
							}
						} catch (Exception e) {
							Logger.exception(e);
						}
					}
				});
			}
		}
		Intent intent = new Intent(context, activityClass);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.putExtra("category", "phone_call");
		intent.putExtra("type", type);
		intent.putExtra("id", callId);
		intent.putExtra("pn", phoneNumber);
		context.startActivity(intent);
	}

	private static final Object mLock = new Object();
	private static HashMap<String, Call> mCallsById = new HashMap<String, Call>();
	private static HashMap<Call, String> mCallsByCall = new HashMap<Call, String>();
	private static long mLastCallId = 0;

}
