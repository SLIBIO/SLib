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

#include "slib/media/camera.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(CameraInfo)

	CameraInfo::CameraInfo()
	{
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(CameraTakePictureResult)
	
	CameraTakePictureResult::CameraTakePictureResult()
	{
		flagSuccess = sl_false;
		
		rotation = RotationMode::Rotate0;
		flip = FlipMode::None;
		
		frame = sl_null;
	}
	
	Ref<Image> CameraTakePictureResult::getImage()
	{
		if (frame) {
			return Image::create(frame->image);
		}
		if (jpeg.isNotNull()) {
			return Image::loadJPEG(jpeg.getData(), jpeg.getSize());
		}
		return sl_null;
	}
	
	Ref<Drawable> CameraTakePictureResult::getDrawable()
	{
		if (frame) {
			return Image::create(frame->image);
		}
		if (jpeg.isNotNull()) {
			return PlatformDrawable::loadFromMemory(jpeg.getData(), jpeg.getSize());
		}
		return sl_null;
	}
	
	Memory CameraTakePictureResult::getJpeg()
	{
		if (jpeg.isNotNull()) {
			return jpeg;
		}
		if (frame) {
			Ref<Image> image = Image::create(frame->image);
			if (image.isNotNull()) {
				return image->saveJPEG();
			}
		}
		return sl_null;
	}
	
	void CameraTakePictureResult::setFrame(const VideoCaptureFrame& _frame)
	{
		frame = &_frame;
		rotation = frame->rotation;
		flip = frame->flip;
	}
	
	void CameraTakePictureResult::setJpeg(const Memory& _jpeg)
	{
		jpeg = _jpeg;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(CameraTakePictureParam)
	
	CameraTakePictureParam::CameraTakePictureParam()
	{
		flashMode = CameraFlashMode::Auto;
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(CameraParam)

	CameraParam::CameraParam()
	{
		preferedFrameWidth = 0;
		preferedFrameHeight = 0;
		preferedFrameFormat = BitmapFormat::YUV_NV21;
	}

	void CameraParam::setFrontCamera()
	{
		deviceId = "FRONT";
	}

	sl_bool CameraParam::isFrontCamera()
	{
		return deviceId == "FRONT";
	}

	void CameraParam::setBackCamera()
	{
		deviceId = "BACK";
	}

	sl_bool CameraParam::isBackCamera()
	{
		return deviceId == "BACK";
	}
	
	SLIB_DEFINE_OBJECT(Camera, VideoCapture)

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
		CameraTakePictureParam param;
		while (m_queueTakePictureRequests.pop_NoLock(&param)) {
			CameraTakePictureResult result;
			param.onComplete(result);
		}
	}
	
	void Camera::takePicture(const CameraTakePictureParam& param)
	{
		if (!(isRunning()) || m_queueTakePictureRequests.getCount() > 100) {
			CameraTakePictureResult result;
			param.onComplete(result);
			return;
		}
		m_queueTakePictureRequests.push(param);
	}
	
	void Camera::setFocusMode(CameraFocusMode mode)
	{
	}
	
	void Camera::autoFocus()
	{
	}
	
	void Camera::autoFocusOnPoint(sl_real x, sl_real y)
	{
	}
	
	sl_bool Camera::isAdjustingFocus()
	{
		return sl_false;
	}
	
	void Camera::onCaptureVideoFrame(VideoCaptureFrame& frame)
	{
		VideoCapture::onCaptureVideoFrame(frame);
		if (m_queueTakePictureRequests.isNotEmpty()) {
			CameraTakePictureParam param;
			if (m_queueTakePictureRequests.pop(&param)) {
				CameraTakePictureResult result;
				result.flagSuccess = sl_true;
				result.setFrame(frame);
				param.onComplete(result);
			}
		}
	}
	
}
