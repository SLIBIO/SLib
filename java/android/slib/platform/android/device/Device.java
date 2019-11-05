package slib.platform.android.device;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Point;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Vibrator;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;

import java.io.File;
import java.util.UUID;

import slib.platform.android.Logger;
import slib.platform.android.helper.FileHelper;
import slib.platform.android.ui.UiThread;

public class Device {

	public static int getAudioMode(Activity context) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return am.getMode();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return 0;
	}

	public static void setAudioMode(Activity context, int mode) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				am.setMode(mode);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static int getRingerMode(Activity context) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return am.getRingerMode();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return 0;
	}

	public static void setRingerMode(Activity context, int mode) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				am.setRingerMode(mode);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static float getVolume(Activity context, int stream) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return (float)(am.getStreamVolume(stream)) / (float)(am.getStreamMaxVolume(stream));
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return 0;
	}

	public static void setVolume(Activity context, float volume, int stream, int flags) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				int max = am.getStreamMaxVolume(stream);
				int vol = (int)(max * volume);
				am.setStreamVolume(stream, vol, flags);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static boolean isMicrophoneMute(Activity context) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return am.isMicrophoneMute();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static void setMicrophoneMute(Activity context, boolean flag) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				am.setMicrophoneMute(flag);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static boolean isSpeakerOn(Activity context) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return am.isSpeakerphoneOn();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static void setSpeakerOn(Activity context, boolean flag) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				am.setSpeakerphoneOn(flag);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static boolean isBluetoothScoOn(Activity context) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				return am.isBluetoothScoOn();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static void setBluetoothScoOn(Activity context, boolean flag) {
		try {
			AudioManager am = (AudioManager)(context.getSystemService(Context.AUDIO_SERVICE));
			if (am != null) {
				am.setBluetoothScoOn(flag);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@SuppressLint("MissingPermission")
	public static void vibrate(Activity context, int durationMillis) {
		try {
			Vibrator v = (Vibrator)(context.getSystemService(Context.VIBRATOR_SERVICE));
			if (v != null) {
				if (durationMillis > 0) {
					v.vibrate(durationMillis);
				} else {
					v.cancel();
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}

	private static final int MAX_SIM_SLOT_COUNT = 8;

	@SuppressLint("MissingPermission")
	public static String getIMEIs(Activity context) {
		try {
			TelephonyManager tm = (TelephonyManager)(context.getSystemService(Activity.TELEPHONY_SERVICE));
			if (tm != null) {
				String numbers = "";
				for (int i = 0; i < MAX_SIM_SLOT_COUNT; i++) {
					String number = null;
					try {
						if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
							number = tm.getImei(i);
						} else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
							number = tm.getDeviceId(i);
						}
					} catch (Exception e) {
					}
					if (number != null && number.length() > 0) {
						if (numbers.length() > 0) {
							numbers += ";";
						}
						numbers += number;
					} else {
						break;
					}
				}
				if (numbers.isEmpty()) {
					return tm.getDeviceId();
				}
				return numbers;
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}

	@SuppressLint("MissingPermission")
	public static String getPhoneNumbers(Activity context) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP_MR1) {
			try {
				String numbers = "";
				SubscriptionManager sm = (SubscriptionManager)(context.getSystemService(Context.TELEPHONY_SUBSCRIPTION_SERVICE));
				if (sm != null) {
					for (SubscriptionInfo info : sm.getActiveSubscriptionInfoList()) {
						if (numbers.length() > 0) {
							numbers += ";";
						}
						numbers += info.getNumber();
					}
				}
				if (numbers.length() > 0) {
					return numbers;
				}
			} catch (Throwable e) {
				Logger.exception(e);
			}
		}
		try {
			TelephonyManager tm = (TelephonyManager) (context.getSystemService(Activity.TELEPHONY_SERVICE));
			if (tm != null) {
				return tm.getLine1Number();
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}

	@SuppressLint("MissingPermission")
	public static int getSimSlotsCount(Activity context) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
				TelephonyManager tm = (TelephonyManager) (context.getSystemService(Activity.TELEPHONY_SERVICE));
				if (tm != null) {
					return tm.getPhoneCount();
				}
			} else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP_MR1) {
				SubscriptionManager sm = (SubscriptionManager)(context.getSystemService(Context.TELEPHONY_SUBSCRIPTION_SERVICE));
				if (sm != null) {
					int n = 0;
					for (int i = 0; i < MAX_SIM_SLOT_COUNT; i++) {
						try {
							SubscriptionInfo info = sm.getActiveSubscriptionInfoForSimSlotIndex(i);
							if (info != null) {
								n = i + 1;
							}
						} catch (Exception e) {
							Logger.exception(e);
						}
					}
					return n;
				}
			} else {
				return 1;
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return 0;
	}

	@SuppressLint("MissingPermission")
	public static String getPhoneNumber(Activity context, int slot) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP_MR1) {
				SubscriptionManager sm = (SubscriptionManager)(context.getSystemService(Context.TELEPHONY_SUBSCRIPTION_SERVICE));
				if (sm != null) {
					SubscriptionInfo info = sm.getActiveSubscriptionInfoForSimSlotIndex(slot);
					if (info != null) {
						return info.getNumber();
					}
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static String getDeviceId(Activity context) {
		try {
			final String KEY_DEVICEID = "DeviceId";
			SharedPreferences prefs = context.getSharedPreferences("Slib.device_id_prefs", 0);
			String value = prefs.getString(KEY_DEVICEID, null);
			if (value == null) {
				value = UUID.randomUUID().toString();
				prefs.edit().putString(KEY_DEVICEID, value).apply();
			}
			return value;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static String getDeviceOSVersion() {
		return Build.VERSION.RELEASE;
	}

	public static String getDeviceName() {
		String manufacturer = Build.MANUFACTURER;
		String model = Build.MODEL;
		if (model.startsWith(manufacturer)) {
			return model;
		} else {
			return manufacturer + " " + model;
		}
	}

	public static Point getScreenSize(Activity context) {
		DisplayMetrics dm = new DisplayMetrics();
		context.getWindowManager().getDefaultDisplay().getMetrics(dm);
		return new Point(dm.widthPixels, dm.heightPixels);
	}

	public static int getScreenPPI(Activity context) {
		DisplayMetrics dm = new DisplayMetrics();
		context.getWindowManager().getDefaultDisplay().getMetrics(dm);
		return dm.densityDpi;
	}

	public static void openURL(final Activity activity, final String url) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					openURL(activity, url);
				}
			});
			return;
		}
		try {
			if (url.startsWith("file://")) {
				File file = new File(url.substring(7));
				Uri uri = FileHelper.getUriForFile(activity, file);
				if (uri == null) {
					Logger.error("File exposed beyond app: " + file.getAbsolutePath());
					return;
				}
				Intent intent;
				if (url.endsWith("jpg") || url.endsWith("jpeg") || url.endsWith("png")) {
					intent = new Intent(Intent.ACTION_VIEW);
					intent.setDataAndType(uri, "image/*");
				} else {
					intent = new Intent(Intent.ACTION_VIEW, uri);
				}
				intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
				activity.startActivity(intent);
			} else {
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				activity.startActivity(intent);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

}
