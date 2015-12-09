#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/media/camera.h"
#include "../../../inc/slib/media/avfoundation.h"

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<Camera> Camera::create(const CameraParam& param)
{
	return AVFoundation::createCamera(param);
}

List<CameraInfo> Camera::getCamerasList()
{
	return AVFoundation::getCamerasList();
}

SLIB_MEDIA_NAMESPACE_END

#endif
