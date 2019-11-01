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

#ifndef CHECKHEADER_SLIB_UI_SWITCH_VIEW
#define CHECKHEADER_SLIB_UI_SWITCH_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT SwitchView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		SwitchView();
		
		~SwitchView();
		
	public:
		sl_bool getValue();
		
		sl_bool isLeft();
		
		void switchToLeft(UIUpdateMode mode = UIUpdateMode::Animate);
		
		sl_bool isRight();
		
		void switchToRight(UIUpdateMode mode = UIUpdateMode::Animate);
		
		virtual void setValue(sl_bool value, UIUpdateMode mode = UIUpdateMode::Animate);
		
		
		sl_bool isTextInButton();
		
		void setTextInButton(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		String getTextOn();
		
		void setTextOn(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getTextOff();
		
		void setTextOff(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getLeftText();
		
		void setLeftText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getRightText();
		
		void setRightText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getText();
		
		void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Color getTextColorOn();
		
		void setTextColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColorOff();
		
		void setTextColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Color getTextColor();
		
		void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getThumbOff();
		
		void setThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getThumbOn();
		
		void setThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getThumb();
		
		void setThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getTrackOff();
		
		void setTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getTrackOn();
		
		void setTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getTrack();
		
		void setTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		Ref<Drawable> getPressedThumbOff();
		
		void setPressedThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedThumbOn();
		
		void setPressedThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedThumb();

		void setPressedThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		Ref<Drawable> getPressedTrackOff();
		
		void setPressedTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedTrackColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedTrackOn();
		
		void setPressedTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedTrackColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getPressedTrack();

		void setPressedTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		Ref<Drawable> getHoverThumbOff();
		
		void setHoverThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverThumbOn();
		
		void setHoverThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverThumb();

		void setHoverThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getHoverTrackOff();
		
		void setHoverTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverTrackColorOff(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverTrackOn();
		
		void setHoverTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setHoverTrackColorOn(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getHoverTrack();

		void setHoverTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setHoverTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	public:
		void setPressedState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void setHoverState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(SwitchView, Change, sl_bool newValue)

	protected:
		void onDraw(Canvas* canvas) override;
		
		void onUpdateLayout() override;
		
		void onMouseEvent(UIEvent* ev) override;
		
	protected:
		virtual sl_bool calculateSwitchRegion(UIRect& _out);
		
		virtual void drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst);
		
		virtual void drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst);
		
	private:
		void _changeValue(sl_bool value);
		
		void _onTimerAnimation(Timer* timer);
		
	protected:
		sl_bool m_value;
		sl_bool m_flagTextInButton;
		
		AtomicString m_textOff;
		AtomicString m_textOn;
		
		Color m_textColorOff;
		Color m_textColorOn;

		AtomicRef<Drawable> m_thumbOff;
		AtomicRef<Drawable> m_thumbOn;
		AtomicRef<Drawable> m_trackOff;
		AtomicRef<Drawable> m_trackOn;
		AtomicRef<Drawable> m_pressedThumbOff;
		AtomicRef<Drawable> m_pressedThumbOn;
		AtomicRef<Drawable> m_pressedTrackOff;
		AtomicRef<Drawable> m_pressedTrackOn;
		AtomicRef<Drawable> m_hoverThumbOff;
		AtomicRef<Drawable> m_hoverThumbOn;
		AtomicRef<Drawable> m_hoverTrackOff;
		AtomicRef<Drawable> m_hoverTrackOn;

		sl_bool m_flagDefaultTextOff : 1;
		sl_bool m_flagDefaultTextOn : 1;
		sl_bool m_flagDefaultTextColorOff : 1;
		sl_bool m_flagDefaultTextColorOn : 1;
		sl_bool m_flagDefaultThumbOff : 1;
		sl_bool m_flagDefaultThumbOn : 1;
		sl_bool m_flagDefaultTrackOff : 1;
		sl_bool m_flagDefaultTrackOn : 1;
		sl_bool m_flagDefaultPressedThumbOff : 1;
		sl_bool m_flagDefaultPressedThumbOn : 1;
		sl_bool m_flagDefaultPressedTrackOff : 1;
		sl_bool m_flagDefaultPressedTrackOn : 1;
		sl_bool m_flagDefaultHoverThumbOff : 1;
		sl_bool m_flagDefaultHoverThumbOn : 1;
		sl_bool m_flagDefaultHoverTrackOff : 1;
		sl_bool m_flagDefaultHoverTrackOn : 1;

		sl_real m_posThumb;
		Ref<Timer> m_timer;
		MotionTracker m_tracker;
		Point m_ptMouseDown;
		Time m_timeMouseDown;
		sl_real m_posMouseDown;
		sl_bool m_flagTapping;
		
	};

}

#endif
