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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "slib/media/camera.h"

#include "slib/core/platform_apple.h"
#include "slib/core/endian.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>

#define TAG "Camera"

namespace slib
{
	class _priv_AVFoundation_Camera;
}

#if defined(SLIB_PLATFORM_IS_IOS)
@interface _priv_AVFoundation_Camera_Callback : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate, AVCapturePhotoCaptureDelegate>
#else
@interface _priv_AVFoundation_Camera_Callback : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate>
#endif
{
	@public slib::WeakRef<slib::_priv_AVFoundation_Camera> m_camera;
}
@end

namespace slib
{
#if defined(SLIB_PLATFORM_IS_IOS)
	extern UIInterfaceOrientation _g_slib_ui_screen_orientation;
#endif
	
	class _priv_AVFoundation_Camera_Static
	{
	public:
		dispatch_queue_t dispatch_queue;
		
	public:
		_priv_AVFoundation_Camera_Static()
		{
			dispatch_queue = dispatch_queue_create("SLIB_CAMERA", DISPATCH_QUEUE_SERIAL);
		}
	};
	SLIB_SAFE_STATIC_GETTER(_priv_AVFoundation_Camera_Static, _priv_AVFoundation_Camera_getStatic)

	class _priv_AVFoundation_Camera : public Camera
	{
	public:
		_priv_AVFoundation_Camera_Callback* m_callback;

		AVCaptureDevice* m_device;
		AVCaptureSession *m_session;
		AVCaptureVideoDataOutput* m_outputVideo;
#if defined(SLIB_PLATFORM_IS_IOS)
		AVCapturePhotoOutput* m_outputPhoto;
		UIInterfaceOrientation m_orientation;
		sl_bool m_flagFront;
#endif
		
		sl_bool m_flagRunning;
		
	public:
		_priv_AVFoundation_Camera()
		{
			m_callback = nil;
			
			m_device = nil;
			m_session = nil;
			m_outputVideo = nil;
#if defined(SLIB_PLATFORM_IS_IOS)
			m_outputPhoto = nil;
			m_orientation = UIInterfaceOrientationPortrait;
			m_flagFront = sl_false;
#endif
			
			m_flagRunning = sl_false;
		}

		~_priv_AVFoundation_Camera()
		{
			release();
		}
		
	public:
		static void logError(String error) {
			Log(TAG, error);
		}
		
		static void logError(String error, NSError* err) {
			Log(TAG, "%s: [%s]", error, [err localizedDescription]);
		}

