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

#include "slib/core/variant.h"

#include <Elementary.h>

namespace slib
{

	class EditViewInstance_Impl : public Tizen_ViewInstance
	{
	public:
		EditViewInstance_Impl()
		{
		}

		~EditViewInstance_Impl()
		{
		}


	};

	class EditView_Impl : public EditView
	{
	public:
		/*
			type
				0 - EditView
				1 - PasswordView
				2 - TextArea
		*/
		Ref<ViewInstance> _createInstance(ViewInstance* _parent, int type)
		{
			Tizen_ViewInstance* parent = static_cast<Tizen_ViewInstance*>(_parent);
			if (parent) {
				Evas_Object* handleParent = parent->getHandle();
				if (handleParent) {
					Evas_Object* handle = ::elm_entry_add(handleParent);
					if (handle) {
						_applyProperties(handle, type);
						Ref<Tizen_ViewInstance> ret = Tizen_ViewInstance::create<Tizen_ViewInstance>(this, parent, TizenViewType::Generic, handle, sl_true);
						if (ret.isNotNull()) {
							return ret;
						}
					}
				}
			}
			return sl_null;
		}
		
		void _applyProperties(Evas_Object* handle, int type)
		{
			if (type == 1) {
				::elm_entry_password_set(handle, EINA_TRUE);
			} else if (type == 2) {
				::elm_entry_single_line_set(handle, EINA_FALSE);
			}

			String text = m_text;
			::elm_entry_entry_set(handle, text.getData());

			_pushStyle(handle);

			[handle setTextAlignment:translateAlignment(m_textAlignment)];
			[handle setBorderStyle:(isBorder()?UITextBorderStyleBezel:UITextBorderStyleNone)];
			applyPlaceholder(handle);
			[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
			[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
			[handle setEnabled:(m_flagReadOnly ? NO : YES)];
			Ref<Font> font = getFont();
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
			[handle setReturnKeyType:convertReturnKeyType(m_returnKeyType)];
			[handle setKeyboardType:convertKeyboardType(m_keyboardType)];
		}

		void _pushStyle(Evas_Object* handle)
		{
			Ref<Font> font = getFont();
			char* fontName;
			String strFontName;
			char* fontStyle;
			if (font.isNotNull()) {
				strFontName = font->getFamilyName();
				fontName = strFontName.getData();
				if (font->isBold()) {
					fontStyle = "Bold";
				} else {
					fontStyle = "Light";
				}
			} else {
				fontName = "Tizen";
				fontStyle = "Light";
			}
			char* align;
			{
				Alignment ha = m_textAlignment & Alignment::HorizontalMask;
				if (ha == Alignment::Left) {
					align = "Left";
				} else if (ha == Alignment::Right) {
					align = "Right";
				} else {
					align = "Center";
				}
			}

			String style = String::format("DEFAULT='font=%s:style=%s font_size=%d color=%s align=%s'", fontName, fontStyle, m_textColor.toString(), align);
		}

		void _setStyle(Evas_Object* handle)
		{
			::elm_entry_text_style_user_pop(handle);
			_pushStyle(handle);
		}

	};
	
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 0);
	}
	
	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* _parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 1);
	}
	
	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 2);
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
