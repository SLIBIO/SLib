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
