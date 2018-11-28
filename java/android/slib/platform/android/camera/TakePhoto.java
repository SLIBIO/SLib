/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

package slib.platform.android.camera;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.ui.UiThread;

public class TakePhoto {

	static Uri currentPhotoPath = null;

	public static void takePhoto(final Activity context, final String outputFilePath) {

		if (!(UiThread.isUiThread())) {
			UiThread.post(new Runnable() {
				public void run() {
					takePhoto(context, outputFilePath);
				}
			});
			return;
		}

		try {

			if (outputFilePath == null || outputFilePath.isEmpty()) {

				String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath();
				String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
				String fileName = "Photo_" + timeStamp;
				File file = File.createTempFile(
						fileName,
						".jpg",
						new File(path)
				);
				file.deleteOnExit();
				currentPhotoPath = Uri.fromFile(file);
			} else {
				String path;
				if (outputFilePath.charAt(0) == '/') {
					path = outputFilePath;
				} else {
					path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath() + "/" + outputFilePath;
				}
				int index = path.lastIndexOf('/');
				if (index < 0) {
					onFailure();
					return;
				}
				String dirPath = path.substring(0, index);
				File dir = new File(dirPath);
				if (!dir.exists() || !dir.isDirectory()) {
					onFailure();
					return;
				}
				currentPhotoPath = Uri.fromFile(new File(path));
			}

			Intent takePhotoIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			if (takePhotoIntent.resolveActivity(context.getPackageManager()) != null) {
				takePhotoIntent.putExtra(MediaStore.EXTRA_OUTPUT, currentPhotoPath);
				context.startActivityForResult(takePhotoIntent, SlibActivity.REQUEST_IMAGE_CAPTURE);
			} else {
				onFailure();
			}

		} catch (Exception e) {
			Logger.exception(e);
			onFailure();
		}
	}

	public static void onResult(Activity activity, int resultCode, Intent data) {
		if (resultCode == SlibActivity.RESULT_OK && currentPhotoPath != null) {
			final String path = currentPhotoPath.getPath();
			new Thread(new Runnable() {
				public void run() {
					try {
						int orientation = 0;
						int i = 0;
						for (i = 0; i < 500; i++) {
							if (new File(path).exists()) {
								break;
							}
							try {
								Thread.sleep(10);
							} catch (Exception e) {
							}
						}
						if (i == 300) {
							onFailure();
							return;
						}
						onSuccess(path);
					} catch (Exception e) {
						onFailure();
						Logger.exception(e);
					}
				}
			}).start();
		} else if (resultCode == SlibActivity.RESULT_CANCELED) {
			onCanceled();
		}
	}

	private static native void onSuccess(String path);
	private static native void onFailure();
	private static native void onCanceled();

}