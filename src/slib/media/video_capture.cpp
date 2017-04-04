/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/video_capture.h"

namespace slib
{

	VideoCaptureFrame::VideoCaptureFrame()
	{
	}

	VideoCaptureFrame::~VideoCaptureFrame()
	{
	}


	IVideoCaptureListener::IVideoCaptureListener()
	{
	}

	IVideoCaptureListener::~IVideoCaptureListener()
	{
	}


	VideoCaptureParam::VideoCaptureParam()
	{
		flagAutoStart = sl_true;
	}

	VideoCaptureParam::~VideoCaptureParam()
	{
	}


	SLIB_DEFINE_OBJECT(VideoCapture, Object)

	VideoCapture::VideoCapture()
	{
	}

	VideoCapture::~VideoCapture()
	{
	}

	void VideoCapture::_init(const VideoCaptureParam& param)
	{
		setListener(param.listener);
		setOnCaptureVideoFrame(param.onCaptureVideoFrame);
	}

	void VideoCapture::_onCaptureVideoFrame(VideoCaptureFrame* frame)
	{
		PtrLocker<IVideoCaptureListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onCaptureVideoFrame(this, frame);
		}
		getOnCaptureVideoFrame()(this, frame);
	}

}
