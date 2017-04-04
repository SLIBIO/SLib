/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/ui/edit_view.h"

#include "view_tizen.h"

namespace slib
{
	class _EditView : public EditView
	{
	public:
		
	};
	
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		return sl_null;
	}
	
	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* _parent)
	{
		return sl_null;
	}
	
	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		return sl_null;
	}
	
	
	void EditView::_getText_NW()
	{
	}
	
	void EditView::_setText_NW(const String& value)
	{
	}
	
	void EditView::_setBorder_NW(sl_bool flag)
	{
	}
	
	void EditView::_setTextAlignment_NW(Alignment align)
	{
	}
	
	void EditView::_setHintText_NW(const String& value)
	{
	}
	
	void EditView::_setHintTextColor_NW(const Color& value)
	{
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
	}
	
	void EditView::_setMultiLine_NW(sl_bool flag)
	{
	}
	
	void EditView::_setTextColor_NW(const Color& color)
	{
	}
	
	void EditView::_setBackgroundColor_NW(const Color& color)
	{
	}
	
	void EditView::_setFont_NW(const Ref<Font>& font)
	{
	}
	
	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
	}
	
	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
	}
	
	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
	}
}

#endif
