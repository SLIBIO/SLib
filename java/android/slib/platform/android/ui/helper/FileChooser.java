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

package slib.platform.android.ui.helper;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v4.content.FileProvider;
import android.widget.Toast;

import java.io.File;
import java.util.Vector;

import io.slib.R;
import slib.platform.android.Logger;
import slib.platform.android.helper.Permissions;
import slib.util.Time;

public class FileChooser {

	private Activity activity;
	private int requestCode;
	private FileChooserListener listener;

	private String filePathImageCapture;
	private boolean flagIgnoreActivityResult;

	public FileChooser(Activity activity, int requestCode, FileChooserListener listener) {
		this.activity = activity;
		this.requestCode = requestCode;
		this.listener = listener;
	}

	public void captureImage() {
		filePathImageCapture = null;
		start(createImageCaptureIntent());
	}

	public void chooseImage() {
		filePathImageCapture = null;
		if (Permissions.checkPermission(activity, Manifest.permission.CAMERA) && Permissions.checkPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
			start(createChooseFileIntent("image/*", createImageCaptureIntent()));
		} else {
			start(createChooseFileIntent("image/*"));
		}
	}

	public void captureVideo() {
		filePathImageCapture = null;
		start(createVideoCaptureIntent());
	}

	public void chooseVideo() {
		filePathImageCapture = null;
		if (Permissions.checkPermission(activity, Manifest.permission.CAMERA)) {
			start(createChooseFileIntent("video/*", createVideoCaptureIntent()));
		} else {
			start(createChooseFileIntent("video/*"));
		}
	}

	public void captureAudio() {
		filePathImageCapture = null;
		start(createRecordSoundIntent());
	}

	public void chooseAudio() {
		filePathImageCapture = null;
		if (Permissions.checkPermission(activity, Manifest.permission.RECORD_AUDIO)) {
			start(createChooseFileIntent("audio/*", createRecordSoundIntent()));
		} else {
			start(createChooseFileIntent("audio/*"));
		}
	}

	public void chooseFile() {
		filePathImageCapture = null;
		start(createChooseFileIntent());
	}

	public void processActivityResult(int resultCode, Intent intent) {
		if (resultCode != Activity.RESULT_OK && flagIgnoreActivityResult) {
			flagIgnoreActivityResult = false;
			return;
		}
		flagIgnoreActivityResult = false;
		Uri result = null;
		try {
			if (resultCode == Activity.RESULT_OK) {
				if (filePathImageCapture != null) {
					File cameraFile = new File(filePathImageCapture);
					if (cameraFile.exists()) {
						result = Uri.fromFile(cameraFile);
						activity.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, result));
					}
				}
				if (result == null && intent != null) {
					result = intent.getData();
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		listener.onChooseFile(result, null);
	}

	private void start(Intent intent) {
		try {
			flagIgnoreActivityResult = false;
			activity.startActivityForResult(intent, requestCode);
		} catch (ActivityNotFoundException e1) {
			try {
				flagIgnoreActivityResult = true;
				activity.startActivityForResult(createChooseFileIntent(), requestCode);
			} catch (ActivityNotFoundException e2) {
				Toast.makeText(activity, R.string.slib_util_file_chooser_activity_not_found, Toast.LENGTH_LONG).show();
			}
		}
	}

	private Intent createImageCaptureIntent() {
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		File dirDCIM = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);
		String fileName = new Time().format("IMG_%Y%M%D_%H%I%S_%n.jpg");
		filePathImageCapture = dirDCIM.getAbsolutePath() + File.separator + fileName;
		File fileCapture = new File(filePathImageCapture);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			Uri photoURI = FileProvider.getUriForFile(activity, activity.getPackageName() + ".slib.filechooser", fileCapture);
			intent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
		} else {
			intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(fileCapture));
		}
		return intent;
	}

	private Intent createVideoCaptureIntent() {
		return new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
	}

	private Intent createRecordSoundIntent() {
		return new Intent(MediaStore.Audio.Media.RECORD_SOUND_ACTION);
	}

	private Intent createChooseFileIntent(String type, Intent... initialIntents) {
		Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType(type);
		return createChooserIntent(intent, initialIntents);
	}

	private Intent createChooseFileIntent() {
		Vector<Intent> intents = new Vector<Intent>();
		if (Permissions.checkPermission(activity, Manifest.permission.CAMERA)) {
			if (Permissions.checkPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
				intents.add(createImageCaptureIntent());
			}
			intents.add(createVideoCaptureIntent());
		}
		if (Permissions.checkPermission(activity, Manifest.permission.RECORD_AUDIO)) {
			intents.add(createRecordSoundIntent());
		}
		return createChooseFileIntent("*/*", intents.toArray(new Intent[] {}));
	}

	private Intent createChooserIntent(Intent extraIntent, Intent... initialIntents) {
		if (initialIntents.length == 0) {
			return extraIntent;
		}
		Intent chooser = new Intent(Intent.ACTION_CHOOSER);
		chooser.putExtra(Intent.EXTRA_INITIAL_INTENTS, initialIntents);
		chooser.putExtra(Intent.EXTRA_INTENT, extraIntent);
		chooser.putExtra(Intent.EXTRA_TITLE, activity.getResources().getString(R.string.slib_util_file_chooser_choose_file));
		return chooser;
	}

}
