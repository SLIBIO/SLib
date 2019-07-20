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

#ifndef CHECKHEADER_SLIB_UI_LINE_VIEW
#define CHECKHEADER_SLIB_UI_LINE_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT LineView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		LineView();
		
		~LineView();

	public:
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHorizontal();
		
		void setHorizontal(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isVertical();
		
		void setVertical(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Color getLineColor();
		
		void setLineColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getLineThickness();
		
		void setLineThickness(sl_real thickness, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		PenStyle getLineStyle();
		
		void setLineStyle(PenStyle style, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		void setGravity(const Alignment& align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onUpdateLayout() override;
		
	private:
		void _updatePen();
		
	private:
		LayoutOrientation m_orientation;
		Color m_color;
		sl_real m_thickness;
		PenStyle m_style;
		Alignment m_gravity;
		
		AtomicRef<Pen> m_pen;
		
	};
	
	class SLIB_EXPORT VerticalLineView : public LineView
	{
	public:
		VerticalLineView();

		~VerticalLineView();

	};
	
	class SLIB_EXPORT HorizontalLineView : public LineView
	{
	public:
		HorizontalLineView();

		~HorizontalLineView();

	};

}

#endif
