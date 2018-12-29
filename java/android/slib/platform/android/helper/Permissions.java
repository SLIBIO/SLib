package slib.platform.android.helper;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Process;

import java.security.Permission;
import java.util.Vector;

import slib.platform.android.Logger;

public class Permissions {

	public static boolean requestPermissions(Activity activity, String[] permissions, int requestCode) {
		if (Build.VERSION.SDK_INT >= 23) {
			try {
				if (permissions.length == 0) {
					return false;
				}
				activity.requestPermissions(permissions, requestCode);
				return true;
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return false;
	}

	public static boolean requestPermission(Activity activity, String permission, int requestCode) {
		return requestPermissions(activity, new String[] {permission}, requestCode);
	}

	public static boolean checkPermission(Context context, String permission) {
		return context.checkPermission(permission, Process.myPid(), Process.myUid()) == PackageManager.PERMISSION_GRANTED;
	}

	public static boolean checkPermissions(Context context, String[] permissions) {
		for (int i = 0; i < permissions.length; i++) {
			if (!(checkPermission(context, permissions[i]))) {
				return false;
			}
		}
		return true;
	}

	public static boolean grantPermissions(Activity activity, String[] permissions, int requestCode) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			Vector<String> list = new Vector<String>();
			for (int i = 0; i < permissions.length; i++) {
				if (!(checkPermission(activity, permissions[i]))) {
					list.add(permissions[i]);
				}
			}
			if (list.size() == 0) {
				return false;
			}
			return requestPermissions(activity, list.toArray(new String[] {}), requestCode);
		} else {
			return false;
		}
	}

	public static boolean grantPermission(Activity activity, String permission, int requestCode) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			if (checkPermission(activity, permission)) {
				return false;
			} else {
				return requestPermission(activity, permission, requestCode);
			}
		} else {
			return false;
		}
	}

}
