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

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import io.slib.R;

public class FileChooser {

	private Activity activity;
	private int requestCode;
	private FileChooserListener listener;

	private boolean flagLoadingContent;

	private String filePathImageCapture;
	private boolean flagIgnoreActivityResult;

	public FileChooser(Activity activity, int requestCode, FileChooserListener listener) {
		this.activity = activity;
		this.requestCode = requestCode;
		this.listener = listener;
	}

	public void setLoadingContent(boolean flag) {
		this.flagLoadingContent = flag;
	}

	void captureImage() {
		start(createImageCaptureIntent());
	}

	void chooseImage() {
		start(createChooseFileIntent("image/*", createImageCaptureIntent()));
	}

	void captureVideo() {
		start(createVideoCaptureIntent());
	}

	void chooseVideo() {
		start(createChooseFileIntent("video/*", createVideoCaptureIntent()));
	}

	void captureAudio() {
		start(createRecordSoundIntent());
	}

	void chooseAudio() {
		start(createChooseFileIntent("audio/*", createRecordSoundIntent()));
	}

	void chooseFile() {
		start(createChooseFileIntent());
	}

	public void processActivityResult(int resultCode, Intent intent) {
		if (resultCode != Activity.RESULT_OK && flagIgnoreActivityResult) {
			flagIgnoreActivityResult = false;
			return;
		}
		flagIgnoreActivityResult = false;
		Uri result = null;
		if (intent != null && resultCode == Activity.RESULT_OK) {
			result = intent.getData();
			if (result != null) {
				File cameraFile = new File(filePathImageCapture);
				if (cameraFile.exists()) {
					result = Uri.fromFile(cameraFile);
					activity.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, result));
				}
			}
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
		String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.US).format(new Date());
		filePathImageCapture = dirDCIM.getAbsolutePath() + File.separator + "IMG_" + timeStamp + ".jpg";
		File fileCapture = new File(filePathImageCapture);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(fileCapture));
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
		return createChooseFileIntent("*/*", createImageCaptureIntent(), createVideoCaptureIntent(), createRecordSoundIntent());
	}

	private Intent createChooserIntent(Intent extraIntent, Intent... initialIntents) {
		Intent chooser = new Intent(Intent.ACTION_CHOOSER);
		chooser.putExtra(Intent.EXTRA_INITIAL_INTENTS, initialIntents);
		chooser.putExtra(Intent.EXTRA_INTENT, extraIntent);
		chooser.putExtra(Intent.EXTRA_TITLE, activity.getResources().getString(R.string.slib_util_file_chooser_choose_file));
		return chooser;
	}

}
