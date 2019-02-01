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

#include "slib/ui/label_view.h"

#include "slib/graphics/util.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(LabelView, View)
	
	LabelView::LabelView()
	{
		setSavingCanvasState(sl_false);
		
		setUsingFont(sl_true);
		
		m_textAlignment = Alignment::Left;
		m_textColor = Color::Black;
		m_multiLineMode = MultiLineMode::Single;
		m_ellipsizeMode = EllipsizeMode::None;
		
		setPadding(1, 1, 1, 1, UIUpdateMode::Init);
	}
	
	LabelView::~LabelView()
	{
	}
	
	String LabelView::getText()
	{
		return m_text;
	}
	
	void LabelView::setText(const String& text, UIUpdateMode mode)
	{
		m_text = text;
		invalidateLayoutOfWrappingControl(mode);
	}
	
	Color LabelView::getTextColor()
	{
		return m_textColor;
	}
	
	void LabelView::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColor = color;
		invalidate(mode);
	}
	
	Alignment LabelView::getGravity()
	{
		return m_textAlignment;
	}
	
	void LabelView::setGravity(Alignment align, UIUpdateMode mode)
	{
		m_textAlignment = align;
		invalidate(mode);
	}
	
	MultiLineMode LabelView::getMultiLine()
	{
		return m_multiLineMode;
	}
	
	void LabelView::setMultiLine(MultiLineMode multiLineMode, UIUpdateMode updateMode)
	{
		m_multiLineMode = multiLineMode;
		invalidateLayoutOfWrappingControl(updateMode);
	}
	
	EllipsizeMode LabelView::getEllipsize()
	{
		return m_ellipsizeMode;
	}
	
	void LabelView::setEllipsize(EllipsizeMode ellipsizeMode, UIUpdateMode updateMode)
	{
		m_ellipsizeMode = ellipsizeMode;
		invalidate(updateMode);
	}
	
	void LabelView::onDraw(Canvas* canvas)
	{
		m_textBox.draw(canvas, m_text, getFont(), getBoundsInnerPadding(), isWidthWrapping(), m_multiLineMode, m_ellipsizeMode, m_textAlignment, m_textColor);
	}
	
	void LabelView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		sl_ui_pos paddingWidth = getPaddingLeft() + getPaddingRight();
		m_textBox.update(m_text, getFont(), (sl_real)(getLayoutWidth() - paddingWidth), isWidthWrapping(), m_multiLineMode, m_ellipsizeMode, m_textAlignment);
		if (flagHorizontal) {
			setLayoutWidth((sl_ui_pos)(m_textBox.getContentWidth()) + paddingWidth);
		}
		if (flagVertical) {
			setLayoutHeight((sl_ui_pos)(m_textBox.getContentHeight()) + getPaddingTop() + getPaddingBottom());
		}		
	}
	
}
