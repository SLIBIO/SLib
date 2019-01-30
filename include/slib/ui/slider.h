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

#ifndef CHECKHEADER_SLIB_UI_SLIDER
#define CHECKHEADER_SLIB_UI_SLIDER

#include "definition.h"

#include "progress_bar.h"

namespace slib
{
	
	class SLIB_EXPORT Slider : public ProgressBar
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Slider(LayoutOrientation orientation = LayoutOrientation::Horizontal);
		
		~Slider();

	public:
		Ref<Drawable> getThumbDrawable();
		
		void setThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedThumbDrawable();
		
		void setPressedThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverThumbDrawable();
		
		void setHoverThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		const UISize& getThumbSize();
		
		virtual void setThumbSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbSize(sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getThumbWidth();
		
		void setThumbWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getThumbHeight();
		
		void setThumbHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(Slider, Change, float value)
		SLIB_DECLARE_EVENT_HANDLER(Slider, ChangeSecondary, float value)
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onMouseWheelEvent(UIEvent* ev) override;
		
		void onKeyEvent(UIEvent* event) override;
		
	protected:
		void drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst);
		
		void drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst);
		
		sl_ui_pos getThumbLength();
		
		sl_ui_pos getStartPadding();
		
		sl_ui_pos getEndPadding();
		
		sl_ui_pos getMinimumPadding();
		
		sl_ui_pos getPositionFromValue(float value);
		
		float getValueFromPosition(sl_ui_pos pos);
		
		void getRegions(UIRect& outTrack, UIRect& outProgress, UIRect& outSecondaryProgress, UIRect& outThumb, UIRect& outSecondaryThumb);
		
		void setHoverThumb(int index);
		
		void changeValue(float value, sl_bool flagChange2);
		
	protected:
		AtomicRef<Drawable> m_thumb;
		AtomicRef<Drawable> m_pressedThumb;
		AtomicRef<Drawable> m_hoverThumb;
		
		UISize m_thumbSize;
		
		int m_indexHoverThumb;
		int m_indexPressedThumb;
		
	};

}

#endif
