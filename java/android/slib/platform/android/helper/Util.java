package slib.platform.android.helper;

import android.os.Environment;

public class Util {

	public static String getPicturesDirectory() {
		return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getAbsolutePath();
	}

}
