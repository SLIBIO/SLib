#include "../../../inc/slib/media/video_capture.h"

SLIB_MEDIA_NAMESPACE_BEGIN

VideoCaptureParam::VideoCaptureParam()
{
	flagAutoStart = sl_true;
}

void VideoCapture::onCaptureVideoFrame(VideoCaptureFrame* frame)
{
	PtrLocker<IVideoCaptureListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onCaptureVideoFrame(this, frame);
	}
}

SLIB_MEDIA_NAMESPACE_END

