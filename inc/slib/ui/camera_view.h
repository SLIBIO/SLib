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
	virtual void start(const CameraParam& param);
	
	virtual void stop();
	
public:
	SLIB_PTR_PROPERTY(IVideoCaptureListener, FrameListener)
	
protected:
	virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame);
	
protected:
	SafeRef<Camera> m_camera;
	
};

SLIB_UI_NAMESPACE_END

#endif
