package slib.platform.android.helper;

import android.content.Context;
import android.net.Uri;
import android.os.Build;
import android.support.v4.content.FileProvider;

import java.io.File;

public class FileHelper {

	public static Uri getUriForFile(Context context, File file) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			Uri uri = null;
			try {
				uri = FileProvider.getUriForFile(context, context.getPackageName() + ".slib.filechooser", file);
			} catch (Exception e) {
			}
			if (uri == null) {
				try {
					uri = FileProvider.getUriForFile(context, context.getPackageName() + ".android.fileprovider", file);
				} catch (Exception e) {
				}
			}
			return uri;
		} else {
			return Uri.fromFile(file);
		}
	}

}
