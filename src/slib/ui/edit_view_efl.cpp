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

#if defined(SLIB_UI_IS_EFL)

#include "slib/ui/edit_view.h"

#include "view_efl.h"

#include "slib/core/variant.h"
#include "slib/core/xml.h"

#include <Elementary.h>

#if defined(SLIB_PLATFORM_IS_TIZEN)
#	define SLIB_EFL_DEFAULT_FONT_NAME "Tizen"
#else
#	define SLIB_EFL_DEFAULT_FONT_NAME "Arial"
#endif

namespace slib
{

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
			EFL_ViewInstance* parent = static_cast<EFL_ViewInstance*>(_parent);
			if (parent) {
				Evas_Object* handleParent = parent->getHandle();
				if (handleParent) {
					Evas_Object* handle = ::elm_entry_add(handleParent);
					if (handle) {
						_applyProperties(handle, type);
						Ref<EFL_ViewInstance> ret = EFL_ViewInstance::create<EFL_ViewInstance>(this, parent, EFL_ViewType::Generic, handle, sl_true);
						if (ret.isNotNull()) {
							::evas_object_smart_callback_add(handle, "changed,user", _onChange, sl_null);
							::evas_object_smart_callback_add(handle, "activated", _onEnter, sl_null);
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
			} else {
				::elm_entry_single_line_set(handle, m_flagMultiLine ? EINA_FALSE : EINA_TRUE);
			}

			_pushStyle(handle);

			if (m_text.isNotEmpty()) {
				_applyText(handle);
			}
			if (m_hintText.isNotEmpty()) {
				_applyPlaceholder(handle);
			}

			::elm_entry_editable_set(handle, m_flagReadOnly ? EINA_FALSE : EINA_TRUE);

			::elm_entry_input_panel_return_key_type_set(handle, _convertReturnKeyType(m_returnKeyType));
			::elm_entry_input_panel_layout_set(handle, _convertKeyboardType(m_keyboardType, type == 1));
			::elm_entry_autocapital_type_set(handle, _convertAutoCapitalizationType(m_autoCapitalizationType));

		}

		void _pushStyle(Evas_Object* handle)
		{
			Ref<Font> font = getFont();
			const char* fontName;
			String strFontName;
			const char* fontWeight;
			const char* fontStyle;
			const char* underline;
			const char* strikethrough;
			sl_int32 fontSize;
			if (font.isNotNull()) {
				strFontName = font->getFamilyName();
				fontName = strFontName.getData();
				if (font->isBold()) {
					fontWeight = "Bold";
				} else {
					fontWeight = "Light";
				}
				if (font->isItalic()) {
					fontStyle = "Italic";
				} else {
					fontStyle = "Normal";
				}
				if (font->isUnderline()) {
					underline = "on";
				} else {
					underline = "off";
				}
				if (font->isStrikeout()) {
					strikethrough = "on";
				} else {
					strikethrough = "off";
				}
				fontSize = (sl_int32)(font->getSize());
			} else {
				fontName = SLIB_EFL_DEFAULT_FONT_NAME;
				fontWeight = "Light";
				fontStyle = "Normal";
				underline = "off";
				strikethrough = "off";
				fontSize = 12;
			}
			const char* align;
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
			String style = String::format("DEFAULT='font=%s font_weight=%s font_style=%s underline=%s strikethrough=%s font_size=%d color=#%02x%02x%02x%02x align=%s'",
					fontName, fontWeight, fontStyle, underline, strikethrough, fontSize, m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a, align);
			::elm_entry_text_style_user_push(handle, style.getData());
		}

		void _setStyle(Evas_Object* handle)
		{
			::elm_entry_text_style_user_pop(handle);
			_pushStyle(handle);
		}

		void _applyText(Evas_Object* handle)
		{
			String text = m_text;
			if (text.isEmpty()) {
				::elm_entry_entry_set(handle, "");
				return;
			}
			char* _text = ::elm_entry_utf8_to_markup(text.getData());
			if (_text) {
				::elm_entry_entry_set(handle, _text);
				int len = ::strlen(_text);
				::elm_entry_select_region_set(handle, len, len);
				::free(_text);
			}
		}

		void _applyPlaceholder(Evas_Object* handle)
		{
			String text = m_hintText;
			if (text.isEmpty()) {
				::elm_object_part_text_set(handle, "guide", "");
				return;
			}

			Ref<Font> font = getFont();
			const char* fontName;
			String strFontName;
			const char* fontWeight;
			const char* fontStyle;
			sl_int32 fontSize;
			if (font.isNotNull()) {
				strFontName = font->getFamilyName();
				fontName = strFontName.getData();
				if (font->isBold()) {
					fontWeight = "Bold";
				} else {
					fontWeight = "Light";
				}
				if (font->isItalic()) {
					fontStyle = "Italic";
				} else {
					fontStyle = "Normal";
				}
				fontSize = (sl_int32)(font->getSize());
			} else {
				fontName = SLIB_EFL_DEFAULT_FONT_NAME;
				fontWeight = "Light";
				fontStyle = "Normal";
				fontSize = 12;
			}
			const char* align;
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
			char* _text = ::elm_entry_utf8_to_markup(text.getData());
			if (_text) {
				String t = String::format("<font=%s><font_weight=%s><font_style=%s><font_size=%d><color=#%02x%02x%02x%02x><align=%s>%s</align></color></font_size></font_weight></font>",
						fontName, fontWeight, fontStyle, fontSize, m_hintTextColor.r, m_hintTextColor.g, m_hintTextColor.b, m_hintTextColor.a, align, _text);
				::elm_object_part_text_set(handle, "guide", t.getData());
				::free(_text);
			}
		}

		static Elm_Input_Panel_Return_Key_Type _convertReturnKeyType(UIReturnKeyType type)
		{
			switch (type) {
			case UIReturnKeyType::Done:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_DONE;
			case UIReturnKeyType::Search:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_SEARCH;
			case UIReturnKeyType::Next:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_NEXT;
			case UIReturnKeyType::Go:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_GO;
			case UIReturnKeyType::Send:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_SEND;
			case UIReturnKeyType::Join:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_JOIN;
			case UIReturnKeyType::Route:
			case UIReturnKeyType::EmergencyCall:
			case UIReturnKeyType::Google:
			case UIReturnKeyType::Yahoo:
			case UIReturnKeyType::Return:
			case UIReturnKeyType::Continue:
			default:
				return ELM_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
			}
		}

		static Elm_Input_Panel_Layout _convertKeyboardType(UIKeyboardType type, sl_bool flagPassword)
		{
			switch (type) {
			case UIKeyboardType::Numpad:
				return ELM_INPUT_PANEL_LAYOUT_NUMBERONLY;
			case UIKeyboardType::Phone:
				return ELM_INPUT_PANEL_LAYOUT_PHONENUMBER;
			case UIKeyboardType::Email:
				return ELM_INPUT_PANEL_LAYOUT_EMAIL;
			case UIKeyboardType::Decimal:
				return ELM_INPUT_PANEL_LAYOUT_IP;
			case UIKeyboardType::Url:
				return ELM_INPUT_PANEL_LAYOUT_URL;
			case UIKeyboardType::NumbersAndPunctuation:
				return ELM_INPUT_PANEL_LAYOUT_NUMBER;
			case UIKeyboardType::Alphabet:
			case UIKeyboardType::WebSearch:
			case UIKeyboardType::Twitter:
			case UIKeyboardType::NamePhone:
			case UIKeyboardType::Ascii:
			case UIKeyboardType::AsciiNumpad:
			case UIKeyboardType::Default:
			default:
				if (flagPassword) {
					return ELM_INPUT_PANEL_LAYOUT_PASSWORD;
				} else {
					return ELM_INPUT_PANEL_LAYOUT_NORMAL;
				}
			}
		}

		static Elm_Autocapital_Type _convertAutoCapitalizationType(UIAutoCapitalizationType type)
		{
			switch (type) {
			case UIAutoCapitalizationType::None:
				return ELM_AUTOCAPITAL_TYPE_NONE;
			case UIAutoCapitalizationType::Words:
				return ELM_AUTOCAPITAL_TYPE_WORD;
			case UIAutoCapitalizationType::Sentences:
				return ELM_AUTOCAPITAL_TYPE_SENTENCE;
			case UIAutoCapitalizationType::AllCharacters:
			default:
				return ELM_AUTOCAPITAL_TYPE_ALLCHARACTER;
			}
		}

		static void _onChange(void* data, Evas_Object* obj, void* event_info)
		{
			Ref<View> _view = UIPlatform::getView(obj);
			EditView_Impl* view = CastInstance<EditView_Impl>(_view.get());
			if (view) {
				String text;
				const char* s = ::elm_entry_entry_get(obj);
				if (s) {
					char* t = ::elm_entry_markup_to_utf8(s);
					if (t) {
						text = t;
						::free(t);
					}
				}
				String textNew = view->dispatchChange(text);
				if (text != textNew) {
					if (textNew.isEmpty()) {
						::elm_entry_entry_set(obj, "");
					} else {
						char* t = ::elm_entry_utf8_to_markup(textNew.getData());
						if (t) {
							::elm_entry_entry_set(obj, t);
							::free(t);
						}
					}
				}
			}
		}

		static void _onEnter(void* data, Evas_Object* obj, void* event_info)
		{
			Ref<View> _view = UIPlatform::getView(obj);
			EditView_Impl* view = CastInstance<EditView_Impl>(_view.get());
			if (view) {
				view->dispatchReturnKey();
			}
		}

	};
	
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 0);
	}
	
	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 1);
	}
	
	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 2);
	}
	
	
	void EditView::_getText_NW()
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			const char* sz = ::elm_entry_entry_get(handle);
			if (sz) {
				char* t = ::elm_entry_markup_to_utf8(sz);
				if (t) {
					m_text = t;
					::free(t);
				} else {
					m_text.setNull();
				}
			} else {
				m_text.setNull();
			}
		}
	}
	
	void EditView::_setText_NW(const String& value)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_applyText(handle);
		}
	}
	
	void EditView::_setBorder_NW(sl_bool flag)
	{
	}
	
	void EditView::_setTextAlignment_NW(Alignment align)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_setStyle(handle);
		}
	}
	
	void EditView::_setHintText_NW(const String& value)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_applyPlaceholder(handle);
		}
	}
	
	void EditView::_setHintTextColor_NW(const Color& value)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_setStyle(handle);
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::elm_entry_editable_set(handle, flag ? EINA_FALSE : EINA_TRUE);
		}
	}
	
	void EditView::_setMultiLine_NW(sl_bool flag)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::elm_entry_single_line_set(handle, m_flagMultiLine ? EINA_FALSE : EINA_TRUE);
		}
	}
	
	void EditView::_setTextColor_NW(const Color& color)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_setStyle(handle);
		}
	}
	
	void EditView::_setBackgroundColor_NW(const Color& color)
	{
	}
	
	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((EditView_Impl*)this)->_setStyle(handle);
		}
	}
	
	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::elm_entry_input_panel_return_key_type_set(handle, EditView_Impl::_convertReturnKeyType(type));
		}
	}
	
	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::elm_entry_input_panel_layout_set(handle, EditView_Impl::_convertKeyboardType(type, IsInstanceOf<PasswordView>(this)));
		}
	}
	
	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
		Evas_Object* handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::elm_entry_autocapital_type_set(handle, EditView_Impl::_convertAutoCapitalizationType(type));
		}
	}

}

#endif
