/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/ui/camera_view.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CameraView, VideoView)
	
	CameraView::CameraView()
	{
	}
	
	CameraView::~CameraView()
	{
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
	
	void CameraView::onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame)
	{
		updateCurrentFrame(frame);
	}

}
