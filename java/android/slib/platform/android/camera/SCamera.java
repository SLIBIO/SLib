package slib.platform.android.camera;

import java.util.List;
import java.util.Vector;

import android.hardware.Camera;
import android.hardware.Camera.Size;
import slib.platform.android.Logger;

public class SCamera implements Camera.PreviewCallback {
	
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
			if (camera != null) {
				return null;
			}			
			SCamera ret = new SCamera(camera, nativeObject);
			return ret;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static Camera openCamera(String id) {
		SCameraInfo[] infos = getCamerasList();
		for (int i = 0; i < infos.length; i++) {
			if (infos[i].equals(id)) {
				return Camera.open(infos[i].index);
			}
		}
		return null;
	}
	
	Camera camera;
	long nativeObject;
	
	boolean flagRunning = false;
	byte[] bufCapture;
	
	public SCamera(Camera camera, long nativeObject) {
		this.camera = camera;
		this.nativeObject = nativeObject;
	}
	
	public void setPreferedFrameSize(int width, int height) {
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
			stop();
			camera.release();
			camera = null;
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void start() {
		try {
			if (camera == null) {
				return;
			}
			if (flagRunning) {
				return;
			}
			Size size = camera.getParameters().getPreviewSize();
			int len = getSizeForNV21(size.width, size.height);
			if (bufCapture == null || bufCapture.length != len) {
				bufCapture = new byte[size.width * size.height];				
			}
			camera.addCallbackBuffer(bufCapture);
			camera.setPreviewCallbackWithBuffer(this);
			camera.startPreview();
			flagRunning = true;			
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void stop() {
		try {
			if (camera == null) {
				return;
			}
			if (!flagRunning) {
				return;
			}
			camera.stopPreview();
			flagRunning = false;
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
		if (camera == null) {
			return;
		}
		if (!flagRunning) {
			return;
		}
		Size size = camera.getParameters().getPreviewSize();
		nativeOnFrame(nativeObject, data, size.width, size.height);
		camera.addCallbackBuffer(data);
	}
	
	private int getSizeForNV21(int width, int height) {
		return width * height + (width * height) / 2;
	}
}
