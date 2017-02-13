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
