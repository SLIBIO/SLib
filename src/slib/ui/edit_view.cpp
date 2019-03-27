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

#include "slib/ui/edit_view.h"

#include "slib/ui/mobile_app.h"
#include "slib/ui/resource.h"
#include "slib/ui/core.h"
#include "slib/ui/button.h"

#include "slib/core/platform_android.h"

namespace slib
{

	/**********************
		EditView
	 ***********************/

	SLIB_DEFINE_OBJECT(EditView, View)

	EditView::EditView()
	{		
		setCreatingNativeWidget(sl_true);
		setUsingFont(sl_true);
		setFocusable(sl_true);
		
		m_textAlignment = Alignment::MiddleCenter;
		m_flagReadOnly = sl_false;
		m_flagPassword = sl_false;
		m_multiLine = MultiLineMode::Single;
		m_textColor = Color::Black;
		m_hintTextColor = Color(180, 180, 180);
		setBorder(sl_true, UIUpdateMode::Init);
		m_returnKeyType = UIReturnKeyType::Default;
		m_keyboardType = UIKeyboardType::Default;
		m_autoCapitalizationType = UIAutoCapitalizationType::None;
		m_flagAutoDismissKeyboard = sl_true;
	}

	EditView::~EditView()
	{
	}

	String EditView::getText()
	{
		if (isNativeWidget()) {
			_getText_NW();
		}
		return m_text;
	}

	void EditView::setText(const String& text, UIUpdateMode mode)
	{
		m_text = text;
		if (isNativeWidget()) {
			_setText_NW(text);
		} else {
			invalidate(mode);
		}
	}

	Alignment EditView::getGravity()
	{
		return m_textAlignment;
	}

	void EditView::setGravity(Alignment align, UIUpdateMode mode)
	{
		m_textAlignment = align;
		if (isNativeWidget()) {
			_setTextAlignment_NW(align);
		} else {
			invalidate(mode);
		}
	}

	String EditView::getHintText()
	{
		return m_hintText;
	}

	void EditView::setHintText(const String& str, UIUpdateMode mode)
	{
		m_hintText = str;
		if (isNativeWidget()) {
			_setHintText_NW(str);
		} else {
			invalidate(mode);
		}
	}

	sl_bool EditView::isReadOnly()
	{
		return m_flagReadOnly;
	}

	void EditView::setReadOnly(sl_bool flag, UIUpdateMode mode)
	{
		m_flagReadOnly = flag;
		if (isNativeWidget()) {
			_setReadOnly_NW(flag);
		} else {
			invalidate(mode);
		}
	}

	sl_bool EditView::isPassword()
	{
		return m_flagPassword;
	}
	
	void EditView::setPassword(sl_bool flag, UIUpdateMode mode)
	{
		m_flagPassword = flag;
		if (isNativeWidget()) {
			_setPassword_NW(flag);
		} else {
			invalidate(mode);
		}
	}

	
	MultiLineMode EditView::getMultiLine()
	{
		return m_multiLine;
	}
	
	void EditView::setMultiLine(MultiLineMode multiLineMode, UIUpdateMode updateMode)
	{
		m_multiLine = multiLineMode;
		if (isNativeWidget()) {
			_setMultiLine_NW(multiLineMode);
		} else {
			invalidate(updateMode);
		}
	}

	Color EditView::getTextColor()
	{
		return m_textColor;
	}

