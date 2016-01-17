#include "../../../inc/slib/media/camera.h"

SLIB_MEDIA_NAMESPACE_BEGIN
CameraInfo::CameraInfo()
{
}

CameraParam::CameraParam()
{
	preferedFrameWidth = 0;
	preferedFrameHeight = 0;
	preferedFrameFormat = bitmapFormat_YUV_NV21;
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

SLIB_MEDIA_NAMESPACE_END

