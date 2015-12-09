#include "../../../inc/slib/core/definition.h"

#if !defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/media/avfoundation.h"
#include "../../../inc/slib/media/camera.h"

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<Camera> AVFoundation::createCamera(const CameraParam& param)
{
	return Ref<Camera>::null();
}

List<CameraInfo> AVFoundation::getCamerasList()
{
	return List<CameraInfo>::null();
}

SLIB_MEDIA_NAMESPACE_END

#endif