	void EditView::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColor = color;
		if (isNativeWidget()) {
			_setTextColor_NW(color);
		} else {
			invalidate(mode);
		}
	}
	
	Color EditView::getHintTextColor()
	{
		return m_hintTextColor;
	}
	
	void EditView::setHintTextColor(const Color& color, UIUpdateMode mode)
	{
		m_hintTextColor = color;
		if (isNativeWidget()) {
			_setHintTextColor_NW(color);
		} else {
			invalidate(mode);
		}
	}

	UIReturnKeyType EditView::getReturnKeyType()
	{
		return m_returnKeyType;
	}

	void EditView::setReturnKeyType(UIReturnKeyType type)
	{
		m_returnKeyType = type;
		if (isNativeWidget()) {
			_setReturnKeyType_NW(type);
		}
	}

	UIKeyboardType EditView::getKeyboardType()
	{
		return m_keyboardType;
	}

	void EditView::setKeyboardType(UIKeyboardType type)
	{
		m_keyboardType = type;
		if (isNativeWidget()) {
			_setKeyboardType_NW(type);
		}
	}

	void EditView::setAutoCapitalizationType(UIAutoCapitalizationType type)
	{
		m_autoCapitalizationType = type;
		if (isNativeWidget()) {
			_setAutoCapitalizationType_NW(type);
		}
	}

	sl_bool EditView::isAutoDismissKeyboard()
	{
		return m_flagAutoDismissKeyboard;
	}

	void EditView::setAutoDismissKeyboard(sl_bool flag)
	{
		m_flagAutoDismissKeyboard = flag;
	}
	
	void EditView::setFocusNextOnReturnKey()
	{
		setOnReturnKey([](EditView* view) {
			Ref<View> next = view->getNextTabStop();
			if (next.isNotNull()) {
				next->setFocus();
			}
		});
	}

	void EditView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagHorizontal && !flagVertical) {
			return;
		}

		Ref<Font> font = getFont();
		if (flagHorizontal) {
			sl_ui_pos width = getPaddingLeft() + getPaddingRight();
			if (font.isNotNull()) {
				sl_ui_pos t = (sl_ui_pos)(font->getFontHeight());
				if (t > 0) {
					width += t * 4;
				}
			}
			if (width < 0) {
				width = 0;
			}
			setLayoutWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = 0;
			if (font.isNotNull()) {
				height = (sl_ui_pos)(font->getFontHeight() * 1.5f);
				if (height < 0) {
					height = 0;
				}
			}
			height += getPaddingTop() + getPaddingBottom();
			if (height < 0) {
				height = 0;
			}
			setLayoutHeight(height);
		}
	}
	
	void EditView::onDraw(Canvas* canvas)
	{
		if (m_text.isEmpty()) {
			canvas->drawText(m_hintText, getBoundsInnerPadding(), getFont(), m_hintTextColor, m_textAlignment);
		} else {
			if (m_flagPassword) {
				canvas->drawText(String('*', m_text.getLength()), getBoundsInnerPadding(), getFont(), m_textColor, m_textAlignment);
			} else {
				canvas->drawText(m_text, getBoundsInnerPadding(), getFont(), m_textColor, m_textAlignment);
			}
		}
	}
	
	void EditView::onClickEvent(UIEvent* ev)
	{
#if defined(SLIB_PLATFORM_IS_MOBILE)
		if (!m_flagReadOnly) {
			m_windowEdit = new Window;
			m_windowEdit->setBackgroundColor(Color::White);
			if (IsInstanceOf<PasswordView>(this)) {
				m_editViewNative = new PasswordView;
			} else {
#if defined(SLIB_UI_IS_IOS)
				m_editViewNative = new TextArea;
#else
				m_editViewNative = new EditView;
#endif
			}
			m_editViewNative->setText(m_text, UIUpdateMode::Init);
			m_editViewNative->setWidthFilling(1, UIUpdateMode::Init);
			m_editViewNative->setHeightFilling(1, UIUpdateMode::Init);
			m_editViewNative->setMargin(UIResource::getScreenMinimum()/20);
			m_editViewNative->setBorder(sl_false, UIUpdateMode::Init);
			m_editViewNative->setGravity(Alignment::TopLeft, UIUpdateMode::Init);
			m_editViewNative->setMultiLine(getMultiLine(), UIUpdateMode::Init);
			m_editViewNative->setOnChange(SLIB_FUNCTION_WEAKREF(EditView, _onChangeEditViewNative, this));
			m_editViewNative->setOnReturnKey(SLIB_FUNCTION_WEAKREF(EditView, _onReturnKeyEditViewNative, this));
			if (m_returnKeyType == UIReturnKeyType::Default && m_multiLine == MultiLineMode::Single) {
				m_editViewNative->setReturnKeyType(UIReturnKeyType::Done);
			} else {
				m_editViewNative->setReturnKeyType(m_returnKeyType);
			}
			m_editViewNative->setKeyboardType(m_keyboardType);
			m_editViewNative->setAutoCapitalizationType(m_autoCapitalizationType);
			m_editViewNative->setFont(Font::create(getFontFamily(), UIResource::getScreenMinimum()/20));
			m_windowEdit->addView(m_editViewNative);
			m_windowEdit->create();
			m_windowEdit->setOnClose(SLIB_FUNCTION_WEAKREF(EditView, _onCloseWindowEditViewNative, this));
			m_editViewNative->setFocus();

			sl_bool flagDoneButton = m_multiLine != MultiLineMode::Single;
#if defined(SLIB_UI_IS_ANDROID)
			UI::dispatchToUiThread([] {
				Android::showKeyboard();
			}, 500);
			flagDoneButton = sl_true;
#endif
			if (flagDoneButton) {
				sl_ui_pos sw = UIResource::getScreenMinimum();
				m_editViewNative->setMarginRight(sw / 5 - sw / 20);
				Ref<Button> btnDone = new Button;
				btnDone = new Button;
				btnDone->setText("Done");
				btnDone->setWidth(sw / 5);
				btnDone->setMargin(sw / 20);
				btnDone->setMarginRight(sw / 40);
				btnDone->setHeight(sw / 10);
				btnDone->setFont(Font::create(getFontFamily(), sw/20));
				btnDone->setAlignParentRight();
				btnDone->setOnClick(SLIB_FUNCTION_WEAKREF(EditView, _onDoneEditViewNativeButton, this));
				m_windowEdit->addView(btnDone);
			}
		}
#endif
	}
	
	void EditView::_onChangeEditViewNative(EditView* ev, String* text)
	{
		dispatchChange(text);
		if (m_multiLine == MultiLineMode::Single) {
			sl_reg index = ParseUtil::indexOfLine(*text);
			if (index >= 0) {
				*text = text->mid(0, index);
			}
		}
	}
	
	void EditView::_onReturnKeyEditViewNative(EditView* ev)
	{
		if (m_multiLine == MultiLineMode::Single) {
			_onDoneEditViewNativeButton(sl_null);
		}
		dispatchReturnKey();
	}
	
	void EditView::_onDoneEditViewNativeButton(View* view)
	{
		m_windowEdit->close();
		m_windowEdit.setNull();
		m_editViewNative.setNull();
		invalidate();
#if defined(SLIB_PLATFORM_IS_ANDROID)
		Android::dismissKeyboard();
#endif
	}

	void EditView::_onCloseWindowEditViewNative(Window* window, UIEvent* ev)
	{
		m_windowEdit.setNull();
		m_editViewNative.setNull();
		invalidate();
		_onDoneEditViewNativeButton(sl_null);
		dispatchReturnKey();
	}

	SLIB_DEFINE_EVENT_HANDLER(EditView, Change, String* value)

	void EditView::dispatchChange(String* value)
	{
		SLIB_INVOKE_EVENT_HANDLER(Change, value)
		m_text = *value;
	}

	SLIB_DEFINE_EVENT_HANDLER(EditView, ReturnKey)

	void EditView::dispatchReturnKey()
	{
		SLIB_INVOKE_EVENT_HANDLER(ReturnKey)
	}
	
	void EditView::dispatchKeyEvent(UIEvent* ev)
	{
		if (m_multiLine == MultiLineMode::Single || ev->getKeycode() == Keycode::Escape) {
			if (ev->getAction() == UIAction::KeyDown) {
				if (ev->getKeycode() == Keycode::Enter) {
					dispatchReturnKey();
				}
			}
			View::dispatchKeyEvent(ev);
		} else {
			ev->stopPropagation();
		}
	}

	/**********************
		PasswordView
	***********************/

	PasswordView::PasswordView()
	{
		m_flagPassword = sl_true;
	}

	/**********************
		TextArea
	***********************/

	SLIB_DEFINE_OBJECT(TextArea, EditView)

	TextArea::TextArea()
	{
		m_multiLine = MultiLineMode::Multiple;
		m_flagAutoDismissKeyboard = sl_false;
		m_textAlignment = Alignment::TopLeft;
		setReturnKeyType(UIReturnKeyType::Return);
		setScrolling(sl_true, sl_true, UIUpdateMode::Init);
	}

	TextArea::~TextArea()
	{
	}

#if !defined(SLIB_UI)
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}

	void EditView::_getText_NW()
	{
	}

	void EditView::_setText_NW(const String& text)
	{
	}

	void EditView::_setTextAlignment_NW(Alignment align)
	{
	}

	void EditView::_setHintText_NW(const String& str)
	{
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
	}

	void EditView::_setPassword_NW(sl_bool flag)
	{
	}
	
	void EditView::_setMultiLine_NW(MultiLineMode mode)
	{
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
	}
	
	void EditView::_setHintTextColor_NW(const Color& color)
	{
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
	}

	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
#endif

#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID) && !defined(SLIB_UI_IS_EFL)
	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
	}

	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
	}

	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
	}
#endif

}
