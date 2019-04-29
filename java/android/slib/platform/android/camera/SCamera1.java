/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class SCamera1 extends SCamera implements Camera.PreviewCallback, Camera.PictureCallback, Camera.ErrorCallback {

	static class Info extends SCameraInfo {
		int index;
		Camera.CameraInfo info;
	}

	static Vector<SCameraInfo> getCameras() {
		Vector<SCameraInfo> ret = new Vector<SCameraInfo>();
		boolean flagFirstFront = true;
		boolean flagFirstBack = true;
		int n = Camera.getNumberOfCameras();
		for (int i = 0; i < n; i++) {
			try {
				Camera.CameraInfo info = new Camera.CameraInfo();
				Camera.getCameraInfo(i, info);
				Info item = new Info();
				item.index = i;
				item.info = info;
				if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
					if (flagFirstFront) {
						item.id = "FRONT";
						item.name = "Internal Front Camera";
						flagFirstFront = false;
					} else {
						item.id = "FRONT#" + i;
						item.name = "Internal Front Camera #" + i;
					}
				} else {
					if (flagFirstBack) {
						item.id = "BACK";
						item.name = "Internal Back Camera";
						flagFirstBack = false;
					} else {
						item.id = "BACK#" + i;
						item.name = "Internal Back Camera #" + i;
					}
				}
				ret.add(item);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return ret;
	}

	private Camera mCamera;
	private int mCameraIndex = 0;
	private Camera.CameraInfo mCameraInfo;

	private byte[] mBufCapture;
	private SurfaceTexture mTexture = new SurfaceTexture(0);

	private long mTimeLastFrame;
	private long mTimeLastStart;

	@Override
	protected boolean initCamera() {
		Info info = queryCameraInfo(mId);
		if (info != null) {
			mCameraIndex = info.index;
			mCameraInfo = info.info;
			return true;
		}
		return false;
	}

	@Override
	protected void openCamera() {
		mCamera = Camera.open(mCameraIndex);
		mCamera.setErrorCallback(this);
		try {
			configureCamera();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@Override
	protected void releaseCamera() {
		if (mCamera != null) {
			try {
				mCamera.release();
			} catch (Exception e) {
				Logger.exception(e);
			}
			mCamera = null;
		}
	}

	@Override
	protected boolean isOpenedCamera() {
		return mCamera != null;
	}

	@Override
	protected boolean isFrontCamera() {
		return mCameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT;
	}

	@Override
	protected  int getCameraOrientation() {
		return mCameraInfo.orientation;
	}

	@Override
	protected void configureCamera() {
		int preferedWidth = mSettingsWidth;
		int preferedHeight = mSettingsHeight;
		Camera.Parameters params = mCamera.getParameters();
		{
			if (preferedWidth <= 0 || preferedHeight <= 0) {
				preferedWidth = 640;
				preferedHeight = 480;
			}
			List<Camera.Size> sizes = params.getSupportedPreviewSizes();
			int widthSel = 0, heightSel = 0;
			int distMin = -1;
			for (int i = 0; i < sizes.size(); i++) {
				Camera.Size size = sizes.get(i);
				if (size != null) {
					int dist1 = (size.width - preferedWidth) * (size.width - preferedWidth) + (size.height - preferedHeight) * (size.height - preferedHeight);
					int dist2 = (size.width - preferedHeight) * (size.width - preferedHeight) + (size.height - preferedWidth) * (size.height - preferedWidth);
					int dist = Math.min(dist1, dist2);
					if (distMin < 0 || dist < distMin) {
						widthSel = size.width;
						heightSel = size.height;
						distMin = dist;
					}
				}
			}
			if (widthSel > 0 && heightSel > 0) {
				try {
					params.setPreviewSize(widthSel, heightSel);
					mCamera.setParameters(params);
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		}
	}

	@Override
	protected boolean startCamera() throws Exception {

		if (!(SlibActivity.isRunningGlobal())) {
			return false;
		}
		Camera.Parameters params;
		try {
			params = mCamera.getParameters();
		} catch (Exception e) {
			openCamera();
			params = mCamera.getParameters();
		}

		mCamera.setPreviewCallbackWithBuffer(null);
		Camera.Size size = params.getPreviewSize();
		int len = getSizeForNV21(size.width, size.height);
		if (mBufCapture == null || mBufCapture.length != len) {
			mBufCapture = new byte[len];
		}
		mCamera.addCallbackBuffer(mBufCapture);
		mCamera.setPreviewCallbackWithBuffer(this);
		mCamera.setPreviewTexture(mTexture);
		mCamera.startPreview();

		mTimeLastStart = System.currentTimeMillis();

		log("Started with Param: " + size.width + "x" + size.height + " Format:" + params.getPreviewFormat());

		startMonitor();

		return true;
	}

	@Override
	protected void stopCamera() {
		mCamera.stopPreview();
	}

	public void onPreviewFrame(byte[] data, Camera camera) {
		mTimeLastFrame = System.currentTimeMillis();
		if (camera == null) {
			return;
		}
		if (!(isOpenedCamera())) {
			return;
		}
		try {
			Camera.Size size = camera.getParameters().getPreviewSize();
			onFrame(data, size.width, size.height);
			camera.addCallbackBuffer(data);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	protected void takePictureCamera(int flashMode) {
		if (mCameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
			try {
				Camera.Parameters params = mCamera.getParameters();
				switch (flashMode) {
					case FLASH_MODE_ON:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_ON);
						break;
					case FLASH_MODE_OFF:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
						break;
					default:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_AUTO);
						break;
				}
				mCamera.setParameters(params);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		mCamera.takePicture(null, null, null, this);
	}

	@Override
	public void onPictureTaken(byte[] data, Camera camera) {
		if (camera == null) {
			return;
		}
		if (!(isOpenedCamera())) {
			return;
		}
		try {
			onPicture(data);
			if (mFlagRunning) {
				camera.startPreview();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@Override
	protected void setFocusModeCamera(int mode) {
		Camera.Parameters params = mCamera.getParameters();
		switch (mode) {
			case FOCUS_MODE_CONTINUOUS_AUTO_FOCUS:
				params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
				break;
			case FOCUS_MODE_SMOOTH_CONTINUOUS_AUTO_FOCUS:
				params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
				break;
			default:
				params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
				break;
		}
		mCamera.setParameters(params);
	}

	@Override
	protected void autoFocusCamera() {
		try {
			mCamera.cancelAutoFocus();
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			Camera.Parameters params = mCamera.getParameters();
			params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
			mCamera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
		mCamera.autoFocus(new Camera.AutoFocusCallback() {
			@Override
			public void onAutoFocus(boolean success, Camera camera) {
			}
		});
	}

	static private int clampFocusArea(int x) {
		if (x < -1000) {
			return -1000;
		}
		if (x > 1000) {
			return 1000;
		}
		return x;
	}

	@Override
	protected void autoFocusOnPointCamera(float x, float y) {
		try {
			mCamera.cancelAutoFocus();
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			Camera.Parameters params = mCamera.getParameters();
			ArrayList<Camera.Area> areas = new ArrayList<Camera.Area>();
			int cx = clampFocusArea((int)(x * 2000) - 1000);
			int cy = clampFocusArea((int)(y * 2000) - 1000);
			int w = 100;
			Rect rect = new Rect();
			rect.left = clampFocusArea(cx - w);
			rect.top = clampFocusArea(cy - w);
			rect.right = clampFocusArea(cx + w);
			rect.bottom = clampFocusArea(cy + w);
			Camera.Area area = new Camera.Area(rect, 200);
			areas.add(area);
			params.setFocusAreas(areas);
			params.setMeteringAreas(areas);
			params.setFocusMode(Camera.Parameters.FOCUS_MODE_MACRO);
			mCamera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
		mCamera.autoFocus(new Camera.AutoFocusCallback() {
			@Override
			public void onAutoFocus(boolean success, Camera camera) {
			}
		});
	}

	@Override
	public void onError(int error, Camera camera) {
		log("Error occured: Error No = " + error);
	}

	private static Info queryCameraInfo(String id) {
		Vector<SCameraInfo> infos = getCameras();
		if (infos.isEmpty()) {
			return null;
		}
		if (id == null || id.isEmpty()) {
			return (Info)(infos.get(0));
		}
		for (SCameraInfo info : infos) {
			if (id.equalsIgnoreCase(info.id)) {
				return (Info)info;
			}
		}
		return null;
	}

	private int getSizeForNV21(int width, int height) {
		return width * height + (width * height) / 2;
	}

	private void processHalt() {
		try {
			stopCamera();
			startCamera();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static boolean mFlagRunMonitor = false;

	static private void startMonitor() {
		synchronized (mSync) {
			if (mFlagRunMonitor) {
				return;
			}
			mFlagRunMonitor = true;
			new Thread(new Runnable() {
				@Override
				public void run() {
					runMonitor();
				}
			}).start();
		}
	}

	static private void runMonitor() {
		while (mFlagRunMonitor) {
			synchronized (mSync) {
				if (SlibActivity.isRunningGlobal()) {
					try {
						long t = System.currentTimeMillis();
						for (SCamera obj : mCameras) {
							if (obj instanceof SCamera1) {
								SCamera1 camera = (SCamera1)obj;
								if (camera.isOpenedCamera() && camera.mFlagRunning && camera.mFlagRunningRequest) {
									if (t - camera.mTimeLastFrame > 2000 && t - camera.mTimeLastStart > 5000) {
										camera.processHalt();
									}
								}
							}
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
			try {
				Thread.sleep(2000);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	static void doResumeActivity() {
		for (SCamera obj : mCameras) {
			if (obj instanceof SCamera1) {
				SCamera1 camera = (SCamera1)obj;
				if (camera.isOpenedCamera() && camera.mFlagRunningRequest) {
					try {
						camera.mFlagRunning = camera.startCamera();
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
		}
	}

	static void doPauseActivity() {
		for (SCamera obj : mCameras) {
			if (obj instanceof SCamera1) {
				SCamera1 camera = (SCamera1)obj;
				if (camera.isOpenedCamera()) {
					try {
						if (camera.mFlagRunning) {
							camera.stopCamera();
							camera.mFlagRunning = false;
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			}
		}
	}

}
