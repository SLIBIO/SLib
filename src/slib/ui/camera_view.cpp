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

#include "slib/ui/camera_view.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CameraView, VideoView)
	
	CameraView::CameraView()
	{
		m_flagAutoStart = sl_false;
	}
	
	CameraView::~CameraView()
	{
	}

	void CameraView::start()
	{
		Ref<Camera> camera = m_camera;
		if (camera.isNotNull()) {
			return;
		}
		CameraParam param;
		String deviceId = m_deviceId;
		if (deviceId.isNotEmpty()) {
			if (deviceId.equalsIgnoreCase("FRONT")) {
				param.setFrontCamera();
			} else if (deviceId.equalsIgnoreCase("BACK")) {
				param.setBackCamera();
			} else {
				param.deviceId = deviceId;
			}
		} else {
			param.setBackCamera();
		}
		start(param);
	}
	
	void CameraView::start(const CameraParam& _param)
	{
		stop();
		CameraParam param(_param);
		param.listener = (WeakRef<CameraView>)(this);
		m_camera = Camera::create(param);
	}
	
	void CameraView::stop()
	{
		Ref<Camera> camera = m_camera;
		if (camera.isNotNull()) {
			camera->release();
		}
		m_camera.setNull();
	}
	
	sl_bool CameraView::isAutoStart()
	{
		return sl_true;
	}
	
	void CameraView::setAutoStart(sl_bool flagAutoStart)
	{
		m_flagAutoStart = flagAutoStart;
	}
	
	String CameraView::getDeviceId()
	{
		return m_deviceId;
	}
	
	void CameraView::setDeviceId(const String& deviceId)
	{
		m_deviceId = deviceId;
	}
	
	void CameraView::onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame)
	{
		updateCurrentFrame(frame);
	}

	void CameraView::onAttach()
	{
		if (m_flagAutoStart) {
			start();
		}
	}
	
}
