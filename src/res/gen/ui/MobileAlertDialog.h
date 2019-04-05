#pragma once

#include "../layouts_base.h"

namespace slib
{
	namespace ui
	{
		SLIB_DECLARE_PAGE_LAYOUT_BEGIN(MobileAlertDialog)
			slib::Ref<slib::LinearView> _linear1;
			slib::Ref<slib::LabelView> txtTitle;
			slib::Ref<slib::ScrollView> _scroll1;
			slib::Ref<slib::LabelView> txtContent;
			slib::Ref<slib::View> _view1;
			slib::Ref<slib::LinearView> _linear2;
			slib::Ref<slib::Button> btnOK;
			slib::Ref<slib::View> _view2;
			slib::Ref<slib::Button> btnYes;
			slib::Ref<slib::View> _view3;
			slib::Ref<slib::Button> btnNo;
			slib::Ref<slib::View> _view4;
			slib::Ref<slib::Button> btnCancel;
			slib::Ref<slib::View> _view5;
		SLIB_DECLARE_PAGE_LAYOUT_END

	}
}
