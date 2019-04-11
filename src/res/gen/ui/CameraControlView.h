#pragma once

#include "../layouts_base.h"

namespace slib
{
	namespace ui
	{
		SLIB_DECLARE_VIEW_LAYOUT_BEGIN(CameraControlView)
			slib::Ref<slib::Button> btnShutter;
			slib::Ref<slib::Button> btnSwitch;
			slib::Ref<slib::Button> btnFlash;
		SLIB_DECLARE_VIEW_LAYOUT_END

	}
}
