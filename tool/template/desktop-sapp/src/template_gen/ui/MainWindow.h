#pragma once

#include "../layouts_base.h"

namespace sapp
{
	namespace ui
	{
		SLIB_DECLARE_WINDOW_LAYOUT_BEGIN(MainWindow)
			slib::Ref<slib::LinearView> _linear1;
			slib::Ref<slib::ImageView> _image1;
			slib::Ref<slib::LabelView> _label1;
		SLIB_DECLARE_WINDOW_LAYOUT_END

	}
}
