package slib.platform.android.app;

import android.Manifest;
import android.app.Activity;
import android.app.role.RoleManager;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.telecom.TelecomManager;

import java.util.Vector;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;
import slib.platform.android.ui.UiThread;

public class Application {


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

	private static String getRole(int role) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
			switch (role) {
				case 0:
					return RoleManager.ROLE_HOME;
				case 1:
					return RoleManager.ROLE_BROWSER;
				case 2:
					return RoleManager.ROLE_DIALER;
				case 3:
					return RoleManager.ROLE_SMS;
				case 4:
					return RoleManager.ROLE_EMERGENCY;
				case 5:
					return RoleManager.ROLE_CALL_REDIRECTION;
				case 6:
					return RoleManager.ROLE_CALL_SCREENING;
				case 7:
					return RoleManager.ROLE_ASSISTANT;
			}
		}
		return null;
	}

	public static boolean isRoleHeld(final Activity activity, final int role) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
			try {
				RoleManager manager = (RoleManager) (activity.getSystemService(Context.ROLE_SERVICE));
				if (manager != null) {
					String strRole = getRole(role);
					if (strRole != null) {
						return manager.isRoleHeld(strRole);
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return false;
	}

	public static void requestRole(final Activity activity, final int role) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
			try {
				RoleManager manager = (RoleManager) (activity.getSystemService(Context.ROLE_SERVICE));
				if (manager != null) {
					String strRole = getRole(role);
					if (strRole == null) {
						return;
					}
					Intent intent = manager.createRequestRoleIntent(strRole);
					activity.startActivityForResult(intent, SlibActivity.REQUEST_ROLE);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	private static native void nativeOnCallbackRequestRole();

	public static void onRequestRoleResult(Activity activity) {
		nativeOnCallbackRequestRole();
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

}