		static Ref<_priv_AVFoundation_Camera> _create(const CameraParam& param)
		{
			Ref<_priv_AVFoundation_Camera> ret;
			
			_priv_AVFoundation_Camera_Callback* callback = [[_priv_AVFoundation_Camera_Callback alloc] init];
			
			AVCaptureDevice* device = _selectDevice(param.deviceId);
			if (device == nil) {
				logError("Camera is not found: " + param.deviceId);
				return ret;
			}
#if defined(SLIB_PLATFORM_IS_IOS)
			if ([device lockForConfiguration:nil]) {
				if ([device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
					[device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
				}
			}
#endif
			NSError* error;
			AVCaptureDeviceInput* input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
			if (input == nil) {
				logError("Failed to create AVCaptureDeviceInput", error);
				return ret;
			}
			
			AVCaptureSession *session = [[AVCaptureSession alloc] init];
			if (!([session canAddInput:input])) {
				logError("Can not add input to session");
				return ret;
			}
			[session addInput:input];

			AVCaptureVideoDataOutput* outputVideo = [[AVCaptureVideoDataOutput alloc] init];
			{
				[outputVideo setAlwaysDiscardsLateVideoFrames:YES];
				dispatch_queue_t queue = _priv_AVFoundation_Camera_getStatic()->dispatch_queue;
				[outputVideo setSampleBufferDelegate:callback queue:queue];
				NSMutableDictionary *settings = [[NSMutableDictionary alloc] init];
				if (BitmapFormats::getColorSpace(param.preferedFrameFormat) == ColorSpace::YUV) {
					[settings setObject:@(kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
				} else {
					[settings setObject:@(kCVPixelFormatType_32BGRA) forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
				}
				[outputVideo setVideoSettings:settings];
				
				if ([session canAddOutput:outputVideo]) {
					[session addOutput:outputVideo];
				}
				
			}
			
#if defined(SLIB_PLATFORM_IS_IOS)
			AVCapturePhotoOutput* outputPhoto = [[AVCapturePhotoOutput alloc] init];
			{
				[outputPhoto setHighResolutionCaptureEnabled:YES];
				if ([session canAddOutput:outputPhoto]) {
					[session addOutput:outputPhoto];
				}
			}
#endif

			AVCaptureSessionPreset preset = selectPresetForSession(session, param.preferedFrameWidth, param.preferedFrameHeight);
			if (preset != nil && [session canSetSessionPreset:preset]) {
				[session setSessionPreset:preset];
			}

			ret = new _priv_AVFoundation_Camera();
			if (ret.isNotNull()) {
				callback->m_camera = ret;
				ret->m_callback = callback;
				ret->m_device = device;
				ret->m_session = session;
				ret->m_outputVideo = outputVideo;
#if defined(SLIB_PLATFORM_IS_IOS)
				ret->m_outputPhoto = outputPhoto;
				ret->m_orientation = _g_slib_ui_screen_orientation;
				ret->m_flagFront = [device position] == AVCaptureDevicePositionFront;
#endif
				
				ret->_init(param);
				
				if (param.flagAutoStart) {
					ret->start();
				}
			}
			return ret;
		}
		
		struct _priv_PresetInfo
		{
			AVCaptureSessionPreset preset;
			sl_int32 width;
			sl_int32 height;
		};
		static AVCaptureSessionPreset selectPresetForSession(AVCaptureSession* session, sl_int32 req_width, sl_int32 req_height)
		{
			_priv_PresetInfo presets[] = {
				{AVCaptureSessionPreset352x288, 352, 288},
				{AVCaptureSessionPreset640x480, 640, 480},
#if defined(SLIB_PLATFORM_IS_MACOS)
				{AVCaptureSessionPreset960x540, 960, 540},
#endif
				{AVCaptureSessionPreset1280x720, 1280, 720},
#if !defined(SLIB_PLATFORM_IS_MACOS)
				{AVCaptureSessionPreset1920x1080, 1920, 1080},
				{AVCaptureSessionPreset3840x2160, 3840, 2160},
#endif
			};
			if (req_width > 0 && req_height > 0) {
				sl_int32 min_dist = 0;
				NSString* min_preset = nil;
				sl_int32 min_width = 0;
				sl_int32 min_height = 0;
				for (sl_uint32 i = 0; i < sizeof(presets)/sizeof(presets[0]); i++) {
					if ([session canSetSessionPreset:(presets[i].preset)]) {
						sl_int32 dist1 = (req_width - presets[i].width)*(req_width - presets[i].width) + (req_height - presets[i].height)*(req_height - presets[i].height);
						sl_int32 dist2 = (req_height - presets[i].width)*(req_height - presets[i].width) + (req_width - presets[i].height)*(req_width - presets[i].height);
						sl_int32 dist = SLIB_MIN(dist1, dist2);
						if (min_preset == nil || dist < min_dist) {
							min_preset = presets[i].preset;
							min_width = presets[i].width;
							min_height = presets[i].height;
							min_dist = dist;
						}
					}
				}
				if (min_preset != nil) {
					if ([session canSetSessionPreset:min_preset]) {
						return min_preset;
					}
				}
			}
			return 	AVCaptureSessionPresetPhoto;
		}

		static AVCaptureDevice* _selectDevice(String deviceId)
		{
#if !defined(SLIB_PLATFORM_IS_IOS_DEVICE)
			if (deviceId == "FRONT" || deviceId == "BACK") {
				deviceId.setNull();
			}
#endif
			NSArray *devices = [AVCaptureDevice devices];
			for (AVCaptureDevice* device in devices) {
				if ([device hasMediaType:AVMediaTypeVideo]) {
					if (deviceId.isEmpty()) {
						return device;
					}
#if defined(SLIB_PLATFORM_IS_IOS_DEVICE)
					if ([device position] == AVCaptureDevicePositionFront && deviceId == "FRONT") {
						return device;
					}
					if ([device position] == AVCaptureDevicePositionBack && deviceId == "BACK") {
						return device;
					}
#endif
					String uid = Apple::getStringFromNSString([device uniqueID]);
					if (uid == deviceId) {
						return device;
					}
				}
			}
			return NULL;
		}
		
		void release() override
		{
			ObjectLocker lock(this);
			stop();
			m_callback = nil;
			m_outputVideo = nil;
#if defined(SLIB_PLATFORM_IS_IOS)
			m_outputPhoto = nil;
#endif
			m_session = nil;
			m_device = nil;
		}

		sl_bool isOpened() override
		{
			return m_session != nil;
		}

		void start() override
		{
			ObjectLocker lock(this);
			if (m_session != nil) {
				if (!m_flagRunning) {
					[m_session startRunning];
					m_flagRunning = sl_true;
				}
			}
		}

		void stop() override
		{
			ObjectLocker lock(this);
			if (m_session != nil) {
				if (m_flagRunning) {
					[m_session stopRunning];
				}
			}
			m_flagRunning = sl_false;
		}

		sl_bool isRunning() override
		{
			return m_flagRunning;
		}
		
		static void _getFrame(VideoCaptureFrame& frame, CVImageBufferRef imageBuffer)
		{
			sl_uint32 width = (sl_uint32)(CVPixelBufferGetWidth(imageBuffer));
			sl_uint32 height = (sl_uint32)(CVPixelBufferGetHeight(imageBuffer));
			if (width > 0 && height > 0) {
				frame.image.width = width;
				frame.image.height = height;
				OSType type = CVPixelBufferGetPixelFormatType(imageBuffer);
				if (type == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
					frame.image.format = BitmapFormat::YUV_NV12;
					frame.image.data = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
					frame.image.pitch = (sl_int32)(CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0));
					frame.image.data1 = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 1);
					frame.image.pitch1 = (sl_int32)(CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 1));
				} else if (type == kCVPixelFormatType_32BGRA) {
					frame.image.format = BitmapFormat::BGRA;
					frame.image.data = CVPixelBufferGetBaseAddress(imageBuffer);
					frame.image.pitch = (sl_uint32)(CVPixelBufferGetBytesPerRow(imageBuffer));
				}
			}
		}
		
#if defined(SLIB_PLATFORM_IS_IOS)
		static RotationMode _getScreenRotation()
		{
			RotationMode rotation;
			switch (_g_slib_ui_screen_orientation) {
				case UIInterfaceOrientationPortraitUpsideDown:
					rotation = RotationMode::Rotate180;
					break;
				case UIInterfaceOrientationLandscapeLeft:
					rotation = RotationMode::Rotate90;
					break;
				case UIInterfaceOrientationLandscapeRight:
					rotation = RotationMode::Rotate270;
					break;
				default:
					rotation = RotationMode::Rotate0;
					break;
			}
			return rotation;
		}
#endif

		void _onFrame(CMSampleBufferRef sampleBuffer)
		{
			CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
			if (CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly) == kCVReturnSuccess) {
				VideoCaptureFrame frame;
				_getFrame(frame, imageBuffer);
				if (frame.image.format != BitmapFormat::None) {
#if defined(SLIB_PLATFORM_IS_IOS)
					if (m_flagFront) {
						frame.flip = FlipMode::Horizontal;
						frame.rotation = RotationMode::Rotate270 + _getScreenRotation();
					} else {
						frame.rotation = RotationMode::Rotate90 + _getScreenRotation();
					}
#endif
					onCaptureVideoFrame(frame);
				}
				CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
			}
		}
		
#if defined(SLIB_PLATFORM_IS_IOS)
		void takePicture(const CameraTakePictureParam& param) override
		{
			ObjectLocker lock(this);
			
			if (!isRunning()) {
				CameraTakePictureResult result;
				param.onComplete(result);
				return;
			}
			
			AVCaptureSession* session = m_session;
			AVCapturePhotoOutput* output = m_outputPhoto;
			if (session == nil || output == nil) {
				CameraTakePictureResult result;
				param.onComplete(result);
				return;
			}

			AVCapturePhotoSettings* settings = [[AVCapturePhotoSettings alloc] init];
			AVCaptureFlashMode flashMode;
			switch (param.flashMode) {
				case CameraFlashMode::On:
					flashMode = AVCaptureFlashModeOn;
					break;
				case CameraFlashMode::Off:
					flashMode = AVCaptureFlashModeOff;
					break;
				default:
					flashMode = AVCaptureFlashModeAuto;
					break;
			}
			@try {
				[settings setFlashMode:flashMode];
			} @catch (NSException*) {}
			
			m_queueTakePictureRequests.push(param);
			[output capturePhotoWithSettings:settings delegate:m_callback];
		}
		
		void _onCapturePhoto(NSData* data)
		{
			CameraTakePictureParam param;
			if (m_queueTakePictureRequests.pop(&param)) {
				CameraTakePictureResult result;
				if (data != nil) {
					result.flagSuccess = sl_true;
					result.setJpeg(Apple::getMemoryFromNSData(data));
					result.rotation = _getScreenRotation();
					if (m_flagFront) {
						result.flip = FlipMode::Horizontal;
						result.rotation = RotationMode::Rotate90 - result.rotation;
					} else {
						result.rotation = RotationMode::Rotate90 + result.rotation;
					}
				}
				param.onComplete(result);
			}
		}
		
		void onCaptureVideoFrame(VideoCaptureFrame& frame) override
		{
			VideoCapture::onCaptureVideoFrame(frame);
		}
#endif
		
		void setFocusMode(CameraFocusMode mode) override
		{
			ObjectLocker lock(this);
			AVCaptureDevice* device = m_device;
			if (device == nil) {
				return;
			}
			if ([device lockForConfiguration:nil]) {
				switch (mode) {
					case CameraFocusMode::ContinuousAutoFocus:
						if ([device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
							[device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
#if defined(SLIB_PLATFORM_IS_IOS)
							if ([device isSmoothAutoFocusSupported]) {
								[device setSmoothAutoFocusEnabled:NO];
							}
#endif
						}
						break;
					case CameraFocusMode::SmoothContinuousAutoFocus:
						if ([device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
							[device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
#if defined(SLIB_PLATFORM_IS_IOS)
							if ([device isSmoothAutoFocusSupported]) {
								[device setSmoothAutoFocusEnabled:YES];
							}
#endif
						}
						break;
					default:
						if ([device isFocusModeSupported:AVCaptureFocusModeLocked]) {
							[device setFocusMode:AVCaptureFocusModeLocked];
						}
						break;
				}
				[device unlockForConfiguration];
			}
		}
		
		void autoFocus() override
		{
			ObjectLocker lock(this);
			AVCaptureDevice* device = m_device;
			if (device != nil) {
				if ([device lockForConfiguration:nil]) {
					if ([device isFocusModeSupported:AVCaptureFocusModeAutoFocus]) {
						[device setFocusMode:AVCaptureFocusModeAutoFocus];
					}
					[device unlockForConfiguration];
				}
			}
		}

		void autoFocusOnPoint(sl_real x, sl_real y) override
		{
			ObjectLocker lock(this);
			AVCaptureDevice* device = m_device;
			if (device != nil) {
				if ([device lockForConfiguration:nil]) {
					if ([device isFocusPointOfInterestSupported]) {
						[device setFocusPointOfInterest:CGPointMake(x, y)];
					}
					if ([device isFocusModeSupported:AVCaptureFocusModeAutoFocus]) {
						[device setFocusMode:AVCaptureFocusModeAutoFocus];
					}
					[device unlockForConfiguration];
				}
			}
		}
		
		sl_bool isAdjustingFocus() override
		{
			ObjectLocker lock(this);
			AVCaptureDevice* device = m_device;
			if (device != nil) {
				if ([device isAdjustingFocus]) {
					return sl_true;
				}
			}
			return sl_false;
		}
		
	};


	Ref<Camera> Camera::create(const CameraParam& param)
	{
		return _priv_AVFoundation_Camera::_create(param);
	}

	List<CameraInfo> Camera::getCamerasList()
	{
		List<CameraInfo> ret;
		NSArray *devices = [AVCaptureDevice devices];
		for (AVCaptureDevice* device in devices) {
			if ([device hasMediaType:AVMediaTypeVideo]) {
				CameraInfo info;
				info.id = Apple::getStringFromNSString([device uniqueID]);
				info.name = Apple::getStringFromNSString([device localizedName]);
				ret.add_NoLock(info);
			}
		}
		return ret;
	}

}

@implementation _priv_AVFoundation_Camera_Callback

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
	slib::Ref<slib::_priv_AVFoundation_Camera> camera(m_camera);
	if (camera.isNotNull()) {
		camera->_onFrame(sampleBuffer);
	}
}

#if defined(SLIB_PLATFORM_IS_IOS)
- (void)captureOutput:(AVCapturePhotoOutput *)output
didFinishProcessingPhotoSampleBuffer:(CMSampleBufferRef)rawSampleBuffer
previewPhotoSampleBuffer:(CMSampleBufferRef)previewPhotoSampleBuffer
	 resolvedSettings:(AVCaptureResolvedPhotoSettings *)resolvedSettings
	  bracketSettings:(AVCaptureBracketedStillImageSettings *)bracketSettings
				error:(NSError *)error
{
	if (error != nil) {
		NSLog(@"%@", error.localizedDescription);
	}
	slib::Ref<slib::_priv_AVFoundation_Camera> camera(m_camera);
	if (camera.isNotNull()) {
		if (rawSampleBuffer != nil) {
			camera->_onCapturePhoto([AVCapturePhotoOutput JPEGPhotoDataRepresentationForJPEGSampleBuffer:rawSampleBuffer previewPhotoSampleBuffer:nil]);
		} else {
			camera->_onCapturePhoto(nil);
		}
	}
}

- (void)captureOutput:(AVCapturePhotoOutput *)output
didFinishProcessingPhoto:(AVCapturePhoto *)photo
				error:(NSError *)error
API_AVAILABLE(ios(11.0)){
	if (photo.isRawPhoto) {
		return;
	}
	if (error != nil) {
		NSLog(@"%@", error.localizedDescription);
	}
	slib::Ref<slib::_priv_AVFoundation_Camera> camera(m_camera);
	if (camera.isNotNull()) {
		camera->_onCapturePhoto([photo fileDataRepresentation]);
	}
}
#endif

@end

#endif
