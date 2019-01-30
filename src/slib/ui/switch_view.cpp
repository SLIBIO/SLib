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

#include "slib/ui/switch_view.h"

#include "slib/core/safe_static.h"
#include "slib/core/timer.h"

namespace slib
{

	class _priv_SwitchView_Static
	{
	public:
		Ref<Drawable> thumbOff;
		Ref<Drawable> trackOff;
		Ref<Drawable> pressedThumbOff;
		Ref<Drawable> pressedTrackOff;
		Ref<Drawable> hoverThumbOff;
		Ref<Drawable> hoverTrackOff;
		Ref<Drawable> thumbOn;
		Ref<Drawable> trackOn;
		Ref<Drawable> pressedThumbOn;
		Ref<Drawable> pressedTrackOn;
		Ref<Drawable> hoverThumbOn;
		Ref<Drawable> hoverTrackOn;

		_priv_SwitchView_Static(sl_bool flagLabel)
		{
			if (flagLabel) {
				thumbOff = ColorDrawable::create(Color(255, 255, 255));
				trackOff = ColorDrawable::create(Color(130, 130, 130));
				pressedThumbOff = ColorDrawable::create(Color(255, 255, 255));
				pressedTrackOff = ColorDrawable::create(Color(100, 100, 100));
				hoverThumbOff = ColorDrawable::create(Color(255, 255, 255));
				hoverTrackOff = ColorDrawable::create(Color(120, 120, 120));
				thumbOn = thumbOff;
				trackOn = trackOff;
				pressedThumbOn = pressedThumbOff;
				pressedTrackOn = pressedTrackOff;
				hoverThumbOn = hoverThumbOff;
				hoverTrackOn = hoverTrackOff;
			} else {
				thumbOff = ColorDrawable::create(Color(255, 255, 255));
				trackOff = ColorDrawable::create(Color(120, 120, 120));
				pressedThumbOff = ColorDrawable::create(Color(255, 255, 255));
				pressedTrackOff = ColorDrawable::create(Color(0, 70, 210));
				hoverThumbOff = ColorDrawable::create(Color(255, 255, 255));
				hoverTrackOff = ColorDrawable::create(Color(90, 90, 90));
				thumbOn = ColorDrawable::create(Color(255, 255, 255));
				trackOn = ColorDrawable::create(Color(0, 80, 230));
				pressedThumbOn = ColorDrawable::create(Color(255, 255, 255));
				pressedTrackOn = ColorDrawable::create(Color(90, 90, 90));
				hoverThumbOn = ColorDrawable::create(Color(255, 255, 255));
				hoverTrackOn = ColorDrawable::create(Color(0, 70, 210));
			}
		}
	};
	
	SLIB_SAFE_STATIC_GETTER(_priv_SwitchView_Static, _priv_SwitchView_getStatic, sl_false)
	SLIB_SAFE_STATIC_GETTER(_priv_SwitchView_Static, _priv_SwitchLabelView_getStatic, sl_true)

	static Ref<Drawable> const& _priv_SwitchView_resolveDrawable(const Ref<Drawable>& drawableOriginal, const Ref<Drawable>& drawableCommon, const Ref<Drawable>& drawableShared)
	{
		if (drawableOriginal.isNotNull()) {
			return drawableOriginal;
		}
		if (drawableCommon.isNotNull()) {
			if (drawableCommon->isColor()) {
				return drawableShared;
			}
			return drawableCommon;
		}
		return Ref<Drawable>::null();
	}
	
	
	SLIB_DEFINE_OBJECT(SwitchView, View)

	SwitchView::SwitchView()
	{
		m_value = sl_false;
		
		_priv_SwitchView_Static* s = _priv_SwitchView_getStatic();
		if (s) {
			m_trackOff = s->trackOff;
			m_trackOn = s->trackOn;
			m_thumbOff = s->thumbOff;
			m_thumbOn = s->thumbOn;
		}
		
		m_posThumb = 0;
		
	}
	
	SwitchView::~SwitchView()
	{
	}
	
	sl_bool SwitchView::getValue()
	{
		return m_value;
	}
	
