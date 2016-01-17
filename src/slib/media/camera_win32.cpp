#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/camera.h"
#include "../../../inc/slib/media/dshow.h"

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<Camera> Camera::create(const CameraParam& param)
{
	return DirectShow::createCamera(param);
}

List<CameraInfo> Camera::getCamerasList()
{
	return DirectShow::getCamerasList();
}

SLIB_MEDIA_NAMESPACE_END

#endif
