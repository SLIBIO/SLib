package slib.platform.android.device;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Point;
import android.os.Build;
import android.provider.Settings;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;

import com.google.android.gms.ads.identifier.AdvertisingIdClient;

import java.util.Vector;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;
import slib.platform.android.ui.UiThread;

public class Device {

	private static String[] getPermissions(int permissions) {

		Vector<String> list = new Vector<String>();

		if ((permissions & 1) != 0) {
			list.add(Manifest.permission.CAMERA);
		}

		if ((permissions & (1<<1)) != 0) {
			list.add(Manifest.permission.RECORD_AUDIO);
		}

		if ((permissions & (1<<2)) != 0) {
			list.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
		}
		if ((permissions & (1<<3)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
				list.add(Manifest.permission.READ_EXTERNAL_STORAGE);
			}
		}

		if ((permissions & (1<<4)) != 0) {
			list.add(Manifest.permission.READ_PHONE_STATE);
		}
		if ((permissions & (1<<5)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				list.add(Manifest.permission.READ_PHONE_NUMBERS);
			}
		}
		if ((permissions & (1<<6)) != 0) {
			list.add(Manifest.permission.CALL_PHONE);
		}
		if ((permissions & (1<<7)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				list.add(Manifest.permission.ANSWER_PHONE_CALLS);
			}
		}
		if ((permissions & (1<<8)) != 0) {
			list.add(Manifest.permission.ADD_VOICEMAIL);
		}
		if ((permissions & (1<<9)) != 0) {
			list.add(Manifest.permission.USE_SIP);
		}

		if ((permissions & (1<<10)) != 0) {
			list.add(Manifest.permission.SEND_SMS);
		}
		if ((permissions & (1<<11)) != 0) {
			list.add(Manifest.permission.RECEIVE_SMS);
		}
		if ((permissions & (1<<12)) != 0) {
			list.add(Manifest.permission.READ_SMS);
		}
		if ((permissions & (1<<13)) != 0) {
			list.add(Manifest.permission.RECEIVE_WAP_PUSH);
		}
		if ((permissions & (1<<14)) != 0) {
			list.add(Manifest.permission.RECEIVE_MMS);
		}

		if ((permissions & (1<<15)) != 0) {
			list.add(Manifest.permission.READ_CONTACTS);
		}
		if ((permissions & (1<<16)) != 0) {
			list.add(Manifest.permission.WRITE_CONTACTS);
		}
		if ((permissions & (1<<17)) != 0) {
			list.add(Manifest.permission.GET_ACCOUNTS);
		}

		if ((permissions & (1<<18)) != 0) {
			list.add(Manifest.permission.ACCESS_FINE_LOCATION);
		}
		if ((permissions & (1<<19)) != 0) {
			list.add(Manifest.permission.ACCESS_COARSE_LOCATION);
		}

		if ((permissions & (1<<20)) != 0) {
			list.add(Manifest.permission.READ_CALENDAR);
		}
		if ((permissions & (1<<21)) != 0) {
			list.add(Manifest.permission.WRITE_CALENDAR);
		}

		if ((permissions & (1<<22)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
				list.add(Manifest.permission.READ_CALL_LOG);
			}
		}
		if ((permissions & (1<<23)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
				list.add(Manifest.permission.WRITE_CALL_LOG);
			}
		}
		if ((permissions & (1<<24)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
				list.add(Manifest.permission.PROCESS_OUTGOING_CALLS);
			}
		}

		if ((permissions & (1<<25)) != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT_WATCH) {
				list.add(Manifest.permission.BODY_SENSORS);
			}
		}

		return list.toArray(new String[] {});
	}

	public static boolean checkPermissions(final Activity activity, final int permissions) {
		try {
			String[] list = getPermissions(permissions);
			if (list.length > 0) {
				return Permissions.checkPermissions(activity, list);
			} else {
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static void grantPermissions(final Activity activity, final int permissions) {
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					grantPermissions(activity, permissions);
				}
			});
			return;
		}
		try {
			String[] list = getPermissions(permissions);
			if (list.length > 0) {
				if (Permissions.grantPermissions(activity, list, SlibActivity.REQUEST_PERMISSIONS)) {
					return;
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		onRequestPermissionsResult();
	}

	private static native void nativeOnCallbackGrantPermissions();

	public static void onRequestPermissionsResult() {
		nativeOnCallbackGrantPermissions();
	}

	@SuppressLint("MissingPermission")
	public static String getIMEIs(Activity context) {
		try {
			TelephonyManager tm = (TelephonyManager)(context.getSystemService(Activity.TELEPHONY_SERVICE));
			if (tm != null) {
				String numbers = "";
				for (int i = 0; i < 128; i++) {
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
				SubscriptionManager sm = SubscriptionManager.from(context);
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

	public static String getDeviceId(Activity context) {
		try {
			AdvertisingIdClient.Info info = AdvertisingIdClient.getAdvertisingIdInfo(context);
			if (info != null) {
				String id = info.getId();
				if (id != null && id.length() > 0) {
					return id;
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		try {
			return Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
		} catch (Throwable e) {
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

	public static int getDevicePPI(Activity context) {
		DisplayMetrics dm = new DisplayMetrics();
		context.getWindowManager().getDefaultDisplay().getMetrics(dm);
		return dm.densityDpi;
	}

}
