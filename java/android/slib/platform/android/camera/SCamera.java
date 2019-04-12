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

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.view.Surface;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.Permissions;

public class SCamera implements Camera.PreviewCallback, Camera.PictureCallback, Camera.ErrorCallback {

	private final static Object sync = new Object();
	private static Vector<SCamera> cameraObjectList = new Vector<SCamera>();
	private static boolean flagRunningActivity = false;
	private static boolean flagRequestPermission = false;

	public static void onResumeActivity(Activity activity)
	{
		if (!(Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA_ANY))) {
			return;
		}
		synchronized (sync) {
			try {
				flagRunningActivity = true;
				if (flagRequestPermission) {
					flagRequestPermission = false;
					if (!(grantPermission(activity))) {
						onRequestPermissionsResult(activity);
					}
				}
				for (SCamera camera : cameraObjectList) {
					if (camera.flagRunningRequest) {
						camera.startCamera();					
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void onPauseActivity(Activity activity)
	{
		if (!(Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA_ANY))) {
			return;
		}
		synchronized (sync) {
			try {
				flagRunningActivity = false;
				for (SCamera camera : cameraObjectList) {
					camera.stopCamera();
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static SCameraInfo[] getCamerasList() {
		
		Vector<SCameraInfo> ret = new Vector<SCameraInfo>();
		boolean flagFirstFront = true;
		boolean flagFirstBack = true;
		
		try {
			int n = Camera.getNumberOfCameras();
			for (int i = 0; i < n; i++) {
				try {
					Camera.CameraInfo info = new Camera.CameraInfo();
					Camera.getCameraInfo(i, info);
					SCameraInfo sinfo = new SCameraInfo();
					sinfo.index = i;
					sinfo.cameraInfo = info;
					if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
						if (flagFirstFront) {
							sinfo.id = "FRONT";
							sinfo.name = "Internal Front Camera";
							flagFirstFront = false;
						} else {
							sinfo.id = "FRONT#" + i;							
							sinfo.name = "Internal Front Camera #" + i;
						}
					} else {
						if (flagFirstBack) {
							sinfo.id = "BACK";
							sinfo.name = "Internal Back Camera";
							flagFirstBack = false;
						} else {
							sinfo.id = "BACK#" + i;							
							sinfo.name = "Internal Back Camera #" + i;
						}
					}
					ret.add(sinfo);
				} catch (Exception e) {
					Logger.exception(e);
				}			
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return ret.toArray(new SCameraInfo[] {});
	}

	public static SCameraInfo queryCameraInfo(String id) {
		SCameraInfo[] infos = getCamerasList();
		if (id != null && id.length() > 0) {
			for (int i = 0; i < infos.length; i++) {
				if (infos[i].id.equals(id)) {
					return infos[i];
				}
			}
			return null;
		} else {
			for (int i = 0; i < infos.length; i++) {
				if (infos[i].id.equals("BACK")) {
					return infos[i];
				}
			}
			for (int i = 0; i < infos.length; i++) {
				if (infos[i].id.equals("FRONT")) {
					return infos[i];
				}
			}
			return null;
		}
	}

	public static SCamera create(Activity activity, String id, long nativeObject) {
		try {
			if (!(Permissions.hasFeature(activity, PackageManager.FEATURE_CAMERA_ANY))) {
				return null;
			}
			SCameraInfo info = queryCameraInfo(id);
			if (info == null) {
				return null;
			}
			if (grantPermission(activity)) {
				flagRequestPermission = true;
				SCamera ret = new SCamera(activity,null, info, nativeObject);
				cameraObjectList.add(ret);
				return ret;
			}
			Camera camera = Camera.open(info.index);
			synchronized (sync) {
				SCamera ret = new SCamera(activity, camera, info, nativeObject);
				cameraObjectList.add(ret);
				return ret;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	private static boolean grantPermission(Activity activity) {
		return Permissions.grantPermission(activity, Manifest.permission.CAMERA, SlibActivity.REQUEST_SCAMERA_PERMISSION);
	}

	private static boolean checkPermission(Context context) {
		return Permissions.checkPermission(context, Manifest.permission.CAMERA);
	}

	public static void onRequestPermissionsResult(Activity activity) {
		if (!(checkPermission(activity))) {
			return;
		}
		synchronized (sync) {
			try {
				for (SCamera camera : cameraObjectList) {
					if (camera.flagWaitingPermission) {
						camera.flagWaitingPermission = false;
						try {
							camera.setCamera(Camera.open(camera.info.index));
							if (camera.flagRunningRequest) {
								camera.startCamera();
							}
						} catch (Exception _e) {
							Logger.exception(_e);
						}
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	private Activity activity;
	private Camera camera;
	private SCameraInfo info;
	private long nativeObject;

	private boolean flagWaitingPermission = false;
	private boolean flagRunningRequest = false;
	private boolean flagRunning = false;

	private byte[] bufCapture;

	private SurfaceTexture texture = new SurfaceTexture(0);

	private long timeLastFrame;
	private long timeLastStart;
	
	public SCamera(Activity activity, Camera camera, SCameraInfo info, long nativeObject) {

		this.activity = activity;
		this.camera = camera;
		this.info = info;
		this.nativeObject = nativeObject;

		if (camera == null) {
			flagWaitingPermission = true;
		} else {
			setCamera(camera);
		}

		log("Created");
	}

	private void setCamera(Camera camera) {
		this.camera = camera;
		camera.setErrorCallback(this);
	}

	public void setSettings(int preferedWidth, int preferedHeight) {
		if (camera == null) {
			return;
		}
		try {
			Camera.Parameters params = camera.getParameters();
			if (preferedWidth > 0 && preferedHeight > 0) {
				List<Size> sizes = params.getSupportedPreviewSizes();
				int widthSel = 0, heightSel = 0;
				int distMin = -1;
				for (int i = 0; i < sizes.size(); i++) {
					Size size = sizes.get(i);
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
					params.setPreviewSize(widthSel, heightSel);
				}
			}
			params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
			camera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public void release() {
		try {
			cameraObjectList.remove(this);
			stop();
			if (camera != null) {
				camera.release();
				camera = null;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void start() {
		synchronized (sync) {
			flagRunningRequest = true;
			if (flagRunningActivity) {
				startCamera();			
			}
		}
	}
	
	private void startCamera() {
		try {
			if (camera == null) {
				return;
			}
			if (flagRunning) {
				return;
			}

			Camera.Parameters params;
			try {
				params = camera.getParameters();
			} catch (Exception _e) {
				setCamera(Camera.open(info.index));
				params = camera.getParameters();
			}

			timeLastStart = System.currentTimeMillis();

			camera.setPreviewCallbackWithBuffer(null);
			Size size = params.getPreviewSize();
			int len = getSizeForNV21(size.width, size.height);
			if (bufCapture == null || bufCapture.length != len) {
				bufCapture  = new byte[len];				
			}
			camera.addCallbackBuffer(bufCapture);
			camera.setPreviewCallbackWithBuffer(this);

			camera.setPreviewTexture(texture);
			
			camera.startPreview();
			
			flagRunning = true;
			
			log("Started: " + size.width + "x" + size.height + " Format:" + params.getPreviewFormat());

		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void stop() {
		synchronized (sync) {
			flagRunningRequest = false;
			if (flagRunningActivity) {
				stopCamera();
			}
		}
	}
	
	private void stopCamera() {
		try {
			if (camera == null) {
				return;
			}
			if (!flagRunning) {
				return;
			}
			camera.stopPreview();
			flagRunning = false;
			
			log("Stopped");
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public boolean isRunning() {
		if (camera == null) {
			return false;
		}
		return flagRunning;
	}
	
	private native void nativeOnFrame(long nativeObject, byte[] data, int width, int height, int orientation, int flip);
	public void onPreviewFrame(byte[] data, Camera camera) {
		timeLastFrame = System.currentTimeMillis();
		if (camera == null) {
			return;
		}
		if (flagRunning) {
			Size size = camera.getParameters().getPreviewSize();
			int flip;
			int orientation = (info.cameraInfo.orientation - getDisplayOrientation() + 360) % 360;
			if (info.cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
				flip = 1; // Horizontal
				orientation = (orientation + 180) % 360;
			} else {
				flip = 0;
			}
			nativeOnFrame(nativeObject, data, size.width, size.height, orientation, flip);
		}
		camera.addCallbackBuffer(data);
	}

	public void takePicture(int flashMode) {
		if (info.cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
			try {
				Camera.Parameters params = camera.getParameters();
				switch (flashMode) {
					case 1:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_ON);
						break;
					case 2:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
						break;
					default:
						params.setFlashMode(Camera.Parameters.FLASH_MODE_AUTO);
						break;
				}
				camera.setParameters(params);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		try {
			camera.takePicture(null, null, null, this);
		} catch (Exception e) {
			nativeOnPicture(nativeObject, null, 0, 0);
			Logger.exception(e);
		}
	}

	private native void nativeOnPicture(long nativeObject, byte[] data, int orientation, int flip);
	@Override
	public void onPictureTaken(byte[] data, Camera camera) {
		int flip;
		int orientation;
		if (info.cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
			flip = 1; // Horizontal
			orientation = (info.cameraInfo.orientation + getDisplayOrientation()) % 360;
		} else {
			flip = 0;
			orientation = (info.cameraInfo.orientation - getDisplayOrientation() + 360) % 360;
		}
		nativeOnPicture(nativeObject, data, orientation, flip);
		camera.startPreview();
	}

	public void setFocusMode(int mode) {
		try {
			Camera.Parameters params = camera.getParameters();
			switch (mode) {
				case 1: // ContinuousAutoFocus
					params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
					break;
				case 2: // SmoothContinuousAutoFocus
					params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
					break;
				default:
					params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
					break;
			}
			camera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void autoFocus() {
		try {
			camera.cancelAutoFocus();
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			Camera.Parameters params = camera.getParameters();
			params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
			camera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			camera.autoFocus(new Camera.AutoFocusCallback() {
				@Override
				public void onAutoFocus(boolean success, Camera camera) {
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	static int clampFocusArea(int x) {
		if (x < -1000) {
			return -1000;
		}
		if (x > 1000) {
			return 1000;
		}
		return x;
	}

	public void autoFocusOnPoint(float x, float y) {
		try {
			camera.cancelAutoFocus();
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			Camera.Parameters params = camera.getParameters();
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
			camera.setParameters(params);
		} catch (Exception e) {
			Logger.exception(e);
		}
		try {
			camera.autoFocus(new Camera.AutoFocusCallback() {
				@Override
				public void onAutoFocus(boolean success, Camera camera) {
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@Override
	public void onError(int error, Camera camera) {
		log("Error occured: Error No = " + error);
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

	private int getDisplayOrientation() {
		int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
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

	private void log(String s) {
		Logger.info("Camera [" + info.id + "] " + s);
	}


	private static boolean flagRunMonitor = false;

	static private void startMonitor() {
		if (flagRunMonitor) {
			return;
		}
		flagRunMonitor = true;
		new Thread(new Runnable() {
			@Override
			public void run() {
				runMonitor();
			}
		}).start();
	}

	static private void runMonitor() {
		while (flagRunMonitor) {
			synchronized (sync) {
				if (flagRunningActivity) {
					try {
						long t = System.currentTimeMillis();
						for (SCamera camera : cameraObjectList) {
							if (camera.flagRunning && camera.flagRunningRequest) {
								if (t - camera.timeLastFrame > 2000 && t - camera.timeLastStart > 5000) {
									camera.processHalt();
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

}
