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

#include "slib/ui/line_view.h"

#include "slib/ui/core.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace line_view
		{
			
			class StaticContext
			{
			public:
				sl_real defaultThickness;
				Ref<Pen> defaultPen;
				
			public:
				StaticContext()
				{
					defaultThickness = UI::dpToPixel(1);
					if (defaultThickness < 1) {
						defaultThickness = 1;
					}
					defaultPen = Pen::createSolidPen(defaultThickness, Color::Black);
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
			
		}
	}
	
	using namespace priv::line_view;

	SLIB_DEFINE_OBJECT(LineView, View)
	
	LineView::LineView()
	{
		setSavingCanvasState(sl_false);
		
		m_orientation = LayoutOrientation::Horizontal;
		m_style = PenStyle::Solid;
		m_thickness = 1;
		m_color = Color::Black;
		m_gravity = Alignment::Center;
		
		StaticContext* context = GetStaticContext();
		if (context) {
			m_pen = context->defaultPen;
			m_thickness = context->defaultThickness;
		}
	}
	
	LineView::~LineView()
	{
	}

	LayoutOrientation LineView::getOrientation()
	{
		return m_orientation;
	}
	
	void LineView::setOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		if (m_orientation == orientation) {
			return;
		}
		m_orientation = orientation;
		invalidateLayoutOfWrappingControl();
	}
	
	sl_bool LineView::isHorizontal()
	{
		return m_orientation == LayoutOrientation::Horizontal;
	}
	
	void LineView::setHorizontal(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Horizontal, mode);
	}
	
	sl_bool LineView::isVertical()
	{
		return m_orientation == LayoutOrientation::Vertical;
	}
	
	void LineView::setVertical(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Vertical, mode);
	}
	
	Color LineView::getLineColor()
	{
		return m_color;
	}
	
	void LineView::setLineColor(const Color& color, UIUpdateMode mode)
	{
		if (m_color == color) {
			return;
		}
		m_color = color;
		_updatePen();
		invalidate(mode);
	}
	
	sl_real LineView::getLineThickness()
	{
		return m_thickness;
	}
	
	void LineView::setLineThickness(sl_real thickness, UIUpdateMode mode)
	{
		if (thickness < 1) {
			thickness = 1;
		}
		if (m_thickness == thickness) {
			return;
		}
		m_thickness = thickness;
		_updatePen();
		invalidateLayoutOfWrappingControl(mode);
	}
	
	PenStyle LineView::getLineStyle()
	{
		return m_style;
	}
	
	void LineView::setLineStyle(PenStyle style, UIUpdateMode mode)
	{
		if (m_style == style) {
			return;
		}
		m_style = style;
		_updatePen();
		invalidate(mode);
	}
	
	Alignment LineView::getGravity()
	{
		return m_gravity;
	}
	
	void LineView::setGravity(const Alignment& gravity, UIUpdateMode mode)
	{
		if (m_gravity == gravity) {
			return;
		}
		m_gravity = gravity;
		invalidate(mode);
	}
	
	void LineView::onDraw(Canvas* canvas)
	{
		sl_bool flagAntiAliasOriginal = canvas->isAntiAlias();
		if (flagAntiAliasOriginal) {
			canvas->setAntiAlias(sl_false);
		}
		UIRect bounds = getBoundsInnerPadding();
		if (m_orientation == LayoutOrientation::Horizontal) {
			int valign = m_gravity & Alignment::VerticalMask;
			sl_real t = m_thickness / 2;
			sl_real y;
			if (valign == Alignment::Top) {
				if (t <= 1) {
					y = 0;
				} else {
					y = t;
				}
			} else if (valign == Alignment::Bottom) {
				if (t <= 1) {
					y = (sl_real)(bounds.getHeight() - 1);
				} else {
					y = (sl_real)(bounds.getHeight()) - t;
				}
			} else {
				if (t <= 1) {
					y = (sl_real)(bounds.getHeight() / 2);
				} else {
					y = (sl_real)(bounds.getHeight()) / 2 - t;
				}
			}
			y += (sl_real)(bounds.top);
			canvas->drawLine((sl_real)(bounds.left), y, (sl_real)(bounds.right), y, m_pen);
		} else {
			int halign = m_gravity & Alignment::HorizontalMask;
			sl_real t = m_thickness / 2;
			sl_real x;
			if (halign == Alignment::Left) {
				if (t <= 1) {
					x = 0;
				} else {
					x = t;
				}
			} else if (halign == Alignment::Right) {
				if (t <= 1) {
					x = (sl_real)(bounds.getWidth() - 1);
				} else {
					x = (sl_real)(bounds.getWidth()) - t;
				}
			} else {
				if (t <= 1) {
					x = (sl_real)(bounds.getWidth() / 2);
				} else {
					x = (sl_real)(bounds.getWidth()) / 2 - t;
				}
			}
			x += (sl_real)(bounds.left);
			canvas->drawLine(x, (sl_real)(bounds.top), x, (sl_real)(bounds.bottom), m_pen);
		}
		if (flagAntiAliasOriginal) {
			canvas->setAntiAlias(sl_true);
		}
	}
	
	void LineView::onUpdateLayout()
	{
		if (isWidthWrapping()) {
			if (m_orientation == LayoutOrientation::Horizontal) {
				setLayoutWidth(300 + getPaddingLeft() + getPaddingRight());
			} else {
				setLayoutWidth((sl_ui_len)(m_thickness + getPaddingLeft() + getPaddingRight()));
			}
		}
		if (isHeightWrapping()) {
			if (m_orientation == LayoutOrientation::Horizontal) {
				setLayoutHeight((sl_ui_len)(m_thickness + getPaddingTop() + getPaddingBottom()));
			} else {
				setLayoutHeight(300 + getPaddingTop() + getPaddingBottom());
			}
		}
	}
	
	void LineView::_updatePen()
	{
		m_pen = Pen::create(m_style, m_thickness, m_color);
	}
	
	VerticalLineView::VerticalLineView()
	{
		setOrientation(LayoutOrientation::Vertical, UIUpdateMode::Init);
	}
	
	VerticalLineView::~VerticalLineView()
	{
	}

	HorizontalLineView::HorizontalLineView()
	{
		setOrientation(LayoutOrientation::Horizontal, UIUpdateMode::Init);
	}

	HorizontalLineView::~HorizontalLineView()
	{
	}

}
