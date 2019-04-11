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

#ifndef CHECKHEADER_SLIB_MEDIA_CAMERA
#define CHECKHEADER_SLIB_MEDIA_CAMERA

#include "definition.h"

#include "video_capture.h"

#include "../graphics/image.h"
#include "../core/function.h"

namespace slib
{
	class SLIB_EXPORT CameraInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		CameraInfo();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(CameraInfo)
		
	};
	
	enum class CameraFlashMode
	{
		Auto = 0,
		On = 1,
		Off = 2
	};
	
	enum class CameraFocusMode
	{
		Locked = 0,
		AutoFocus = 1,
		SmoothAutoFocus = 2
	};
	
	class SLIB_EXPORT CameraTakePictureResult
	{
	public:
		sl_bool flagSuccess;
		
		RotationMode rotation;
		FlipMode flip;

	public:
		CameraTakePictureResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(CameraTakePictureResult)
		
	public:
		Ref<Image> getImage();
		
		Ref<Drawable> getDrawable();
		
		Memory getJpeg();
		
		void setFrame(const VideoCaptureFrame& frame);
		
		void setJpeg(const Memory& jpeg);
		
	protected:
		const VideoCaptureFrame* frame;
		Memory jpeg;
		
	};
	
	class SLIB_EXPORT CameraTakePictureParam
	{
	public:
		CameraFlashMode flashMode;
		
		Function<void(CameraTakePictureResult& frame)> onComplete;
		
	public:
		CameraTakePictureParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(CameraTakePictureParam)
		
	};
	
	class SLIB_EXPORT CameraParam : public VideoCaptureParam
	{
	public:
		String deviceId;
		
		sl_uint32 preferedFrameWidth;
		sl_uint32 preferedFrameHeight;
		BitmapFormat preferedFrameFormat;
		
	public:
		CameraParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(CameraParam)
		
	public:
		void setFrontCamera();
		
		sl_bool isFrontCamera();
		
		
		void setBackCamera();
		
		sl_bool isBackCamera();
		
	};
	
	class SLIB_EXPORT Camera : public VideoCapture
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Camera();
		
		~Camera();
		
	public:
		virtual void takePicture(const CameraTakePictureParam& param);
		
		virtual void setFocusMode(CameraFocusMode mode);
		
		virtual void autoFocus();
		
		// x, y: 0~1
		virtual void autoFocusOnPoint(sl_real x, sl_real y);
		
		virtual sl_bool isAdjustingFocus();
		
	public:
		static Ref<Camera> create(const CameraParam& param);
		
		static List<CameraInfo> getCamerasList();
		
	protected:
		void onCaptureVideoFrame(VideoCaptureFrame& frame) override;
		
	protected:
		Queue<CameraTakePictureParam> m_queueTakePictureRequests;
		
	};

}

#endif
