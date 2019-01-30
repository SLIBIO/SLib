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
		
		virtual void setValue(sl_bool value, UIUpdateMode mode = UIUpdateMode::Animate);
		
		Ref<Drawable> getThumbOff();
		
		void setThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getThumbOn();
		
		void setThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getTrackOff();
		
		void setTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getTrackOn();
		
		void setTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getPressedThumbOff();
		
		void setPressedThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedThumbOn();
		
		void setPressedThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumbOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getPressedTrackOff();
		
		void setPressedTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPressedTrackOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedTrackOn();
		
		void setPressedTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedTrackOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setPressedTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Ref<Drawable> getHoverThumbOff();
		
		void setHoverThumbOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverThumbOn();
		
		void setHoverThumbOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumb(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverTrackOff();
		
		void setHoverTrackOff(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverTrackOffColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverTrackOn();
		
		void setHoverTrackOn(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setHoverTrackOnColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setHoverTrack(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setHoverTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	public:
		void setHoverState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(SwitchView, Change, sl_bool newValue)

	protected:
		void onDraw(Canvas* canvas) override;
		
		void onUpdateLayout() override;
		
		void onClickEvent(UIEvent* ev) override;
		
	protected:
		void drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst);
		
		void drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst);
		
	private:
		void _onTimerAnimation(Timer* timer);
		
	protected:
		sl_bool m_value;
		
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

		sl_real m_posThumb;
		Ref<Timer> m_timer;
		
	};
	
	
	enum class SwitchLabelValue
	{
		Left = 0,
		Right = 1
	};

	class SLIB_EXPORT SwitchLabelView : public SwitchView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		SwitchLabelView();
		
		~SwitchLabelView();
		
	public:
		SwitchLabelValue getValue();
		
		using SwitchView::setValue;
		
		void setValue(SwitchLabelValue value, UIUpdateMode mode = UIUpdateMode::Animate);
		
		String getLeftText();
		
		void setLeftText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getRightText();
		
		void setRightText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getSelectedTextColor();
		
		void setSelectedTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	public:
		SLIB_DECLARE_EVENT_HANDLER(SwitchLabelView, Change, SwitchLabelValue newValue)

	protected:
		void onDraw(Canvas* canvas) override;
		
		void onUpdateLayout() override;
		
		void onClickEvent(UIEvent* ev) override;
		
	protected:
		AtomicString m_labelLeft;
		AtomicString m_labelRight;
		Color m_textColor;
		Color m_selectedTextColor;
		
	};

}

#endif
