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
#include "slib/ui/core.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(LabelView, View)
	
	LabelView::LabelView()
	{
		setSavingCanvasState(sl_false);
		
		setUsingFont(sl_true);
		
		m_flagHyperText = sl_false;
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
	
	sl_bool LabelView::isHyperText()
	{
		return m_flagHyperText;
	}
	
	void LabelView::setText(const String& text, UIUpdateMode mode)
	{
		m_text = text;
		m_flagHyperText = sl_false;
		invalidateLayoutOfWrappingControl(mode);
	}
	
	void LabelView::setHyperText(const String& text, UIUpdateMode mode)
	{
		m_text = text;
		m_flagHyperText = sl_true;
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
	
	
	SLIB_DEFINE_EVENT_HANDLER(LabelView, ClickLink, const String& href, UIEvent* ev)
	
	void LabelView::dispatchClickLink(const String& href, UIEvent *ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(ClickLink, href, ev)
		if (ev->isPreventedDefault()) {
			return;
		}
		UI::openUrl(href);
	}
	
	
	void LabelView::onDraw(Canvas* canvas)
	{
		m_textBox.draw(canvas, m_text, m_flagHyperText, getFont(), getBoundsInnerPadding(), m_multiLineMode, m_ellipsizeMode, m_textAlignment, m_textColor);
	}
	
	void LabelView::onClickEvent(UIEvent* ev)
	{
		Ref<TextItem> item = m_textBox.getTextItemAtPosition(ev->getX(), ev->getY());
		if (item.isNotNull()) {
			Ref<TextStyle> style = item->getStyle();
			if (style.isNotNull()) {
				String href = style->href;
				if (href.isNotEmpty()) {
					dispatchClickLink(href, ev);
				}
			}
		}
	}
	
	void LabelView::onSetCursor(UIEvent* ev)
	{
		Ref<TextItem> item = m_textBox.getTextItemAtPosition(ev->getX(), ev->getY());
		if (item.isNotNull()) {
			Ref<TextStyle> style = item->getStyle();
			if (style.isNotNull()) {
				String href = style->href;
				if (href.isNotEmpty()) {
					setCursor(Cursor::getHand());
					return;
				}
			}
		}
		setCursor(Cursor::getArrow());
	}

	void LabelView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		sl_ui_pos paddingWidth = getPaddingLeft() + getPaddingRight();
		sl_ui_len width;
		if (isWidthWrapping()) {
			if (isMaximumWidthDefined()) {
				width = getMaximumWidth() - paddingWidth;
			} else {
				width = 0;
			}
		} else {
			width = getLayoutWidth() - paddingWidth;
		}
		
		m_textBox.update(m_text, m_flagHyperText, getFont(), (sl_real)width, m_multiLineMode, m_ellipsizeMode, m_textAlignment);
		if (flagHorizontal) {
			width = (sl_ui_pos)(m_textBox.getContentWidth());
			m_textBox.setWidth(width);
			setLayoutWidth(width + paddingWidth);
		}
		if (flagVertical) {
			setLayoutHeight((sl_ui_pos)(m_textBox.getContentHeight()) + getPaddingTop() + getPaddingBottom());
		}		
	}
	
}
