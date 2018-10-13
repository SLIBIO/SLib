package slib.platform.android.helper;

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.os.Process;

public class AppHelper {

	public static void requestPermissions(final Activity activity, final String[] permissions, final int requestCode) {
		if (Build.VERSION.SDK_INT >= 23) {
			activity.requestPermissions(permissions, requestCode);
		}
	}

	public static int checkSelfPermission(Context context, String permission) {
		if (permission == null) {
			throw new IllegalArgumentException("permission is null");
		}
		return context.checkPermission(permission, android.os.Process.myPid(), Process.myUid());
	}

}
