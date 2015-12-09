#ifndef CHECKHEADER_SLIB_MEDIA_DSHOW
#define CHECKHEADER_SLIB_MEDIA_DSHOW

#include "definition.h"

#include "camera.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT DirectShow
{
public:
	static Ref<Camera> createCamera(const CameraParam& param);
	static List<CameraInfo> getCamerasList();
};

SLIB_MEDIA_NAMESPACE_END

#endif
