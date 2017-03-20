/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android.camera;

import java.util.List;
import java.util.Vector;

import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import slib.platform.android.Logger;

public class SCamera implements Camera.PreviewCallback, Camera.ErrorCallback {
	
	static Monitor monitor = new Monitor();
	static Object sync = new Object();
	static Vector<SCamera> cameraObjectList = new Vector<SCamera>();
	static boolean flagRunningActivity = false;

	public static void onResumeActivity(Activity activity)
	{
		synchronized (sync) {
			try {
				flagRunningActivity = true;
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
	
	static class Monitor implements Runnable {
		
		public Monitor() {
			new Thread(this).start();
		}
		
		public void run() {
			while (true) {
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

	public static SCamera create(String id, long nativeObject) {
		
		try {
			Camera camera = null;
			if (id != null && id.length() > 0) {
				camera = openCamera(id);
			} else {
				camera = Camera.open();			
			}
			if (camera == null) {
				return null;
			}			
			synchronized (sync) {
				SCamera ret = new SCamera(camera, id, nativeObject);
				cameraObjectList.add(ret);
				return ret;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static Camera openCamera(String id) {
		SCameraInfo[] infos = getCamerasList();
		for (int i = 0; i < infos.length; i++) {
			if (infos[i].id.equals(id)) {
				return Camera.open(infos[i].index);
			}
		}
		return null;
	}
	
	Camera camera;
	String id;
	long nativeObject;
	
	boolean flagRunningRequest = false;
	boolean flagRunning = false;
	
	byte[] bufCapture;
	
	SurfaceTexture texture = new SurfaceTexture(0);

	long timeLastFrame;
	long timeLastStart;
	
	public SCamera(Camera camera, String id, long nativeObject) {
		
		this.camera = camera;
		this.id = id;
		this.nativeObject = nativeObject;
		
		camera.setErrorCallback(this);

		log("Created");
	}
	
	public void setPreferedFrameSettings(int width, int height) {
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
			camera.release();
			camera = null;
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
			
			timeLastStart = System.currentTimeMillis();
			
			camera.setPreviewCallbackWithBuffer(null);
			Camera.Parameters params = camera.getParameters();
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
	
	private native void nativeOnFrame(long nativeObject, byte[] data, int width, int height);
	public void onPreviewFrame(byte[] data, Camera camera) {
		timeLastFrame = System.currentTimeMillis();
		if (camera == null) {
			return;
		}
		if (flagRunning) {
			Size size = camera.getParameters().getPreviewSize();
			nativeOnFrame(nativeObject, data, size.width, size.height);
		}
		camera.addCallbackBuffer(data);
	}
	
	void processHalt() {
		try {
			stopCamera();
			startCamera();
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
	
	private void log(String s) {
		Logger.info("Camera [" + id + "] " + s);
	}
}
