package slib.platform.android.device;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;

import java.io.File;
import java.util.UUID;

import slib.platform.android.Logger;
import slib.platform.android.helper.FileHelper;

public class Device {

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
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
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
		});
	}

}
