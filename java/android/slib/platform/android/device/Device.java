package slib.platform.android.device;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.telecom.TelecomManager;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;

import java.io.File;
import java.util.UUID;
import java.util.Vector;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.FileHelper;
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
		nativeOnCallbackGrantPermissions();
	}

	private static native void nativeOnCallbackGrantPermissions();

	public static void onRequestPermissionsResult(Activity activity) {
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

	public static boolean isSupportedDefaultCallingApp() {
		return Build.VERSION.SDK_INT >= Build.VERSION_CODES.M;
	}

	public static boolean isDefaultCallingApp(Activity activity) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
				TelecomManager manger = (TelecomManager) (activity.getSystemService(Context.TELECOM_SERVICE));
				if (manger != null) {
					String current = manger.getDefaultDialerPackage();
					if (current != null && current.equals(activity.getPackageName())) {
						return true;
					}
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	private static native void nativeOnCallbackSetDefaultCallingApp();

	public static void onSetDefaultCallingAppResult(Activity activity) {
		nativeOnCallbackSetDefaultCallingApp();
	}

	private static boolean mFlagInitedSetDefaultCallingAppListener = false;

	public static void setDefaultCallingApp(final Activity activity) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
				if (!mFlagInitedSetDefaultCallingAppListener) {
					mFlagInitedSetDefaultCallingAppListener = true;
					SlibActivity.addActivityResultListener(SlibActivity.REQUEST_ACTIVITY_SET_DEFAULT_CALLING_APP, new SlibActivity.ActivityResultListener() {
						@Override
						public void onActivityResult(Activity activity, int requestCode, int resultCode, Intent data) {
							onSetDefaultCallingAppResult(activity);
						}
					});
				}
				activity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						final Intent intent = new Intent(TelecomManager.ACTION_CHANGE_DEFAULT_DIALER);
						intent.putExtra(TelecomManager.EXTRA_CHANGE_DEFAULT_DIALER_PACKAGE_NAME, activity.getPackageName());
						activity.startActivityForResult(intent, SlibActivity.REQUEST_ACTIVITY_SET_DEFAULT_CALLING_APP);
					}
				});
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void changeDefaultCallingApp(final Activity activity) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
				activity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						activity.startActivity(new Intent("android.settings.MANAGE_DEFAULT_APPS_SETTINGS"));
					}
				});
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

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

}
