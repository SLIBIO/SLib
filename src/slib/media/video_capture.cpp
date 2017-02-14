#include "../../../inc/slib/media/video_capture.h"

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
