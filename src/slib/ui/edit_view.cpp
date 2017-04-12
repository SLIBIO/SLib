/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/edit_view.h"

#include "slib/ui/mobile_app.h"
#include "slib/ui/scroll_view.h"
#include "slib/ui/resource.h"

namespace slib
{

/**********************
	EditView
 ***********************/

	SLIB_DEFINE_OBJECT(EditView, View)

	EditView::EditView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setCreatingNativeWidget(sl_true);
		setUsingFont(sl_true);
		setFocusable(sl_true);
		m_textAlignment = Alignment::MiddleCenter;
		m_flagReadOnly = sl_false;
		m_flagMultiLine = sl_false;
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool EditView::isMultiLine()
	{
		return m_flagMultiLine;
	}

	void EditView::setMultiLine(sl_bool flag, UIUpdateMode mode)
	{
		m_flagMultiLine = flag;
		if (isNativeWidget()) {
			_setMultiLine_NW(flag);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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

	void EditView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!flagHorizontal && !flagVertical) {
			return;
		}

		if (flagHorizontal) {
			sl_ui_pos width = getPaddingLeft() + getPaddingRight();
			if (width < 0) {
				width = 0;
			}
			setMeasuredWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = 0;
			Ref<Font> font = getFont();
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
			setMeasuredHeight(height);
		}
	}
	
	void EditView::onDraw(Canvas* canvas)
	{
		if (m_text.isEmpty()) {
			canvas->drawText(m_hintText, getBoundsInnerPadding(), getFont(), m_hintTextColor, m_textAlignment);
		} else {
			canvas->drawText(m_text, getBoundsInnerPadding(), getFont(), m_textColor, m_textAlignment);
		}
	}
	
	void EditView::onClick(UIEvent* ev)
	{
#if defined(SLIB_PLATFORM_IS_MOBILE)
		if (!m_flagReadOnly) {
			m_windowEdit = new Window;
			m_windowEdit->setBackgroundColor(Color::White);
			m_editViewNative = new TextArea;
			m_editViewNative->setText(m_text, UIUpdateMode::Init);
			m_editViewNative->setSizeFilling(sl_true, sl_true, UIUpdateMode::Init);
			m_editViewNative->setMargin(UIResource::getScreenMinimum()/20);
			m_editViewNative->setBorder(sl_false, UIUpdateMode::Init);
			m_editViewNative->setGravity(Alignment::TopLeft, UIUpdateMode::Init);
			m_editViewNative->setMultiLine(m_flagMultiLine, UIUpdateMode::Init);
			m_editViewNative->setOnChange(SLIB_FUNCTION_WEAKREF(EditView, _onChangeEditViewNative, this));
			m_editViewNative->setOnReturnKey(SLIB_FUNCTION_WEAKREF(EditView, _onReturnKeyEditViewNative, this));
			m_editViewNative->setOnDoneEdit(SLIB_FUNCTION_WEAKREF(EditView, _onDoneEditViewNative, this));
			if (m_returnKeyType == UIReturnKeyType::Default && !m_flagMultiLine) {
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
		}
#endif
	}
	
	String EditView::_onChangeEditViewNative(EditView* ev, const String& text)
	{
		String _text = dispatchChange(text);
		if (!m_flagMultiLine) {
			sl_reg index = ParseUtil::indexOfLine(_text);
			if (index >= 0) {
				_text = _text.mid(0, index);
			}
		}
		m_text = _text;
		return _text;
	}
	
	void EditView::_onReturnKeyEditViewNative(EditView* ev)
	{
		dispatchReturnKey();
		if (!m_flagMultiLine) {
			_onDoneEditViewNative(ev);
		}
	}
	
	void EditView::_onDoneEditViewNative(EditView* ev)
	{
		m_windowEdit->close();
		m_windowEdit.setNull();
		m_editViewNative.setNull();
		invalidate();
		dispatchDoneEdit();
	}
	
	void EditView::_onCloseWindowEditViewNative(Window* window, UIEvent* ev)
	{
		m_windowEdit.setNull();
		m_editViewNative.setNull();
		invalidate();
		dispatchDoneEdit();
	}

	String EditView::onChange(const String& newValue)
	{
		return newValue;
	}

	void EditView::onReturnKey()
	{
	}
	
	void EditView::onDoneEdit()
	{
	}
	
	void EditView::dispatchKeyEvent(UIEvent* ev)
	{
		View::dispatchKeyEvent(ev);
		if (!(isMultiLine())) {
			if (ev->getAction() == UIAction::KeyDown) {
				if (ev->getKeycode() == Keycode::Enter) {
					dispatchReturnKey();
				}
			}
		}
	}

	String EditView::dispatchChange(const String& newValue)
	{
		String value = onChange(newValue);
		Function<String(EditView*, String)> callback = getOnChange();
		if (callback.isNotNull()) {
			value = callback(this, value);
		}
		return value;
	}

	void EditView::dispatchReturnKey()
	{
		onReturnKey();
		getOnReturnKey()(this);
	}
	
	void EditView::dispatchDoneEdit()
	{
		onDoneEdit();
		getOnDoneEdit()(this);
	}

/**********************
	PasswordView
***********************/

	SLIB_DEFINE_OBJECT(PasswordView, EditView)

	PasswordView::PasswordView()
	{
	}

	PasswordView::~PasswordView()
	{
	}

	sl_bool PasswordView::isMultiLine()
	{
		return sl_false;
	}

	void PasswordView::setMultiLine(sl_bool flag, UIUpdateMode mode)
	{
	}

/**********************
	TextArea
***********************/

	SLIB_DEFINE_OBJECT(TextArea, EditView)

	TextArea::TextArea()
	{
		m_flagMultiLine = sl_true;
		setReturnKeyType(UIReturnKeyType::Return);
	}

	TextArea::~TextArea()
	{
	}

	sl_bool TextArea::isMultiLine()
	{
		return sl_true;
	}

	void TextArea::setMultiLine(sl_bool flag, UIUpdateMode mode)
	{
	}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID)) && !(defined(SLIB_PLATFORM_IS_TIZEN))

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

	void EditView::_setMultiLine_NW(sl_bool flag)
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

	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}

#endif


#if !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_ANDROID)) && !(defined(SLIB_PLATFORM_IS_TIZEN))

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
