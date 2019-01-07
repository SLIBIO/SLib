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

#ifndef CHECKHEADER_SLIB_UI_CAMERA_VIEW
#define CHECKHEADER_SLIB_UI_CAMERA_VIEW

#include "definition.h"

#include "video_view.h"

#include "../media/camera.h"

namespace slib
{

	class SLIB_EXPORT CameraView : public VideoView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CameraView();
		
		~CameraView();

	public:
		void start();
		
		virtual void start(const CameraParam& param);
		
		virtual void stop();
		
		sl_bool isAutoStart();
		
		void setAutoStart(sl_bool flagAutoStart);
		
		String getDeviceId();
		
		void setDeviceId(const String& deviceId);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(CameraView, Capture, VideoCaptureFrame* frame)

	protected:
		void onAttach() override;
		
	private:
		void _onCaptureCameraFrame(VideoCapture* capture, VideoCaptureFrame* frame);

	protected:
		AtomicRef<Camera> m_camera;
		sl_bool m_flagAutoStart;
		AtomicString m_deviceId;
		
	};

}

#endif
