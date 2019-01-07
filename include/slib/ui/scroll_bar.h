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

#ifndef CHECKHEADER_SLIB_UI_SCROLL_BAR
#define CHECKHEADER_SLIB_UI_SCROLL_BAR

#include "definition.h"

#include "view.h"

namespace slib
{
	
	class SLIB_EXPORT ScrollBar : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ScrollBar(LayoutOrientation orientation = LayoutOrientation::Horizontal);
		
		~ScrollBar();

	public:
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isVertical();
		
		void setVertical(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHorizontal();
		
		void setHorizontal(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getValue();
		
		void setValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setValueOfOutRange(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getPage();
		
		void setPage(sl_scroll_pos page, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getLine();
		
		void setLine(sl_scroll_pos line, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getMinimumValue();
		
		void setMinimumValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getMaximumValue();
		
		void setMaximumValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getRange();
		
		void setRange(sl_scroll_pos range, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getThumbDrawable();
		
		void setThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedThumbDrawable();
		
		void setPressedThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverThumbDrawable();
		
		void setHoverThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getTrackDrawable();
		
		void setTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedTrackDrawable();
		
		void setPressedTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverTrackDrawable();
		
		void setHoverTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		float getMinimumThumbLengthRatio();
		
		void setMinimumThumbLengthRatio(float ratio);
		
		
		sl_bool getThumbPositionRange(sl_ui_pos& begin, sl_ui_pos& end);
		
		sl_bool getThumbRegion(UIRect& _out);
		
		sl_scroll_pos getValueFromThumbPosition(sl_ui_pos pos);
		
		sl_bool isValid();

	public:
		SLIB_DECLARE_EVENT_HANDLER(ScrollBar, Change, sl_scroll_pos value)
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onMouseWheelEvent(UIEvent* ev) override;
		
	protected:
		void _setHoverThumb(sl_bool flag);
		
	protected:
		LayoutOrientation m_orientation;
		sl_scroll_pos m_value;
		sl_scroll_pos m_page;
		sl_scroll_pos m_line;
		sl_scroll_pos m_value_min;
		sl_scroll_pos m_value_max;
		
		AtomicRef<Drawable> m_thumb;
		AtomicRef<Drawable> m_pressedThumb;
		AtomicRef<Drawable> m_hoverThumb;
		
		AtomicRef<Drawable> m_track;
		AtomicRef<Drawable> m_pressedTrack;
		AtomicRef<Drawable> m_hoverTrack;
		
		float m_thumb_len_ratio_min;
		
		sl_ui_pos m_posDown;
		sl_scroll_pos m_valueDown;
		sl_bool m_flagHoverThumb;
		
	};

}

#endif
