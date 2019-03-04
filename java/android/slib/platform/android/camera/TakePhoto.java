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

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Build;
import android.os.ParcelFileDescriptor;
import android.provider.MediaStore;

import java.io.InputStream;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.helper.FileChooser;
import slib.platform.android.ui.helper.FileChooserListener;

public class TakePhoto {

	public static void open(final Activity activity, final boolean flagCamera, final String outputFilePath) {
		if (!(UiThread.isUiThread())) {
			UiThread.post(new Runnable() {
				public void run() {
					open(activity, flagCamera, outputFilePath);
				}
			});
			return;
		}
		try {
			FileChooser chooser = new FileChooser(activity, SlibActivity.REQUEST_TAKE_PHOTO, new FileChooserListener() {
				@Override
				public void onChooseFile(Uri uri, Object content) {
					ParcelFileDescriptor pfd = null;
					try {
						if (uri != null) {
							String filePath = null;
							int fd = -1;
							int orientation = ExifInterface.ORIENTATION_UNDEFINED;
							int rotation = 0;

							String scheme = uri.getScheme();
							if (scheme != null && scheme.equals("file")) {
								filePath = uri.getPath();
								if (filePath != null) {
									ExifInterface exif = new ExifInterface(filePath);
									orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_UNDEFINED);
								}
							} else {
								if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
									InputStream stream = null;
									try {
										stream = activity.getContentResolver().openInputStream(uri);
										if (stream != null) {
											ExifInterface exif = new ExifInterface(stream);
											orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_UNDEFINED);
										}
									} catch (Exception e) {
										Logger.exception(e);
									}
									try {
										if (stream != null) {
											stream.close();
										}
									} catch (Exception e) {
										Logger.exception(e);
									}
								} else {
									Cursor cursor = null;
									try {
										cursor = activity.getContentResolver().query(uri, new String[] { MediaStore.Images.ImageColumns.ORIENTATION }, null, null, null);
										if (cursor != null) {
											if (cursor.getCount() == 1) {
												cursor.moveToFirst();
												rotation = cursor.getInt(0);
											}
										}
									} catch (Exception e) {
										Logger.exception(e);
									}
									try {
										if (cursor != null) {
											cursor.close();
										}
									} catch (Exception e) {
										Logger.exception(e);
									}
								}
								pfd = activity.getContentResolver().openFileDescriptor(uri, "r");
								if (pfd != null) {
									fd = pfd.getFd();
								}
							}
							if (filePath != null || pfd != null) {
								boolean flipHorz = false;
								boolean flipVert = false;
								switch (orientation) {
									case ExifInterface.ORIENTATION_ROTATE_90:
										rotation = 90;
										break;
									case ExifInterface.ORIENTATION_ROTATE_180:
										rotation = 180;
										break;
									case ExifInterface.ORIENTATION_ROTATE_270:
										rotation = 270;
										break;
									case ExifInterface.ORIENTATION_FLIP_HORIZONTAL:
										flipHorz = true;
										break;
									case ExifInterface.ORIENTATION_FLIP_VERTICAL:
										flipVert = true;
										break;
									case ExifInterface.ORIENTATION_TRANSPOSE:
										rotation = 90;
										flipHorz = true;
										break;
									case ExifInterface.ORIENTATION_TRANSVERSE:
										rotation = 270;
										flipHorz = true;
										break;
								}
								onSuccess(filePath, fd, rotation, flipHorz, flipVert);
								if (pfd != null) {
									pfd.close();
								}
								return;
							}
						}

					} catch (Exception e) {
						Logger.exception(e);
					}
					try {
						if (pfd != null) {
							pfd.close();
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
					onError();
				}
			});
			TakePhoto request = new TakePhoto();
			request.chooser = chooser;
			request.flagCamera = flagCamera;
			request.outputFilePath = outputFilePath;
			if (!flagCamera) {
				request.run();
				return;
			}
			if (!(Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA_ANY))) {
				onError();
				return;
			}
			lastRequest = request;
			if (grantPermission(activity)) {
				return;
			}
			request.run();
		} catch (Exception e) {
			Logger.exception(e);
			onError();
		}

	}

	private void run() {
		try {
			lastRequest = this;
			if (flagCamera) {
				if (!(chooser.captureImage(outputFilePath))) {
					onError();
				}
			} else {
				chooser.chooseImageFile();
			}
		} catch (Exception e) {
			Logger.exception(e);
			onError();
		}
	}

	private static native void navtiveOnComplete(String filePath, int fd, int rotation, boolean flipHorz, boolean flipVert, boolean flagCancel);

	private static void onSuccess(String filePath, int fd, int rotation, boolean flipHorz, boolean flipVert) {
		lastRequest = null;
		navtiveOnComplete(filePath, fd, rotation, flipHorz, flipVert, false);
	}

	private static void onError() {
		lastRequest = null;
		navtiveOnComplete(null, 0, 0, false, false, false);
	}

	private static void onCancel() {
		lastRequest = null;
		navtiveOnComplete(null, 0, 0, false, false, true);
	}

	private static boolean checkPermission(Context context) {
		return Permissions.checkPermission(context, Manifest.permission.CAMERA);
	}

	private static boolean grantPermission(Activity activity) {
		return Permissions.grantPermission(activity, Manifest.permission.CAMERA, SlibActivity.REQUEST_TAKE_PHOTO_PERMISSION);
	}

	public static void onRequestPermissionsResult(Activity activity) {
		if (!(checkPermission(activity))) {
			if (lastRequest != null) {
				onError();
			}
			return;
		}
		if (lastRequest != null) {
			lastRequest.run();
		}
	}

	public static void onResult(Activity activity, int resultCode, Intent data) {
		if (resultCode == SlibActivity.RESULT_OK) {
			if (lastRequest != null) {
				lastRequest.chooser.processActivityResult(resultCode, data);
			}
		} else if (resultCode == SlibActivity.RESULT_CANCELED) {
			if (lastRequest != null) {
				onCancel();
			}
		}
	}

	private static TakePhoto lastRequest;
	private FileChooser chooser;
	private boolean flagCamera;
	private String outputFilePath;

}