	void SwitchView::setValue(sl_bool value, UIUpdateMode mode)
	{
		{
			ObjectLocker lock(this);
			m_value = value;
			if (SLIB_UI_UPDATE_MODE_IS_ANIMATE(mode)) {
				if (m_timer.isNull()) {
					m_timer = startTimer(SLIB_FUNCTION_WEAKREF(SwitchView, _onTimerAnimation, this), 10);
				}
				return;
			}
			m_timer.setNull();
			m_posThumb = value ? 1.0f : 0.0f;
		}
		invalidate(mode);
	}

#define SWITCH_VIEW_GET_SET_DRAWABLE_1(BIG_NAME, SMALL_NAME) \
	Ref<Drawable> SwitchView::get##BIG_NAME() \
	{ \
		return m_##SMALL_NAME; \
	} \
	void SwitchView::set##BIG_NAME(const Ref<Drawable>& drawable, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME = drawable; \
		invalidate(mode); \
	} \
	void SwitchView::set##BIG_NAME##Color(const Color& color, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME = ColorDrawable::create(color); \
		invalidate(mode); \
	}

#define SWITCH_VIEW_GET_SET_DRAWABLE_2(BIG_NAME, SMALL_NAME) \
	SWITCH_VIEW_GET_SET_DRAWABLE_1(BIG_NAME##Off, SMALL_NAME##Off) \
	SWITCH_VIEW_GET_SET_DRAWABLE_1(BIG_NAME##On, SMALL_NAME##On) \
	void SwitchView::set##BIG_NAME(const Ref<Drawable>& drawable, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##Off = drawable; \
		m_##SMALL_NAME##On = drawable; \
		invalidate(mode); \
	} \
	void SwitchView::set##BIG_NAME##Color(const Color& color, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##Off = ColorDrawable::create(color); \
		m_##SMALL_NAME##On = ColorDrawable::create(color); \
		invalidate(mode); \
	}

	SWITCH_VIEW_GET_SET_DRAWABLE_2(Thumb, thumb)
	SWITCH_VIEW_GET_SET_DRAWABLE_2(Track, track)
	SWITCH_VIEW_GET_SET_DRAWABLE_2(PressedThumb, pressedThumb)
	SWITCH_VIEW_GET_SET_DRAWABLE_2(PressedTrack, pressedTrack)
	SWITCH_VIEW_GET_SET_DRAWABLE_2(HoverThumb, hoverThumb)
	SWITCH_VIEW_GET_SET_DRAWABLE_2(HoverTrack, hoverTrack)
	
	void SwitchView::setHoverState(sl_bool flagState, UIUpdateMode mode)
	{
		View::setHoverState(flagState, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
		invalidate(mode);
	}

	SLIB_DEFINE_EVENT_HANDLER(SwitchView, Change, sl_bool newValue)
	
	void SwitchView::dispatchChange(sl_bool newValue)
	{
		SLIB_INVOKE_EVENT_HANDLER(Change, newValue)
	}
	
	void SwitchView::onDraw(Canvas* canvas)
	{
		_priv_SwitchView_Static* s = _priv_SwitchView_getStatic();
		if (!s) {
			return;
		}
		UIRect rect = getBoundsInnerPadding();
		if (!(rect.isValidSize())) {
			return;
		}
		Ref<Drawable> track;
		Ref<Drawable> thumb;
		if (isPressedState()) {
			if (m_value) {
				track = _priv_SwitchView_resolveDrawable(m_pressedTrackOn, m_trackOn, s->pressedTrackOn);
				thumb = _priv_SwitchView_resolveDrawable(m_pressedThumbOn, m_thumbOn, s->pressedThumbOn);
			} else {
				track = _priv_SwitchView_resolveDrawable(m_pressedTrackOff, m_trackOff, s->pressedTrackOff);
				thumb = _priv_SwitchView_resolveDrawable(m_pressedThumbOff, m_thumbOff, s->pressedThumbOff);
			}
		} else if (isHoverState()) {
			if (m_value) {
				track = _priv_SwitchView_resolveDrawable(m_hoverTrackOn, m_trackOn, s->hoverTrackOn);
				thumb = _priv_SwitchView_resolveDrawable(m_hoverThumbOn, m_thumbOn, s->hoverThumbOn);
			} else {
				track = _priv_SwitchView_resolveDrawable(m_hoverTrackOff, m_trackOff, s->hoverTrackOff);
				thumb = _priv_SwitchView_resolveDrawable(m_hoverThumbOff, m_thumbOff, s->hoverThumbOff);
			}
		} else {
			if (m_value) {
				track = m_trackOn;
				thumb = m_thumbOn;
			} else {
				track = m_trackOff;
				thumb = m_thumbOff;
			}
		}
		drawTrack(canvas, track, rect);
		if (thumb.isNotNull()) {
			sl_ui_len widthTrack = rect.getWidth();
			sl_ui_len widthThumb = widthTrack / 2;
			rect.left = (sl_ui_pos)(m_posThumb * (widthTrack - widthThumb));
			rect.setWidth(widthThumb);
			drawThumb(canvas, thumb, rect);
		}
	}
	
	void SwitchView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		sl_ui_len paddingWidth = getPaddingLeft() + getPaddingRight();
		sl_ui_len paddingHeight = getPaddingTop() + getPaddingBottom();
		if (flagHorizontal) {
			if (flagVertical) {
				sl_ui_len h = (sl_ui_len)(getFontSize());
				setLayoutHeight(h + paddingHeight);
				setLayoutWidth(h * 2 + paddingWidth);
			} else {
				sl_ui_len h = getLayoutHeight() - paddingHeight;
				if (h < 0) {
					h = 0;
				}
				setLayoutWidth(h * 2 + paddingWidth);
			}
		} else {
			if (flagVertical) {
				sl_ui_len w = getLayoutWidth() - paddingWidth;
				if (w < 0) {
					w = 0;
				}
				setLayoutHeight(w / 2 + paddingHeight);
			}
		}
	}
	
	void SwitchView::onClickEvent(UIEvent* ev)
	{
		sl_bool value = m_value ? sl_false : sl_true;
		setValue(value);
		dispatchChange(value);
	}

	void SwitchView::drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst)
	{
		if (track.isNull()) {
			return;
		}
		Color color;
		if (ColorDrawable::check(track.get(), &color)) {
			sl_real height = rectDst.getHeight();
			sl_real width = rectDst.getWidth();
			if (height < width) {
				height /= 2;
				canvas->fillRoundRect(rectDst, Size(height, height), color);
			} else {
				canvas->fillRectangle(rectDst, color);
			}
		} else {
			canvas->draw(rectDst, track);
		}
	}
	
	void SwitchView::drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst)
	{
		if (thumb.isNull()) {
			return;
		}
		Color color;
		if (ColorDrawable::check(thumb.get(), &color)) {
			sl_real width = rectDst.getWidth();
			sl_real height = rectDst.getHeight();
			sl_real padding = height / 20;
			if (padding < 1) {
				padding = 1;
			}
			Rectangle rect = rectDst;
			if (width > padding * 2 && height > padding * 2) {
				rect.left += padding;
				rect.right -= padding;
				rect.top += padding;
				rect.bottom -= padding;
				width = rect.getWidth();
				height = rect.getHeight();
			}
			if (height < width) {
				height /= 2;
				canvas->fillRoundRect(rect, Size(height, height), color);
			} else {
				rect.top = (rect.top + rect.bottom - width) / 2;
				rect.setHeight(width);
				canvas->fillEllipse(rect, color);
			}
		} else {
			canvas->draw(rectDst, thumb);
		}
	}
	
	void SwitchView::_onTimerAnimation(Timer* timer)
	{
		do {
			ObjectLocker lock(this);
			sl_real target = m_value ? 1.0f : 0.0f;
			sl_real pos = m_posThumb;
			sl_real d = 0.1f;
			sl_real a = Math::abs(target - pos);
			if (a < d || a > 2) {
				m_posThumb = target;
				m_timer.setNull();
				break;
			}
			if (target > m_posThumb) {
				m_posThumb += d;
			} else {
				m_posThumb -= d;
			}
		} while (0);
		invalidate();
	}

	
	SLIB_DEFINE_OBJECT(SwitchLabelView, SwitchView)
	
	SwitchLabelView::SwitchLabelView()
	{
		_priv_SwitchView_Static* s = _priv_SwitchLabelView_getStatic();
		if (s) {
			m_trackOff = s->trackOff;
			m_trackOn = s->trackOn;
			m_thumbOff = s->thumbOff;
			m_thumbOn = s->thumbOn;
		}
		m_textColor = Color::White;
		m_selectedTextColor = Color::Black;
	}
	
	SwitchLabelView::~SwitchLabelView()
	{
	}
	
	SwitchLabelValue SwitchLabelView::getValue()
	{
		return m_value ? SwitchLabelValue::Right : SwitchLabelValue::Left;
	}
	
	void SwitchLabelView::setValue(SwitchLabelValue value, UIUpdateMode mode)
	{
		SwitchView::setValue(value == SwitchLabelValue::Right, mode);
	}
	
	String SwitchLabelView::getLeftText()
	{
		return m_labelLeft;
	}
	
	void SwitchLabelView::setLeftText(const String& text, UIUpdateMode mode)
	{
		m_labelLeft = text;
		invalidate(mode);
	}
	
	String SwitchLabelView::getRightText()
	{
		return m_labelRight;
	}
	
	void SwitchLabelView::setRightText(const String& text, UIUpdateMode mode)
	{
		m_labelRight = text;
		invalidate(mode);
	}
	
	Color SwitchLabelView::getTextColor()
	{
		return m_textColor;
	}
	
	void SwitchLabelView::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColor = color;
		invalidate(mode);
	}
	
	Color SwitchLabelView::getSelectedTextColor()
	{
		return m_selectedTextColor;
	}
	
	void SwitchLabelView::setSelectedTextColor(const Color& color, UIUpdateMode mode)
	{
		m_selectedTextColor = color;
		invalidate(mode);
	}
	
	SLIB_DEFINE_EVENT_HANDLER(SwitchLabelView, Change, SwitchLabelValue newValue)
	
	void SwitchLabelView::dispatchChange(SwitchLabelValue newValue)
	{
		SLIB_INVOKE_EVENT_HANDLER(Change, newValue)
	}
	
	void SwitchLabelView::onDraw(Canvas* canvas)
	{
		_priv_SwitchView_Static* s = _priv_SwitchLabelView_getStatic();
		if (!s) {
			return;
		}
		UIRect rect = getBoundsInnerPadding();
		if (!(rect.isValidSize())) {
			return;
		}
		Ref<Drawable> track;
		Ref<Drawable> thumb;
		if (isPressedState()) {
			if (m_value) {
				track = _priv_SwitchView_resolveDrawable(m_pressedTrackOn, m_trackOn, s->pressedTrackOn);
				thumb = _priv_SwitchView_resolveDrawable(m_pressedThumbOn, m_thumbOn, s->pressedThumbOn);
			} else {
				track = _priv_SwitchView_resolveDrawable(m_pressedTrackOff, m_trackOff, s->pressedTrackOff);
				thumb = _priv_SwitchView_resolveDrawable(m_pressedThumbOff, m_thumbOff, s->pressedThumbOff);
			}
		} else if (isHoverState()) {
			if (m_value) {
				track = _priv_SwitchView_resolveDrawable(m_hoverTrackOn, m_trackOn, s->hoverTrackOn);
				thumb = _priv_SwitchView_resolveDrawable(m_hoverThumbOn, m_thumbOn, s->hoverThumbOn);
			} else {
				track = _priv_SwitchView_resolveDrawable(m_hoverTrackOff, m_trackOff, s->hoverTrackOff);
				thumb = _priv_SwitchView_resolveDrawable(m_hoverThumbOff, m_thumbOff, s->hoverThumbOff);
			}
		} else {
			if (m_value) {
				track = m_trackOn;
				thumb = m_thumbOn;
			} else {
				track = m_trackOff;
				thumb = m_thumbOff;
			}
		}
		drawTrack(canvas, track, rect);
		if (thumb.isNotNull()) {
			sl_ui_len widthTrack = rect.getWidth();
			sl_ui_len widthThumb = widthTrack / 2;
			rect.left = (sl_ui_pos)(m_posThumb * (widthTrack - widthThumb));
			rect.setWidth(widthThumb);
			drawThumb(canvas, thumb, rect);
			
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				rect.left = 0;
				rect.setWidth(widthThumb);
				canvas->drawText(m_labelLeft, rect, font, m_value ? m_textColor : m_selectedTextColor, Alignment::MiddleCenter);
				rect.left = widthThumb;
				rect.setWidth(widthThumb);
				canvas->drawText(m_labelRight, rect, font, m_value ? m_selectedTextColor : m_textColor, Alignment::MiddleCenter);
			}
		}
	}
	
	void SwitchLabelView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		sl_ui_len paddingWidth = getPaddingLeft() + getPaddingRight();
		sl_ui_len paddingHeight = getPaddingTop() + getPaddingBottom();
		if (flagVertical) {
			sl_ui_len h = (sl_ui_len)(getFontSize() * 1.5f);
			setLayoutHeight(h + paddingHeight);
		}
		if (flagHorizontal) {
			sl_ui_len len = 0;
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				len = (sl_ui_len)(Math::max(font->measureText(m_labelLeft).x, font->measureText(m_labelRight).x));
				len *= 2;
			}
			len += paddingWidth;
			len += getLayoutHeight() * 2;
			setLayoutWidth(len);
		}
	}
	
	void SwitchLabelView::onClickEvent(UIEvent* ev)
	{
		SwitchLabelValue value = ev->getX() < getWidth() / 2 ? SwitchLabelValue::Left : SwitchLabelValue::Right;
		setValue(value);
		dispatchChange(value);
		SwitchView::dispatchChange(value == SwitchLabelValue::Right);
	}
	
}
