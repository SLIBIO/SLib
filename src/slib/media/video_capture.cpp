#include "../../../inc/slib/media/video_capture.h"

SLIB_MEDIA_NAMESPACE_BEGIN

VideoCaptureParam::VideoCaptureParam()
{
	flagAutoStart = sl_true;
}

void IVideoCaptureListener::onCaptureVideoFrame(VideoCapture *capture, VideoCaptureFrame *frame)
{
}

VideoCapture::VideoCapture()
{
}

VideoCapture::~VideoCapture()
{
}

void VideoCapture::onCaptureVideoFrame(VideoCaptureFrame* frame)
{
	PtrLocker<IVideoCaptureListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onCaptureVideoFrame(this, frame);
	}
}

SLIB_MEDIA_NAMESPACE_END

