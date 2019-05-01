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

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.TotalCaptureResult;
import android.hardware.camera2.params.MeteringRectangle;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.annotation.NonNull;
import android.util.Size;
import android.view.Surface;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Vector;

import slib.platform.android.Logger;

@TargetApi(Build.VERSION_CODES.M)
public class SCamera2 extends SCamera {

	static Vector<SCameraInfo> getCameras(Activity activity) throws Exception {
		Vector<SCameraInfo> ret = new Vector<SCameraInfo>();
		CameraManager manager = getManager(activity);
		String[] ids = manager.getCameraIdList();
		boolean flagFirstFront = true;
		boolean flagFirstBack = true;
		for (String id : ids) {
			SCameraInfo info = new SCameraInfo();
			info.id = id;
			try {
				CameraCharacteristics characteristics = manager.getCameraCharacteristics(id);
				Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
				if (facing != null) {
					if (facing == CameraCharacteristics.LENS_FACING_FRONT) {
						if (flagFirstFront) {
							info.name = "Internal Front Camera";
							flagFirstFront = false;
						} else {
							info.name = "Internal Front Camera #" + id;
						}
					} else if (facing == CameraCharacteristics.LENS_FACING_BACK) {
						if (flagFirstBack) {
							info.name = "Internal Back Camera";
							flagFirstBack = false;
						} else {
							info.name = "Internal Back Camera #" + id;
						}
					}
				}
				if (info.name == null) {
					info.name = "Camera #" + id;
				}
				ret.add(info);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return ret;
	}

	static boolean isLegacyDevice(Activity activity) {
		try {
			CameraManager manager = getManager(activity);
			String[] ids = manager.getCameraIdList();
			for (String id : ids) {
				CameraCharacteristics characteristics = manager.getCameraCharacteristics(id);
				Integer level = characteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL);
				if (level != null && level == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY) {
					return true;
				}
			}
			return false;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return true;
	}

	private CameraDevice mDevice;
	private CameraCaptureSession mSession;

	private boolean mFlagReleased = false;
	private CameraCharacteristics mCharacteristics;
	private StreamConfigurationMap mStreamConfig;
	private CaptureRequest.Builder mPreviewRequestBuilder;
	private CaptureRequest.Builder mPhotoRequestBuilder;
	private ImageReader mImageReader;
	private ImageReader mImageReaderPhoto;

	@Override
	protected boolean initCamera() throws Exception {
		String id = mId;
		mId = resolveCameraId(mActivity, id);
		if (mId == null) {
			throw new RuntimeException("Camera device is not found: id=" + id);
		}
		CameraManager manager = getManager(mActivity);
		mCharacteristics = manager.getCameraCharacteristics(mId);
		mStreamConfig = mCharacteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
		if (mStreamConfig == null) {
			throw new RuntimeException("CameraCharacteristics is not found: SCALER_STREAM_CONFIGURATION_MAP");
		}
		return true;
	}

	@SuppressLint("MissingPermission")
	@Override
	protected void openCamera() {
		if (mFlagReleased) {
			return;
		}
		if (mDevice != null) {
			return;
		}
		startBackgroundThread();
		CameraManager manager = getManager(mActivity);
		try {
			manager.openCamera(mId, new CameraDevice.StateCallback() {
				@Override
				public void onOpened(@NonNull CameraDevice device) {
					synchronized (mSync) {
						if (!mFlagReleased) {
							mDevice = device;
							if (mFlagRunningRequest) {
								start();
							}
						}
					}
				}

				@Override
				public void onClosed(@NonNull CameraDevice device) {
					synchronized (mSync) {
						for (SCamera obj : mCameras) {
							if (obj instanceof SCamera2) {
								SCamera2 camera = (SCamera2) obj;
								if (mId.equals(camera.mId)) {
									try {
										reopen();
										return;
									} catch (Exception e) {
										Logger.exception(e);
									}
								}
							}
						}
					}
				}

				@Override
				public void onDisconnected(@NonNull CameraDevice device) {
					release();
				}

				@Override
				public void onError(@NonNull CameraDevice camera, int error) {
					log("Error: " + error);
				}
			}, mBackgroundHandler);
		} catch (CameraAccessException e) {
			Logger.exception(e);
		}
	}

	@Override
	protected void releaseCamera() {
		mFlagReleased = true;
		if (mDevice != null) {
			try {
				mDevice.close();
			} catch (Exception e) {
				Logger.exception(e);
			}
			mDevice = null;
		}
	}

	@Override
	protected boolean isOpenedCamera() {
		if (mFlagReleased) {
			return false;
		}
		return mDevice != null;
	}

	@Override
	protected boolean isFrontCamera() {
		Integer n = mCharacteristics.get(CameraCharacteristics.LENS_FACING);
		return n != null && n == CameraCharacteristics.LENS_FACING_FRONT;
	}

	@Override
	protected int getCameraOrientation() {
		Integer n = mCharacteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);
		if (n != null) {
			return n;
		} else {
			return 0;
		}
	}

	@Override
	protected void configureCamera() {
	}

	@Override
	protected boolean startCamera() throws Exception {

		createImageReader();

		mPreviewRequestBuilder = mDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO);
		mPreviewRequestBuilder.addTarget(mImageReader.getSurface());

		mPhotoRequestBuilder = mDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
		mPhotoRequestBuilder.set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO);
		mPhotoRequestBuilder.addTarget(mImageReaderPhoto.getSurface());

