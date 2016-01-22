#include "../../../inc/slib/ui/camera_view.h"

SLIB_UI_NAMESPACE_BEGIN

CameraView::CameraView()
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

SLIB_UI_NAMESPACE_END
