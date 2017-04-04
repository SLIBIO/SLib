/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_CAMERA_VIEW
#define CHECKHEADER_SLIB_UI_CAMERA_VIEW

#include "definition.h"

#include "video_view.h"

#include "../media/camera.h"

namespace slib
{

	class SLIB_EXPORT CameraView : public VideoView, public IVideoCaptureListener
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CameraView();
		
		~CameraView();

	public:
		virtual void start(const CameraParam& param);
		
		virtual void stop();
		
	public:
		SLIB_PROPERTY(AtomicPtr<IVideoCaptureListener>, FrameListener)
		
	protected:
		virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame);
		
	protected:
		AtomicRef<Camera> m_camera;
		
	};

}

#endif