		ArrayList<Surface> surfaces = new ArrayList<Surface>();
		surfaces.add(mImageReader.getSurface());
		surfaces.add(mImageReaderPhoto.getSurface());
		mDevice.createCaptureSession(surfaces, new CameraCaptureSession.StateCallback() {
			@Override
			public void onConfigured(@NonNull CameraCaptureSession session) {
				synchronized (mSync) {
					if (!mFlagReleased && mFlagRunningRequest) {
						mSession = session;
						try {
							runPreview();
							log("Started with size - " + mImageReader.getWidth() + ", " + mImageReader.getHeight());
						} catch (Exception e) {
							Logger.exception(e);
						}
					}
				}
			}

			@Override
			public void onConfigureFailed(@NonNull CameraCaptureSession session) {
				log("Failed to create session");
			}
		}, mBackgroundHandler);
		return true;
	}

	@Override
	protected void stopCamera() {
		try {
			mSession.close();
		} catch (Exception e) {
			Logger.exception(e);
		}
		mSession = null;
	}

	@Override
	protected void takePictureCamera(int flashMode) throws Exception {
		if (mSession == null) {
			return;
		}
		if (flashMode == FLASH_MODE_OFF) {
			mPhotoRequestBuilder.set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_OFF);
			mPhotoRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON);
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON);
			runCapture();
			return;
		}
		mPhotoRequestBuilder.set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_OFF);
		if (flashMode == FLASH_MODE_ON) {
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_ALWAYS_FLASH);
			mPhotoRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_ALWAYS_FLASH);
		} else {
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
			mPhotoRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
		}
		runPreview();
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_START);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START);
		mSession.capture(mPreviewRequestBuilder.build(), new CameraCaptureSession.CaptureCallback() {
			@Override
			public void onCaptureCompleted(@NonNull CameraCaptureSession session, @NonNull CaptureRequest request, @NonNull TotalCaptureResult result) {
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_CANCEL);
				runCapture();
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_IDLE);
			}
		}, mBackgroundHandler);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_IDLE);
	}

	@Override
	protected void setFocusModeCamera(int mode) {
		if (mSession == null) {
			return;
		}
		switch (mode) {
			case FOCUS_MODE_CONTINUOUS_AUTO_FOCUS:
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE);
				break;
			case FOCUS_MODE_SMOOTH_CONTINUOUS_AUTO_FOCUS:
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO);
				break;
			default:
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_AUTO);
				break;
		}
		runPreview();
	}

	@Override
	protected void autoFocusCamera() throws Exception {
		if (mSession == null) {
			return;
		}
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_AUTO);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
		runPreview();
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_START);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START);
		mSession.capture(mPreviewRequestBuilder.build(), new CameraCaptureSession.CaptureCallback() {
			@Override
			public void onCaptureCompleted(@NonNull CameraCaptureSession session, @NonNull CaptureRequest request, @NonNull TotalCaptureResult result) {
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_CANCEL);
				runPreview();
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_IDLE);
			}
		}, mBackgroundHandler);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_IDLE);
	}

	static private int clampFocusArea(int x, int max) {
		if (x < 0) {
			return 0;
		}
		if (x > max) {
			return max;
		}
		return x;
	}

	@Override
	protected void autoFocusOnPointCamera(float x, float y) throws Exception {
		Rect activeSize = mCharacteristics.get(CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE);
		if (activeSize == null) {
			return;
		}
		int width = activeSize.width();
		int height = activeSize.height();
		if (mSession == null) {
			return;
		}
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_AUTO);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
		runPreview();
		{
			int ix = clampFocusArea((int)(x * width), width - 1);
			int iy = clampFocusArea((int)(x * height), height - 1);
			int size = Math.min(width, height) / 20;
			int weight = 200;
			MeteringRectangle rect = new MeteringRectangle(clampFocusArea(ix - size, width - 1), clampFocusArea(iy - size, height - 1), clampFocusArea(ix + size, width - 1), clampFocusArea(iy + size, height - 1), weight);
			MeteringRectangle[] rects = new MeteringRectangle[] {rect};
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_REGIONS, rects);
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_REGIONS, rects);
		}
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_START);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START);
		mSession.capture(mPreviewRequestBuilder.build(), new CameraCaptureSession.CaptureCallback() {
			@Override
			public void onCaptureCompleted(@NonNull CameraCaptureSession session, @NonNull CaptureRequest request, @NonNull TotalCaptureResult result) {
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_CANCEL);
				runPreview();
				mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_IDLE);
			}
		}, mBackgroundHandler);
		mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_IDLE);
		{
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_REGIONS, null);
			mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AE_REGIONS, null);
		}
	}

	@Override
	protected boolean isTorchActiveCamera() {
		Integer value = mPreviewRequestBuilder.get(CaptureRequest.FLASH_MODE);
		return value != null && value == CaptureRequest.FLASH_MODE_TORCH;
	}

	@Override
	protected void setTorchModeCamera(int mode, float level) throws Exception {
		if (mode == TORCH_MODE_ON) {
			mPreviewRequestBuilder.set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_TORCH);
		} else {
			mPreviewRequestBuilder.set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_OFF);
		}
		runPreview();
	}

	static boolean isMobileDeviceTorchActive2(Activity activity) {
		try {
			CameraManager manager = getManager(activity);
			if (manager != null) {
				String id = getBackCameraId(manager);
				if (id != null) {
					synchronized (mSync) {
						for (SCamera camera : mCameras) {
							try {
								if (camera.mId != null && camera.mId.equals(id)) {
									return camera.isTorchActive();
								}
							} catch (Exception e) {
								Logger.exception(e);
							}
						}
					}
					return mTorchDevices.contains(id);
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	static void setMobileDeviceTorchMode2(Activity activity, int mode, float level) {
		try {
			CameraManager manager = getManager(activity);
			if (manager != null) {
				String id = getBackCameraId(manager);
				if (id != null) {
					synchronized (mSync) {
						for (SCamera camera : mCameras) {
							try {
								if (camera.mId != null && camera.mId.equals(id)) {
									if (camera.isRunning()) {
										camera.setTorchMode(mode, level);
										return;
									}
									break;
								}
							} catch (Exception e) {
								Logger.exception(e);
							}
						}
					}
					manager.setTorchMode(id, mode == TORCH_MODE_ON);
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static String getBackCameraId(CameraManager manager) throws Exception {
		String[] ids = manager.getCameraIdList();
		for (String id : ids) {
			try {
				CameraCharacteristics characteristics = manager.getCameraCharacteristics(id);
				Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
				if (facing != null) {
					if (facing == CameraCharacteristics.LENS_FACING_BACK) {
						return id;
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return null;
	}

	private static boolean mFlagInitializedCameraManager = false;
	private static final Object mLockInitizingCameraManager = new Object();
	private static HashSet<String> mTorchDevices = new HashSet<String>();

	private static CameraManager getManager(Activity activity) {
		CameraManager manager = (CameraManager)(activity.getSystemService(Context.CAMERA_SERVICE));
		if (manager != null) {
			if (!mFlagInitializedCameraManager) {
				synchronized (mLockInitizingCameraManager) {
					if (!mFlagInitializedCameraManager) {
						mFlagInitializedCameraManager = true;
						try {
							manager.registerTorchCallback(new CameraManager.TorchCallback() {
								@Override
								public void onTorchModeUnavailable(@NonNull String cameraId) {
									mTorchDevices.remove(cameraId);
								}

								@Override
								public void onTorchModeChanged(@NonNull String cameraId, boolean enabled) {
									if (enabled) {
										mTorchDevices.add(cameraId);
									} else {
										mTorchDevices.remove(cameraId);
									}
								}
							}, mBackgroundHandler);
						} catch (Exception e) {
							Logger.exception(e);
						}
					}
				}
			}
		}
		return manager;
	}

	private static String resolveCameraId(Activity activity, String id) {
		boolean flagBack = id == null || id.toUpperCase().equals("BACK");
		boolean flagFront = id == null || id.toUpperCase().equals("FRONT");
		if (flagBack || flagFront) {
			try {
				CameraManager manager = getManager(activity);
				String[] ids = manager.getCameraIdList();
				for (String dev : ids) {
					try {
						CameraCharacteristics characteristics = manager.getCameraCharacteristics(dev);
						Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
						if (facing != null) {
							if (flagFront) {
								if (facing == CameraCharacteristics.LENS_FACING_FRONT) {
									return dev;
								}
							}
							if (flagBack) {
								if (facing == CameraCharacteristics.LENS_FACING_BACK) {
									return dev;
								}
							}
						}
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
		return id;
	}

	private void createImageReader() {
		createVideoImageReader();
		createPhotoImageReader();
	}

	private void createVideoImageReader() {
		int format = ImageFormat.YUV_420_888;
		int preferedWidth = mSettingsWidth;
		int preferedHeight = mSettingsHeight;
		Size[] sizes = mStreamConfig.getOutputSizes(format);
		int widthSel = 0;
		int heightSel = 0;
		{
			if (preferedWidth <= 0 || preferedHeight <= 0) {
				preferedWidth = 640;
				preferedHeight = 480;
			}
			int distMin = -1;
			for (Size size : sizes) {
				int width = size.getWidth();
				int height = size.getHeight();
				int dist1 = (width - preferedWidth) * (width - preferedWidth) + (height - preferedHeight) * (height - preferedHeight);
				int dist2 = (width - preferedHeight) * (width - preferedHeight) + (height - preferedWidth) * (height - preferedWidth);
				int dist = Math.min(dist1, dist2);
				if (distMin < 0 || dist < distMin) {
					widthSel = width;
					heightSel = height;
					distMin = dist;
				}
			}
		}
		if (mImageReader != null && mImageReader.getWidth() == widthSel && mImageReader.getHeight() == heightSel) {
			return;
		}
		mImageReader = ImageReader.newInstance(widthSel, heightSel, format, 2);
		mImageReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
			@Override
			public void onImageAvailable(ImageReader reader) {
				try {
					Image image = reader.acquireLatestImage();
					if (image != null) {
						Image.Plane[] planes = image.getPlanes();
						onFrame2(image.getWidth(), image.getHeight(), planes[0].getBuffer(), planes[1].getBuffer(), planes[2].getBuffer(), planes[0].getRowStride(), planes[1].getRowStride(), planes[1].getPixelStride());
						image.close();
					}
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		}, mBackgroundHandler);
	}

	private void createPhotoImageReader() {
		int format = ImageFormat.JPEG;
		Size[] sizes = mStreamConfig.getOutputSizes(format);
		int widthSel = 0;
		int heightSel = 0;
		{
			int distMax = -1;
			for (Size size : sizes) {
				int width = size.getWidth();
				int height = size.getHeight();
				int d = width * width + height * height;
				if (distMax < 0 || d > distMax) {
					widthSel = width;
					heightSel = height;
					distMax = d;
				}
			}
		}
		if (mImageReaderPhoto != null && mImageReaderPhoto.getWidth() == widthSel && mImageReaderPhoto.getHeight() == heightSel) {
			return;
		}
		mImageReaderPhoto = ImageReader.newInstance(widthSel, heightSel, format, 1);
		mImageReaderPhoto.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
			@Override
			public void onImageAvailable(ImageReader reader) {
				try {
					Image image = reader.acquireLatestImage();
					if (image != null) {
						log("Taken picture with size - " + image.getWidth() + ", " + image.getHeight());
						Image.Plane[] planes = image.getPlanes();
						onPicture2(planes[0].getBuffer());
						image.close();
					}
					runPreview();
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		}, mBackgroundHandler);
	}

	private void runPreview() {
		if (mSession == null) {
			return;
		}
		try {
			mSession.setRepeatingRequest(mPreviewRequestBuilder.build(), null, mBackgroundHandler);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private void runCapture() {
		if (mSession == null) {
			return;
		}
		try {
			mSession.stopRepeating();
			mSession.capture(mPhotoRequestBuilder.build(), null, mBackgroundHandler);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static Handler mBackgroundHandler;
	private static HandlerThread mBackgroundThread;

	private static synchronized void startBackgroundThread() {
		if (mBackgroundThread == null) {
			try {
				mBackgroundThread = new HandlerThread("Camera Background");
				mBackgroundThread.start();
				mBackgroundHandler = new Handler(mBackgroundThread.getLooper());
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	static void doResumeActivity() {
	}

	static void doPauseActivity() {
	}

}