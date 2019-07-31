#pragma once

#include "../layouts_base.h"

namespace slib
{
	namespace ui
	{
		SLIB_DECLARE_WINDOW_LAYOUT_BEGIN(TakePhotoFromCameraDialog)
			slib::Ref<slib::CameraView> camera;
		SLIB_DECLARE_WINDOW_LAYOUT_END

	}
}
