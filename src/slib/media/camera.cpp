#include "../../../inc/slib/media/camera.h"

SLIB_MEDIA_NAMESPACE_BEGIN
CameraInfo::CameraInfo()
{
}

CameraParam::CameraParam()
{
	preferedFrameWidth = 0;
	preferedFrameHeight = 0;
	preferedFrameFormat = bitmapFormatYUV_NV21;
}

void CameraParam::setFrontCamera()
{
	deviceId = "FRONT";
}

sl_bool CameraParam::isFrontCamera()
{
	return deviceId == "FRONT";
}

void CameraParam::setBackCamera()
{
	deviceId = "BACK";
}

sl_bool CameraParam::isBackCamera()
{
	return deviceId == "BACK";
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
SLIB_MEDIA_NAMESPACE_END

