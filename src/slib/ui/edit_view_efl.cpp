/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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

	namespace priv
	{
		namespace edit_view
		{

			static Elm_Input_Panel_Return_Key_Type ConvertReturnKeyType(UIReturnKeyType type)
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

			static Elm_Input_Panel_Layout ConvertKeyboardType(UIKeyboardType type, sl_bool flagPassword)
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

			static Elm_Autocapital_Type ConvertAutoCapitalizationType(UIAutoCapitalizationType type)
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


			class EditViewHelper : public EditView
			{
			public:
				/*
					type
						0 - EditView
						1 - PasswordView
						2 - TextArea
				*/
				Ref<ViewInstance> createInstance(ViewInstance* _parent, int type);
				
				void applyProperties(Evas_Object* handle, int type)
				{
					if (type == 2) {
						elm_entry_single_line_set(handle, EINA_FALSE);
					} else {
						elm_entry_single_line_set(handle, m_flagMultiLine ? EINA_FALSE : EINA_TRUE);
					}

					pushStyle(handle);

					if (m_text.isNotEmpty()) {
						applyText(handle);
					}
					if (m_hintText.isNotEmpty()) {
						applyPlaceholder(handle);
					}

					elm_entry_editable_set(handle, m_flagReadOnly ? EINA_FALSE : EINA_TRUE);
					elm_entry_password_set(handle, m_flagPassword ? EINA_TRUE : EINA_FALSE);

					elm_entry_input_panel_return_key_type_set(handle, ConvertReturnKeyType(m_returnKeyType));
					elm_entry_input_panel_layout_set(handle, ConvertKeyboardType(m_keyboardType, type == 1));
					elm_entry_autocapital_type_set(handle, ConvertAutoCapitalizationType(m_autoCapitalizationType));

				}

				void pushStyle(Evas_Object* handle)
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
						Alignment ha = m_gravity & Alignment::HorizontalMask;
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
					elm_entry_text_style_user_push(handle, style.getData());
				}

				void setStyle(Evas_Object* handle)
				{
					elm_entry_text_style_user_pop(handle);
					pushStyle(handle);
				}

				void applyText(Evas_Object* handle)
				{
					String text = m_text;
					if (text.isEmpty()) {
						elm_entry_entry_set(handle, "");
						return;
					}
					char* _text = ::elm_entry_utf8_to_markup(text.getData());
					if (_text) {
						elm_entry_entry_set(handle, _text);
						int len = ::strlen(_text);
						elm_entry_select_region_set(handle, len, len);
						free(_text);
					}
				}

				void applyPlaceholder(Evas_Object* handle)
				{
					String text = m_hintText;
					if (text.isEmpty()) {
						elm_object_part_text_set(handle, "guide", "");
						return;
					}

					Ref<Font> font = getHintFont();
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
						Alignment ha = m_hintGravity & Alignment::HorizontalMask;
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
						elm_object_part_text_set(handle, "guide", t.getData());
						free(_text);
					}
				}

			};

			class EditViewInstance : public EFL_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				sl_bool getText(EditView* view, String& _out) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						const char* sz = ::elm_entry_entry_get(handle);
						if (sz) {
							char* t = ::elm_entry_markup_to_utf8(sz);
							if (t) {
								_out = t;
								::free(t);
								return sl_true;
							}
						}
					}
					return sl_false;
				}
				
				void setText(EditView* view, const String& value) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->applyText(handle);
					}
				}
				
				void setGravity(EditView* view, const Alignment& align) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->setStyle(handle);
					}
				}
				
				void setTextColor(EditView* view, const Color& color) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->setStyle(handle);
					}
				}

				void setHintText(EditView* view, const String& value) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->applyPlaceholder(handle);
					}
				}
				
				void setHintGravity(EditView* view, const Alignment& align) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->applyPlaceholder(handle);
					}
				}
				
				void setHintTextColor(EditView* view, const Color& value) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->applyPlaceholder(handle);
					}
				}

				void setHintFont(EditView* view, const Ref<Font>& value) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->applyPlaceholder(handle);
					}
				}

				void setReadOnly(EditView* view, sl_bool flag) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_editable_set(handle, flag ? EINA_FALSE : EINA_TRUE);
					}
				}
				
				void setPassword(EditView* view, sl_bool flag) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_password_set(handle, flag ? EINA_TRUE : EINA_FALSE);
					}
				}

				void setMultiLine(EditView* view, sl_bool flag) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_single_line_set(handle, m_flagMultiLine ? EINA_FALSE : EINA_TRUE);
					}
				}
				
				void setReturnKeyType(EditView* view, UIReturnKeyType type) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_input_panel_return_key_type_set(handle, ConvertReturnKeyType(type));
					}
				}
				
				void setKeyboardType(EditView* view, UIKeyboardType type) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_input_panel_layout_set(handle, ConvertKeyboardType(type, IsInstanceOf<PasswordView>(this)));
					}
				}
				
				void setAutoCapitalizationType(EditView* view, UIAutoCapitalizationType type) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						elm_entry_autocapital_type_set(handle, ConvertAutoCapitalizationType(type));
					}
				}

				void setFont(View* view, const Ref<Font>& font) override
				{
					Evas_Object* handle = m_handle;
					if (handle) {
						(static_cast<EditViewHelper*>(view))->setStyle(handle);
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
				}
				
				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
				}
				
			};

			SLIB_DEFINE_OBJECT(EditViewInstance, EFL_ViewInstance)
			
			static void OnChange(void* data, Evas_Object* obj, void* event_info)
			{
				Ref<EditViewHelper> helper = CastRef<EditViewHelper>(UIPlatform::getView(obj));
				if (helper.isNotNull()) {
					String text;
					const char* s = elm_entry_entry_get(obj);
					if (s) {
						char* t = elm_entry_markup_to_utf8(s);
						if (t) {
							text = t;
							free(t);
						}
					}
					String textNew = text;
					helper->dispatchChange(&textNew);
					if (text != textNew) {
						if (textNew.isEmpty()) {
							elm_entry_entry_set(obj, "");
						} else {
							char* t = elm_entry_utf8_to_markup(textNew.getData());
							if (t) {
								elm_entry_entry_set(obj, t);
								free(t);
							}
						}
					}
				}
			}

			static void OnEnter(void* data, Evas_Object* obj, void* event_info)
			{
				Ref<EditViewHelper> helper = CastRef<EditViewHelper>(UIPlatform::getView(obj));
				if (helper.isNotNull()) {
					helper->dispatchReturnKey();
				}
			}

			Ref<ViewInstance> EditViewHelper::createInstance(ViewInstance* _parent, int type)
			{
				EFL_ViewInstance* parent = static_cast<EFL_ViewInstance*>(_parent);
				if (parent) {
					Evas_Object* handleParent = parent->getHandle();
					if (handleParent) {
						Evas_Object* handle = elm_entry_add(handleParent);
						if (handle) {
							applyProperties(handle, type);
							Ref<EditViewInstance> ret = EFL_ViewInstance::create<EditViewInstance>(this, parent, EFL_ViewType::Generic, handle, sl_true);
							if (ret.isNotNull()) {
								evas_object_smart_callback_add(handle, "changed,user", OnChange, sl_null);
								evas_object_smart_callback_add(handle, "activated", OnEnter, sl_null);
								return ret;
							}
						}
					}
				}
				return sl_null;
			}
			
		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		return (static_cast<EditViewHelper*>(this))->createInstance(parent, 0);
	}
	
	Ptr<IEditViewInstance> EditView::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return (static_cast<EditViewHelper*>(this))->createInstance(parent, 2);
	}
	
	Ptr<IEditViewInstance> TextArea::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}

}

#endif
