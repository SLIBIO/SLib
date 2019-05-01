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
import android.content.pm.PackageManager;
import android.os.Build;
import android.view.Surface;

import java.nio.ByteBuffer;
import java.util.Vector;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;

public abstract class SCamera {

	private static boolean supportsCamera2 = true;
	private static boolean mCheckSupportsCamera2 = false;

	public static final int FLASH_MODE_AUTO = 0;
	public static final int FLASH_MODE_ON = 1;
	public static final int FLASH_MODE_OFF = 2;

	public static final int FOCUS_MODE_NONE = 0;
	public static final int FOCUS_MODE_CONTINUOUS_AUTO_FOCUS = 1;
	public static final int FOCUS_MODE_SMOOTH_CONTINUOUS_AUTO_FOCUS = 2;

	public static final int TORCH_MODE_AUTO = 0;
	public static final int TORCH_MODE_ON = 1;
	public static final int TORCH_MODE_OFF = 2;


	public static SCameraInfo[] getCamerasList(Activity activity) {
		try {
			checkSupportsCamera2(activity);
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
				return SCamera2.getCameras(activity).toArray(new SCameraInfo[]{});
			} else {
				return SCamera1.getCameras().toArray(new SCameraInfo[]{});
			}
		} catch (Exception e) {
			Logger.exception(e);
			return new SCameraInfo[]{};
		}
	}

	private static void checkSupportsCamera2(Activity activity) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
			if (!mCheckSupportsCamera2) {
				supportsCamera2 = !(SCamera2.isLegacyDevice(activity));
				mCheckSupportsCamera2 = true;
			}
		}
	}

	public static SCamera create(Activity activity, String id, long nativeObject) {
		try {
			if (activity == null) {
				return null;
			}
			if (!(hasFeature(activity))) {
				return null;
			}
			checkSupportsCamera2(activity);
			initialize();
			SCamera camera;
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
				camera = new SCamera2();
			} else {
				camera = new SCamera1();
			}
			camera.mActivity = activity;
			camera.mId = id;
			camera.mNativeObject = nativeObject;
			if (!(camera.initCamera())) {
				return null;
			}
			synchronized (mSync) {
				if (grantPermission(activity)) {
					camera.mFlagWaitingPermission = true;
					mFlagRequestPermission = true;
					mCameras.add(camera);
					return camera;
				}
				camera.openCamera();
				mCameras.add(camera);
			}
			return camera;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public void release() {
		synchronized (mSync) {
			mCameras.remove(this);
			stop();
			try {
				releaseCamera();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public void setSettings(int width, int height) {
		mSettingsWidth = width;
		mSettingsHeight = height;
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return;
			}
			try {
				configureCamera();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public void start() {
		synchronized (mSync) {
			mFlagRunningRequest = true;
			if (!(isOpenedCamera())) {
				return;
			}
			if (mFlagRunning) {
				return;
			}
			try {
				mFlagRunning = startCamera();
				if (mFlagRunning) {
					log("Started");
				}
			} catch (Exception e) {
				Logger.exception(e);
				mFlagRunningRequest = false;
			}
		}
	}

	public void stop() {
		synchronized (mSync) {
			mFlagRunningRequest = false;
			if (!(isOpenedCamera())) {
				return;
			}
			if (!mFlagRunning) {
				return;
			}
			mFlagRunning = false;
			try {
				stopCamera();
				log("Stopped");
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public boolean isRunning() {
		return isOpenedCamera() && mFlagRunning;
	}

	public void takePicture(int flashMode) {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				nativeOnPicture(mNativeObject, null, 0, 0);
				return;
			}
			try {
				takePictureCamera(flashMode);
			} catch (Exception e) {
				Logger.exception(e);
				nativeOnPicture(mNativeObject, null, 0, 0);
			}
		}
	}

	public void setFocusMode(int mode) {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return;
			}
			try {
				setFocusModeCamera(mode);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public void autoFocus() {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return;
			}
			try {
				autoFocusCamera();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public void autoFocusOnPoint(float x, float y) {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return;
			}
			try {
				autoFocusOnPointCamera(x, y);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public boolean isTorchActive() {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return false;
			}
			try {
				return isTorchActiveCamera();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return false;
	}

	public void setTorchMode(int mode, float level) {
		synchronized (mSync) {
			if (!(isOpenedCamera())) {
				return;
			}
			try {
				setTorchModeCamera(mode, level);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static boolean isMobileDeviceTorchActive(Activity activity) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
			return SCamera2.isMobileDeviceTorchActive2(activity);
		} else {
			return SCamera1.isMobileDeviceTorchActive1();
		}
	}

	public static void setMobileDeviceTorchMode(Activity activity, int mode, float level) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
			SCamera2.setMobileDeviceTorchMode2(activity, mode, level);
		} else {
			SCamera1.setMobileDeviceTorchMode1(mode, level);
		}
	}

	private static native void nativeOnFrame(long nativeObject, byte[] data, int width, int height, int orientation, int flip);

	void onFrame(byte[] data, int width, int height) {
		int flip;
		int orientation = (getCameraOrientation() - getDisplayOrientation() + 360) % 360;
		if (isFrontCamera()) {
			flip = 1; // Horizontal
			orientation = (orientation + 180) % 360;
		} else {
			flip = 0;
		}
		nativeOnFrame(mNativeObject, data, width, height, orientation, flip);
	}

	private static native void nativeOnFrame2(long nativeObject, int width, int height, ByteBuffer Y, ByteBuffer U, ByteBuffer V, int rowStrideY, int rowStrideUV, int pixelStrideUV, int orientation, int flip);

	void onFrame2(int width, int height, ByteBuffer Y, ByteBuffer U, ByteBuffer V, int rowStrideY, int rowStrideUV, int pixelStrideUV) {
		int flip;
		int orientation = (getCameraOrientation() - getDisplayOrientation() + 360) % 360;
		if (isFrontCamera()) {
			flip = 1; // Horizontal
			orientation = (orientation + 180) % 360;
		} else {
			flip = 0;
		}
		nativeOnFrame2(mNativeObject, width, height, Y, U, V, rowStrideY, rowStrideUV, pixelStrideUV, orientation, flip);
	}

	private static native void nativeOnPicture(long nativeObject, byte[] data, int orientation, int flip);

	void onPicture(byte[] data) {
		int flip;
		int orientation;
		if (isFrontCamera()) {
			flip = 1; // Horizontal
			orientation = (getCameraOrientation() + getDisplayOrientation()) % 360;
		} else {
			flip = 0;
			orientation = (getCameraOrientation() - getDisplayOrientation() + 360) % 360;
		}
		nativeOnPicture(mNativeObject, data, orientation, flip);
	}

	private static native void nativeOnPicture2(long nativeObject, ByteBuffer data, int orientation, int flip);

	void onPicture2(ByteBuffer data) {
		int flip;
		int orientation;
		if (isFrontCamera()) {
			flip = 1; // Horizontal
			orientation = (getCameraOrientation() + getDisplayOrientation()) % 360;
		} else {
			flip = 0;
			orientation = (getCameraOrientation() - getDisplayOrientation() + 360) % 360;
		}
		nativeOnPicture2(mNativeObject, data, orientation, flip);
	}

	public static boolean hasFeature(Activity activity) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
			return Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA_ANY);
		} else {
			return Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA);
		}
	}

	public static boolean grantPermission(Activity activity) {
		return Permissions.grantPermission(activity, Manifest.permission.CAMERA, SlibActivity.REQUEST_PERMISSION_SCAMERA);
	}

	public static boolean checkPermission(Context context) {
		return Permissions.checkPermission(context, Manifest.permission.CAMERA);
	}


	private int getDisplayOrientation() {
		int rotation = mActivity.getWindowManager().getDefaultDisplay().getRotation();
		switch (rotation) {
			case Surface.ROTATION_0:
				return 0;
			case Surface.ROTATION_90:
				return 90;
			case Surface.ROTATION_180:
				return 180;
			case Surface.ROTATION_270:
				return 270;
		}
		return 0;
	}

	void reopen() throws Exception {
		synchronized (mSync) {
			openCamera();
			if (mFlagRunningRequest) {
				start();
			}
		}
	}


	static final Object mSync = new Object();
	static Vector<SCamera> mCameras = new Vector<SCamera>();
	private static boolean mFlagRequestPermission = false;

	Activity mActivity;
	String mId;
	private long mNativeObject = 0;

	int mSettingsWidth = 0;
	int mSettingsHeight = 0;

	boolean mFlagRunningRequest = false;
	boolean mFlagRunning = false;
	private boolean mFlagWaitingPermission = false;

	protected abstract boolean initCamera() throws Exception;
	protected abstract void openCamera() throws Exception;
	protected abstract void releaseCamera() throws Exception;
	protected abstract boolean isOpenedCamera();
	protected abstract boolean isFrontCamera();
	protected abstract int getCameraOrientation();
	protected abstract void configureCamera() throws Exception;
	protected abstract boolean startCamera() throws Exception;
	protected abstract void stopCamera() throws Exception;
	protected abstract void takePictureCamera(int flashMode) throws Exception;
	protected abstract void setFocusModeCamera(int focusMode) throws Exception;
	protected abstract void autoFocusCamera() throws Exception;
	protected abstract void autoFocusOnPointCamera(float x, float y) throws Exception;
	protected abstract boolean isTorchActiveCamera() throws Exception;
	protected abstract void setTorchModeCamera(int torchMode, float level) throws Exception;

	private static boolean mFlagInitialized = false;
	private synchronized static void initialize() {
		if (mFlagInitialized) {
			return;
		}
		mFlagInitialized = true;
		SlibActivity.addResumeActivityListener(new SlibActivity.ResumeActivityListener() {
			@Override
			public void onResumeActivity(Activity activity) {
				if (!(hasFeature(activity))) {
					return;
				}
				synchronized (mSync) {
					try {
						if (mFlagRequestPermission) {
							mFlagRequestPermission = false;
							if (!(grantPermission(activity))) {
								onRequestPermissionsResult(activity);
							}
						}
						if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
							SCamera2.doResumeActivity();
						} else {
							SCamera1.doResumeActivity();
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
		});
		SlibActivity.addPauseActivityListener(new SlibActivity.PauseActivityListener() {
			@Override
			public void onPauseActivity(Activity activity) {
				if (!(hasFeature(activity))) {
					return;
				}
				synchronized (mSync) {
					try {
						if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && supportsCamera2) {
							SCamera2.doPauseActivity();
						} else {
							SCamera1.doPauseActivity();
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
		});
		SlibActivity.addPermissionResultListener(SlibActivity.REQUEST_PERMISSION_SCAMERA, new SlibActivity.PermissionResultListener() {
			@Override
			public void onPermissionResult(Activity activity, int requestCode, String[] permissions, int[] grantResults) {
				onRequestPermissionsResult(activity);
			}
		});
	}

	private static void onRequestPermissionsResult(Activity activity) {
		if (!(checkPermission(activity))) {
			return;
		}
		synchronized (mSync) {
			for (SCamera camera : mCameras) {
				if (camera.mFlagWaitingPermission) {
					camera.mFlagWaitingPermission = false;
					try {
						camera.reopen();
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
		}
	}

	protected static void log(String s) {
		Logger.info("Camera: " + s);
	}

}
