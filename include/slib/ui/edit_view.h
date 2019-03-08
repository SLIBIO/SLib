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

#ifndef CHECKHEADER_SLIB_UI_EDIT_VIEW
#define CHECKHEADER_SLIB_UI_EDIT_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{
	
	class SLIB_EXPORT EditView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		EditView();

		~EditView();
		
	public:
		String getText();
		
		virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getHintText();
		
		virtual void setHintText(const String& str, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isReadOnly();
		
		virtual void setReadOnly(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isPassword();
		
		virtual void setPassword(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		MultiLineMode getMultiLine();
		
		virtual void setMultiLine(MultiLineMode multiLineMode, UIUpdateMode updateMode = UIUpdateMode::UpdateLayout);
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getHintTextColor();
		
		virtual void setHintTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		UIReturnKeyType getReturnKeyType();
		
		void setReturnKeyType(UIReturnKeyType type);
		
		UIKeyboardType getKeyboardType();
		
		void setKeyboardType(UIKeyboardType type);
		
		void setAutoCapitalizationType(UIAutoCapitalizationType type);
		
		UIAutoCapitalizationType getAutoCaptializationType();
		
		sl_bool isAutoDismissKeyboard();
		
		void setAutoDismissKeyboard(sl_bool flag);
		
		void setFocusNextOnReturnKey();
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(EditView, Change, String* pValue);
		SLIB_DECLARE_EVENT_HANDLER(EditView, ReturnKey)
		
	protected:
		void onUpdateLayout() override;
		
		void onDraw(Canvas* canvas) override;

		void onClickEvent(UIEvent* ev) override;
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		void dispatchKeyEvent(UIEvent* ev) override;
		
	private:
		void _getText_NW();
		
		void _setText_NW(const String& text);
		
		void _setTextAlignment_NW(Alignment align);
		
		void _setHintText_NW(const String& str);
		
		void _setReadOnly_NW(sl_bool flag);
		
		void _setPassword_NW(sl_bool flag);

		void _setMultiLine_NW(MultiLineMode mode);
		
		void _setTextColor_NW(const Color& color);
		
		void _setHintTextColor_NW(const Color& color);
		
		void _setReturnKeyType_NW(UIReturnKeyType type);
		
		void _setKeyboardType_NW(UIKeyboardType type);
		
		void _setAutoCapitalizationType_NW(UIAutoCapitalizationType type);
		
		void _setFont_NW(const Ref<Font>& font) override;
		
		void _setBorder_NW(sl_bool flag) override;
		
		void _setBackgroundColor_NW(const Color& color) override;
		
		void _setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical) override;

		void _onChangeEditViewNative(EditView* ev, String* text);
		
		void _onReturnKeyEditViewNative(EditView* ev);
		
		void _onDoneEditViewNativeButton(View* view);

		void _onCloseWindowEditViewNative(Window* window, UIEvent* ev);

	protected:
		AtomicString m_text;
		Alignment m_textAlignment;
		AtomicString m_hintText;
		sl_bool m_flagReadOnly;
		sl_bool m_flagPassword;
		MultiLineMode m_multiLine;
		Color m_textColor;
		Color m_hintTextColor;
		UIReturnKeyType m_returnKeyType;
		UIKeyboardType m_keyboardType;
		UIAutoCapitalizationType m_autoCapitalizationType;
		sl_bool m_flagAutoDismissKeyboard;
		
		Ref<Window> m_windowEdit;
		Ref<EditView> m_editViewNative;
		
	};
	
	class PasswordView : public EditView
	{
	public:
		PasswordView();
		
	};
	
	class TextArea : public EditView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextArea();
		
		~TextArea();

	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	};

}

#endif
