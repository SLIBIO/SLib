#ifndef CHECKHEADER_SLIB_UI_CAMERA_VIEW
#define CHECKHEADER_SLIB_UI_CAMERA_VIEW

#include "definition.h"

#include "video_view.h"

#include "../media/camera.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT CameraView : public VideoView, public IVideoCaptureListener
{
	SLIB_DECLARE_OBJECT(CameraView, VideoView)
public:
	CameraView();
	
public:
	virtual void start(sl_uint32 cameraIndex);
	virtual void stop();
	
public:
	SLIB_PROPERTY_INLINE(Ptr<IVideoCaptureListener>, FrameListener)
	
public:
	virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame);
	
};
SLIB_UI_NAMESPACE_END

#endif
