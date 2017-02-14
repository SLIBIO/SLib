#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/camera.h"

#include "../../../inc/slib/media/dshow.h"

namespace slib
{

	Ref<Camera> Camera::create(const CameraParam& param)
	{
		return DirectShow::createCamera(param);
	}

	List<CameraInfo> Camera::getCamerasList()
	{
		return DirectShow::getCamerasList();
	}

}

#endif
