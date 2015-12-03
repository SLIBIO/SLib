#include "../../../inc/slib/capture/camera.h"

SLIB_CAPTURE_NAMESPACE_BEGIN
CameraInfo::CameraInfo()
{
}

CameraParam::CameraParam()
{
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
SLIB_CAPTURE_NAMESPACE_END

