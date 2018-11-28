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

import java.util.List;
import java.util.Vector;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.view.Surface;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.helper.AppHelper;

public class SCamera implements Camera.PreviewCallback, Camera.ErrorCallback {

	private final static Object sync = new Object();
	private static Vector<SCamera> cameraObjectList = new Vector<SCamera>();
	private static Activity lastRunningActivity;
	private static boolean flagRunningActivity = false;
	private static boolean flagRequestPermission = false;

	static {
		startMonitor();
	}

	public static void onResumeActivity(Activity activity)
	{
		synchronized (sync) {
			try {
				lastRunningActivity = activity;
				flagRunningActivity = true;
				if (flagRequestPermission) {
					flagRequestPermission = false;
					if (grantPermission()) {
						onRequestPermissionsResult();
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

	public static SCamera create(String id, long nativeObject) {
		try {
			SCameraInfo info = queryCameraInfo(id);
			if (info == null) {
				return null;
			}
			if (!(grantPermission())) {
				SCamera ret = new SCamera(null, info, nativeObject);
				cameraObjectList.add(ret);
				return ret;
			}
			Camera camera = Camera.open(info.index);
			synchronized (sync) {
				SCamera ret = new SCamera(camera, info, nativeObject);
				cameraObjectList.add(ret);
				return ret;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	private static boolean grantPermission() {
		if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
			if (checkPermission()) {
				return true;
			} else {
				requestPermission();
				return false;
			}
		} else {
			return true;
		}
	}

	private static boolean checkPermission() {
		Activity activity = lastRunningActivity;
		if (activity == null) {
			return false;
		}
		Context context = activity.getApplicationContext();
		int result = AppHelper.checkSelfPermission(context, Manifest.permission.CAMERA);
		return result == PackageManager.PERMISSION_GRANTED;
	}

	private static void requestPermission() {
		Activity activity = lastRunningActivity;
		if (activity == null) {
			flagRequestPermission = true;
			return;
		}
		AppHelper.requestPermissions(activity, new String[]{ Manifest.permission.CAMERA }, SlibActivity.PERMISSION_REQUEST_CAMERA);
	}

	public static void onRequestPermissionsResult() {
		if (!(checkPermission())) {
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
	
	public SCamera(Camera camera, SCameraInfo info, long nativeObject) {
		
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
		if (flagSetPreferedFrameSettings) {
			setPreferedFrameSettings(preferedWidth, preferedHeight);
		}
	}

	private boolean flagSetPreferedFrameSettings = false;
	private int preferedWidth = 0;
	private int preferedHeight = 0;

	public void setPreferedFrameSettings(int width, int height) {
		preferedWidth = width;
		preferedHeight = height;
		flagSetPreferedFrameSettings = true;
		if (camera == null) {
			return;
		}
		try {
			if (width > 0 && height > 0) {
				Camera.Parameters params = camera.getParameters();
				List<Size> sizes = params.getSupportedPreviewSizes();
				int widthSel = 0, heightSel = 0;
				int distMin = -1;
				for (int i = 0; i < sizes.size(); i++) {
					Size size = sizes.get(i);
					if (size != null) {
						int dist1 = (size.width - width) * (size.width - width) + (size.height - height) * (size.height - height);
						int dist2 = (size.width - height) * (size.width - height) + (size.height - width) * (size.height - width);
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
					camera.setParameters(params);				
				}
			}
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
	
	public void stopCamera() {
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
	
	private native void nativeOnFrame(long nativeObject, byte[] data, int width, int height, int orientation);
	public void onPreviewFrame(byte[] data, Camera camera) {
		timeLastFrame = System.currentTimeMillis();
		if (camera == null) {
			return;
		}
		if (flagRunning) {
			Size size = camera.getParameters().getPreviewSize();
			int orientation = calculateDisplayOrientation();
			nativeOnFrame(nativeObject, data, size.width, size.height, orientation);
		}
		camera.addCallbackBuffer(data);
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

	private int calculateDisplayOrientation() {
		int orientation = info.cameraInfo.orientation;
		Activity activity = lastRunningActivity;
		if (activity == null) {
			return orientation;
		}
		int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
		int degrees = 0;
		switch (rotation) {
			case Surface.ROTATION_0:
				degrees = 0;
				break;
			case Surface.ROTATION_90:
				degrees = 90;
				break;
			case Surface.ROTATION_180:
				degrees = 180;
				break;
			case Surface.ROTATION_270:
				degrees = 270;
				break;
		}
		if (info.cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
			return (orientation + degrees) % 360;
		} else {
			return (orientation - degrees + 360) % 360;
		}
	}

	private void log(String s) {
		Logger.info("Camera [" + info.id + "] " + s);
	}


	public static boolean flagRunMonitor = false;

	static private void startMonitor() {
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
